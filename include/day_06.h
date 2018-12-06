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
#include <daw/daw_parser_helper_sv.h>
#include <daw/daw_static_string.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

namespace daw {
	namespace {
		struct point_t {
			int16_t x;
			int16_t y;
		};

		template<size_t N>
		struct point_array_t {
			std::array<point_t, N> points{};
			int16_t left = std::numeric_limits<int16_t>::max( );
			int16_t top = std::numeric_limits<int16_t>::max( );
			int16_t right = std::numeric_limits<int16_t>::min( );
			int16_t bottom = std::numeric_limits<int16_t>::min( );

			using iterator = typename std::array<point_t, N>::iterator;
		};

		template<int16_t width, int16_t height>
		struct grid_t {
			static inline constexpr size_t const m_size =
			  static_cast<size_t>( width * height );

			using size_type = int16_t;
			using value_type = intmax_t;
			static inline constexpr auto const no_value_v =
			  std::numeric_limits<value_type>::max( );
			static inline constexpr auto const common_v =
			  std::numeric_limits<value_type>::max( ) - 1;

			std::array<value_type, m_size> m_values{};
			size_type off_x = 0;
			size_type off_y = 0;

			constexpr grid_t( ) noexcept {
				for( auto &v : m_values ) {
					v = no_value_v;
				}
			}

			constexpr auto &operator( )( size_type x, size_type y ) noexcept {
				return m_values[static_cast<size_t>( x + off_x ) +
				                ( static_cast<size_t>( y + off_y ) *
				                  static_cast<size_t>( width ) )];
			}

			constexpr auto const &operator( )( size_type x, size_type y ) const
			  noexcept {
				return m_values[static_cast<size_t>( x + off_x ) +
				                ( static_cast<size_t>( y + off_y ) *
				                  static_cast<size_t>( width ) )];
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
				  auto x = daw::parser::parse_unsigned_int<int16_t>(
				    sv.pop_front( sv.find_first_of( ',' ) ) );
				  sv.remove_prefix( );
				  sv = daw::parser::trim_left( sv );
				  auto y = daw::parser::parse_unsigned_int<int16_t>(
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

		constexpr auto distance( point_t a, point_t b ) noexcept {
			if( b.x > a.x ) {
				std::swap( b.x, a.x );
			}
			if( b.y > a.y ) {
				std::swap( b.y, a.y );
			}
			auto x = a.x - b.x;
			auto y = a.y - b.y;
			return x + y;
		}

		template<size_t N>
		auto part_01( point_array_t<N> const &coords ) {
			constexpr int16_t const MAXX = 360;
			constexpr int16_t const MAXY = 360;
			grid_t<MAXX, MAXY> grid{};
			for( int16_t x = 0; x < MAXX; ++x ) {
				for( int16_t y = 0; y < MAXY; ++y ) {
					std::array<std::pair<size_t, int16_t>, N> distances{};
					auto const pt = point_t{x, y};
					for( size_t n = 0; n < coords.points.size( ); ++n ) {
						distances[n] =
						  std::pair<size_t, int16_t>( n, distance( pt, coords.points[n] ) );
					}
					std::sort(
					  begin( distances ), end( distances ),
					  []( auto lhs, auto rhs ) { return lhs.second < rhs.second; } );
					if( distances[0].second == distances[1].second ) {
						grid( x, y ) = grid.common_v;
					} else {
						grid( x, y ) = distances[0].first;
					}
				}
			}
			std::set<intmax_t> invalid_points{};
			// All perimiter points are for removal
			// do not count their areas here as it doesn't matter
			for( int16_t x = 0; x < MAXX; ++x ) {
				invalid_points.insert( grid( x, 0 ) );
				invalid_points.insert( grid( x, MAXY - 1 ) );
			}
			for( int16_t y = 1; y < MAXY; ++y ) {
				invalid_points.insert( grid( 0, y ) );
				invalid_points.insert( grid( MAXX - 1, y ) );
			}
			std::array<size_t, N> areas{};
			for( int16_t x = 1; x < ( MAXX - 1 ); ++x ) {
				for( int16_t y = 1; y < ( MAXY - 1 ); ++y ) {
					if( grid( x, y ) < grid.no_value_v ) {
						++areas[grid( x, y )];
					}
				}
			}
			int16_t max_point = [&]( ) {
				int16_t n = 0;
				// assume at least 1 is valid
				while( invalid_points.count( n ) > 0 ) {
					++n;
				}
				return n;
			}( );
			int16_t max_area = areas[max_point];
			for( auto n = max_point; n < N; ++n ) {
				if( invalid_points.count( n ) > 0 ) {
					continue;
				}
				if( areas[n] > max_area ) {
					max_point = n;
					max_area = areas[n];
				}
			}
			return max_area;
		}

		template<size_t MaxDistance, size_t N>
		auto part_02( point_array_t<N> const &coords ) {
			constexpr int16_t const MAXX = 1000;
			constexpr int16_t const MAXY = 1000;
			auto grid_ptr = std::make_unique<grid_t<(MAXX*2+1), (MAXY*2+1)>>( );
			auto & grid = *grid_ptr;
			grid.off_x = MAXX;
			grid.off_y = MAXY;
			for( int16_t x = -MAXX; x < MAXX; ++x ) {
				for (int16_t y = -MAXY; y < MAXY; ++y) {
					grid(x, y) = daw::algorithm::accumulate(begin(coords.points), end(coords.points), 0ULL,
																									[pt = point_t{x, y}](auto tot, auto pt2) {
																										tot += distance(pt, pt2);
																										return tot;
																									});
				}
			}
			return daw::algorithm::accumulate( begin( grid.m_values ), end( grid.m_values ), 0ULL, []( auto tot, auto i ) {
				if (i < MaxDistance) {
					return tot + 1;
				}
				return tot;
			} );
		}

	} // namespace
} // namespace daw
