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

#include <cstdint>

#include <daw/daw_algorithm.h>
#include <daw/daw_keep_n.h>
#include <daw/daw_parser_helper_sv.h>
#include <daw/daw_static_string.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

namespace daw {
	namespace {
		using index_t = intmax_t;
		struct point_t {
			index_t x;
			index_t y;
		};

		template<size_t N>
		struct point_array_t {
			std::array<point_t, N> points{};
			index_t left = std::numeric_limits<index_t>::max( );
			index_t top = std::numeric_limits<index_t>::max( );
			index_t right = std::numeric_limits<index_t>::min( );
			index_t bottom = std::numeric_limits<index_t>::min( );

			using iterator = typename std::array<point_t, N>::iterator;
		};

		template<index_t Max_x, index_t Max_y, index_t Min_x = 0, index_t Min_y = 0>
		struct grid_t {
			static inline constexpr size_t const width =
			  static_cast<size_t>( Max_x - Min_x );
			static inline constexpr size_t const height =
			  static_cast<size_t>( Max_y - Min_y );

			static_assert( width > 0 );
			static_assert( height > 0 );

			static inline constexpr size_t const m_size = width * height;

			using size_type = index_t;
			using value_type = intmax_t;
			using reference = value_type &;
			using const_reference = value_type const &;

		private:
			using values_type = std::array<value_type, m_size>;

			values_type m_values{};

			static constexpr size_t location( size_type x, size_type y ) noexcept {
				return static_cast<size_t>( x - Min_x ) +
				       ( static_cast<size_t>( y - Min_y ) * width );
			}

		public:
			using iterator = typename values_type::iterator;
			using const_iterator = typename values_type::const_iterator;

			// Some sentinals
			static inline constexpr auto const no_value_v =
			  std::numeric_limits<value_type>::max( );

			static inline constexpr auto const common_v =
			  std::numeric_limits<value_type>::max( ) - 1;

			constexpr bool has_value( index_t x, index_t y ) const noexcept {
				return operator( )( x, y ) < common_v;
			}

			constexpr bool is_empty( index_t x, index_t y ) const noexcept {
				return operator( )( x, y ) == no_value_v;
			}

			constexpr index_t min_x( ) const noexcept {
				return Min_x;
			}

			constexpr index_t max_x( ) const noexcept {
				return Max_x;
			}

			constexpr index_t min_y( ) const noexcept {
				return Min_y;
			}

			constexpr index_t max_y( ) const noexcept {
				return Max_y;
			}

			constexpr grid_t( ) noexcept {
				daw::algorithm::fill( std::begin( m_values ), std::end( m_values ),
				                      no_value_v );
			}

			constexpr reference operator( )( size_type x, size_type y ) noexcept {
				return m_values[location( x, y )];
			}

			constexpr const_reference operator( )( size_type x, size_type y ) const
			  noexcept {
				return m_values[location( x, y )];
			}

			constexpr iterator begin( ) noexcept {
				return std::begin( m_values );
			}

			constexpr const_iterator begin( ) const noexcept {
				return std::begin( m_values );
			}

			constexpr const_iterator cbegin( ) const noexcept {
				return std::cbegin( m_values );
			}

			constexpr iterator end( ) noexcept {
				return std::end( m_values );
			}

			constexpr const_iterator end( ) const noexcept {
				return std::end( m_values );
			}

			constexpr const_iterator cend( ) const noexcept {
				return std::cend( m_values );
			}
		};

		template<size_t N>
		constexpr point_array_t<N>
		parse_grid( std::array<daw::string_view, N> const &locs ) {
			point_array_t<N> results{};

			daw::algorithm::transform(
			  begin( locs ), end( locs ), results.points.data( ),
			  [&]( auto sv ) -> point_t {
				  sv = daw::parser::trim_left( sv );
				  auto x = daw::parser::parse_unsigned_int<index_t>(
				    sv.pop_front( sv.find_first_of( ',' ) ) );

				  sv.remove_prefix( );
				  sv = daw::parser::trim_left( sv );
				  auto y = daw::parser::parse_unsigned_int<index_t>(
				    sv.pop_front( sv.find_first_of( ',' ) ) );
				  if( x < results.left ) {
					  results.left = x;
				  }
				  if( x > results.right ) {
					  results.right = x;
				  }
				  if( y < results.top ) {
					  results.top = y;
				  }
				  if( y > results.bottom ) {
					  results.bottom = y;
				  }
				  return {x, y};
			  } );
			return results;
		}

		constexpr intmax_t man_distance( point_t a, point_t b ) noexcept {
			if( b.x > a.x ) {
				std::swap( b.x, a.x );
			}
			if( b.y > a.y ) {
				std::swap( b.y, a.y );
			}
			auto const x = a.x - b.x;
			auto const y = a.y - b.y;
			return x + y;
		}

		struct p_compare {
			template<typename T, typename U>
			constexpr bool operator( )( T &&lhs, U &&rhs ) const {
				return lhs.second < rhs.second;
			}
		};

		template<size_t N, typename Grid>
		constexpr std::array<bool, N> find_invalid_points( Grid &&grid ) {
			std::array<bool, N> invalid_points{};
			// All perimiter points are for removal
			// do not count their areas here as it doesn't matter
			{
				auto const set_invalid = [&]( index_t x, index_t y ) {
					if( grid.has_value( x, y ) ) {
						invalid_points[static_cast<size_t>( grid( x, y ) )] = true;
					}
				};

				for( index_t x = grid.min_x( ); x < grid.max_x( ); ++x ) {
					set_invalid( x, grid.min_y( ) );
					set_invalid( x, grid.max_y( ) - 1 );
				}
				for( index_t y = grid.min_y( ) + 1; y < grid.max_y( ) - 1; ++y ) {
					set_invalid( grid.min_x( ), y );
					set_invalid( grid.max_x( ) - 1, y );
				}
			}
			return invalid_points;
		}

		template<intmax_t Max_x, intmax_t Max_y, size_t N>
		constexpr grid_t<Max_x, Max_y>
		calc_distances( point_array_t<N> const &coords ) {
			grid_t<360, 360> grid{};
			using dist_item_t = std::pair<intmax_t, index_t>;
			for( index_t x = grid.min_x( ); x < grid.max_x( ); ++x ) {
				for( index_t y = grid.min_y( ); y < grid.max_y( ); ++y ) {
					auto const pt = point_t{x, y};

					daw::keep_n<dist_item_t, 2, daw::keep_n_order::ascending, p_compare>
					  two_smallest( {0ULL, std::numeric_limits<index_t>::max( )} );

					for( size_t n = 0; n < coords.points.size( ); ++n ) {
						two_smallest.insert( {n, man_distance( pt, coords.points[n] )} );
					}

					if( two_smallest.front( ).second == two_smallest.back( ).second ) {
						grid( x, y ) = grid.common_v;
					} else {
						grid( x, y ) = two_smallest[0].first;
					}
				}
			}
			return grid;
		}

		template<size_t N>
		constexpr auto part_01( point_array_t<N> const &coords ) {
			static_assert( N >= 2 );

			auto const grid = calc_distances<360, 360>( coords );
			auto const invalid_points = find_invalid_points<N>( grid );

			std::array<size_t, N> areas{};
			// Calc areas for non-permimiter grid locations
			for( index_t x = grid.min_x( ) + 1; x < grid.max_x( ) - 1; ++x ) {
				for( index_t y = grid.min_x( ) + 1; y < grid.max_x( ) - 1; ++y ) {
					if( grid.has_value( x, y ) ) {
						++areas[static_cast<size_t>( grid( x, y ) )];
					}
				}
			}
			index_t max_point = [&]( ) {
				size_t n = 0;
				// assume at least 1 is valid
				while( invalid_points[n] ) {
					++n;
				}
				return static_cast<index_t>( n );
			}( );
			uintmax_t max_area = areas[static_cast<size_t>( max_point )];
			for( size_t n = static_cast<size_t>( max_point ); n < N; ++n ) {
				if( invalid_points[n] ) {
					continue;
				}
				if( areas[n] > max_area ) {
					max_point = static_cast<index_t>( n );
					max_area = areas[n];
				}
			}
			return max_area;
		}

		template<size_t MaxDistance, size_t N>
		constexpr auto part_02( point_array_t<N> const &coords ) {
			size_t total = 0;
			auto const min_pos = -200;
			auto const max_pos = 575;
			for( index_t x = min_pos; x < max_pos; ++x ) {
				for( index_t y = min_pos; y < max_pos; ++y ) {
					auto tmp = daw::algorithm::accumulate(
					  begin( coords.points ), end( coords.points ), 0LL,
					  [pt = point_t{x, y}]( auto tot, auto pt2 ) {
						  tot += man_distance( pt, pt2 );
						  return tot;
					  } );
					if( static_cast<size_t>( tmp ) < MaxDistance ) {
						++total;
					}
				}
			}
			return total;
		}
	} // namespace
} // namespace daw
