// The MIT License (MIT)
//
// Copyright (c) 2018 Darrell Wright
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and / or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <iterator>
#include <list>
#include <set>

#include <daw/daw_algorithm.h>
#include <daw/daw_keep_n.h>
#include <daw/daw_parser_helper_sv.h>
#include <daw/daw_static_string.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

namespace daw {
	namespace {
		struct point_t {
			intmax_t x;
			intmax_t y;
		};

		constexpr bool operator==( point_t lhs, point_t rhs ) noexcept {
			return lhs.x == rhs.x and lhs.y == rhs.y;
		}
		constexpr bool operator!=( point_t lhs, point_t rhs ) noexcept {
			return lhs.x != rhs.x or lhs.y != rhs.y;
		}

		constexpr bool operator<( point_t lhs, point_t rhs ) noexcept {
			return lhs.y < rhs.y or ( lhs.y == rhs.y and lhs.x < rhs.x );
		}

		constexpr bool operator>( point_t lhs, point_t rhs ) noexcept {
			return lhs.y > rhs.y or ( lhs.y == rhs.y and lhs.x > rhs.x );
		}

		constexpr bool operator<=( point_t lhs, point_t rhs ) noexcept {
			return lhs < rhs or lhs == rhs;
		}

		constexpr bool operator>=( point_t lhs, point_t rhs ) noexcept {
			return lhs > rhs or lhs == rhs;
		}

		struct light_data_t {
			point_t position;
			point_t velocity;
		};

		constexpr bool operator<( light_data_t const &lhs,
		                          light_data_t const &rhs ) noexcept {
			return lhs.position < rhs.position;
		}

		constexpr bool operator==( light_data_t const &lhs,
		                           light_data_t const &rhs ) noexcept {
			return lhs.position == rhs.position and lhs.velocity == rhs.velocity;
		}
		constexpr bool operator!=( light_data_t const &lhs,
		                           light_data_t const &rhs ) noexcept {
			return lhs.position != rhs.position or lhs.velocity != rhs.velocity;
		}

		template<size_t N>
		std::array<light_data_t, N> constexpr process_data(
		  std::array<daw::string_view, N> const &arry ) noexcept {

			// "position=< 7,  0> velocity=<-1,  0>"_sv,
			std::array<light_data_t, N> result{};
			daw::algorithm::transform(
			  begin( arry ), end( arry ), result.data( ), []( daw::string_view sv ) {
				  sv.remove_prefix( 10 );
				  sv = daw::parser::trim_left( sv );
				  auto const x = daw::parser::parse_int<intmax_t>(
				    sv.pop_front( sv.find_first_of( ',' ) ) );
				  sv.remove_prefix( 1 );
				  sv = daw::parser::trim_left( sv );
				  auto const y = daw::parser::parse_int<intmax_t>(
				    sv.pop_front( sv.find_first_of( '>' ) ) );
				  sv.remove_prefix( 12 );
				  sv = daw::parser::trim_left( sv );
				  auto const dx = daw::parser::parse_int<intmax_t>(
				    sv.pop_front( sv.find_first_of( ',' ) ) );
				  sv.remove_prefix( 1 );
				  sv = daw::parser::trim_left( sv );
				  auto const dy = daw::parser::parse_int<intmax_t>(
				    sv.pop_front( sv.find_first_of( '>' ) ) );
				  return light_data_t{point_t{x, y}, point_t{dx, dy}};
			  } );
			return result;
		}

		template<size_t N>
		constexpr std::array<light_data_t, N>
		do_tick( std::array<light_data_t, N> data, size_t num_steps = 1 ) {
			daw::algorithm::transform(
			  begin( data ), end( data ), begin( data ),
			  [num_steps]( light_data_t ld ) {
				  ld.position.x =
				    ld.position.x +
				    ( ld.velocity.x * static_cast<intmax_t>( num_steps ) );
				  ld.position.y =
				    ld.position.y +
				    ( ld.velocity.y * static_cast<intmax_t>( num_steps ) );
				  return ld;
			  } );
			return data;
		}

		constexpr size_t man_distance( light_data_t const &ld_a,
		                               light_data_t const &ld_b ) noexcept {
			auto a = ld_a.position;
			auto b = ld_b.position;
			if( b.x > a.x ) {
				std::swap( b.x, a.x );
			}
			if( b.y > a.y ) {
				std::swap( b.y, a.y );
			}
			auto const x = a.x - b.x;
			auto const y = a.y - b.y;
			return static_cast<size_t>( x + y );
		}

		template<size_t N>
		auto find_bounding_box( std::array<light_data_t, N> const &arry ) {
			struct bounding_box_t {
				point_t min_pt;
				point_t max_pt;

				size_t height( ) const noexcept {
					return static_cast<size_t>( max_pt.y - min_pt.y );
				}

				size_t width( ) const noexcept {
					return static_cast<size_t>( max_pt.x - min_pt.x );
				}

				size_t area( ) const noexcept {
					return width( ) * height( );
				}
			};
			bounding_box_t bounding_box{arry[0].position, arry[0].position};

			for( size_t n = 1; n < arry.size( ); ++n ) {
				auto const &pt = arry[n].position;
				if( pt.x < bounding_box.min_pt.x ) {
					bounding_box.min_pt.x = pt.x;
				}
				if( pt.y < bounding_box.min_pt.y ) {
					bounding_box.min_pt.y = pt.y;
				}
				if( pt.x > bounding_box.max_pt.x ) {
					bounding_box.max_pt.x = pt.x;
				}
				if( pt.y > bounding_box.max_pt.y ) {
					bounding_box.max_pt.y = pt.y;
				}
			}
			return bounding_box;
		}

		template<size_t N>
		constexpr bool operator<( std::array<light_data_t, N> const &lhs,
		                          std::array<light_data_t, N> const &rhs ) {
			return find_bounding_box( lhs ).area( ) <
			       find_bounding_box( rhs ).area( );
		}

		template<size_t N>
		std::pair<size_t, size_t>
		test_areas( std::array<light_data_t, N> const &data, size_t min_pos,
		            size_t max_pos ) {

			auto range_size = max_pos - min_pos;
			if( range_size < 6 ) {
				auto a = find_bounding_box( do_tick( data, min_pos ) ).area( );
				for( size_t n=min_pos + 1; n<=max_pos; ++n ) {
					auto b = find_bounding_box( do_tick( data, n ) ).area( );
					if( b >= a ) {
						return {n-1, a};
					}
					a = b;
				}
				return {min_pos, a};
			}

			auto const mid_pos = min_pos + range_size/2;
			auto const b0 = find_bounding_box( do_tick( data, mid_pos - 1 ) ).area( );
			auto const b1 = find_bounding_box( do_tick( data, mid_pos + 1 ) ).area( );
			auto db = static_cast<intmax_t>( b1 ) - static_cast<intmax_t>( b0 );
			if( db <= 0 ) {
				return test_areas( data, mid_pos-1, max_pos );
			}
			return test_areas( data, min_pos, mid_pos+1 );
		}

		template<size_t N>
		std::string part_01( std::array<light_data_t, N> const &arry ) {
			auto min_data = arry;
			auto pos_info = test_areas( arry, 0, 100'000 );
			min_data = do_tick( arry, pos_info.first );
			auto bound = find_bounding_box( min_data );
			std::vector<std::string> result{};
			result.resize( bound.height( ) + 1,
			               std::string( bound.width( ) + 1, ' ' ) );

			for( light_data_t const &ld : min_data ) {
				size_t const x = static_cast<size_t>( ld.position.x - bound.min_pt.x );
				size_t const y = static_cast<size_t>( ld.position.y - bound.min_pt.y );
				result[y][x] = '#';
			}
			std::string result_str{};
			for( auto const &s : result ) {
				result_str += s + '\n';
			}
			result_str +=
			  "\nConverged on step: " + std::to_string( pos_info.first ) + '\n';
			return result_str;
		}
	} // namespace
} // namespace daw
