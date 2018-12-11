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
#include <future>
#include <iostream>
#include <iterator>
#include <list>
#include <set>
#include <tuple>

#include <daw/daw_keep_n.h>
#include <daw/daw_parser_helper_sv.h>
#include <daw/daw_static_string.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>
#include <daw/daw_tuple_helper.h>
#include <daw/fs/algorithms.h>

namespace daw {
	namespace {
		using data_t = std::array<std::array<intmax_t, 301>, 301>;

		constexpr intmax_t find_power_level( intmax_t x, intmax_t y,
		                                     intmax_t sn ) noexcept {
			auto const rack_id = x + 10LL;
			auto power_level = rack_id * y;
			power_level += sn;
			power_level *= rack_id;
			power_level /= 100LL;
			power_level %= 10LL;
			power_level -= 5LL;
			return power_level;
		}
		static_assert( find_power_level( 122, 79, 57 ) == -5LL );
		static_assert( find_power_level( 217, 196, 39 ) == 0LL );
		static_assert( find_power_level( 3, 5, 8 ) == 4LL );

		struct max_data_t {
			intmax_t x = 0;
			intmax_t y = 0;
			intmax_t power_level = 0;
		};

		constexpr std::tuple<intmax_t, intmax_t, intmax_t>
		find_power_level_3x3( intmax_t x, intmax_t y, intmax_t sn ) {

			auto result0 = find_power_level( x, y, sn ) +
			               find_power_level( x + 1, y, sn ) +
			               find_power_level( x + 2, y, sn );
			auto result1 = find_power_level( x, y + 1, sn ) +
			               find_power_level( x + 1, y + 1, sn ) +
			               find_power_level( x + 2, y + 1, sn );
			auto result2 = find_power_level( x, y + 2, sn ) +
			               find_power_level( x + 1, y + 2, sn ) +
			               find_power_level( x + 2, y + 2, sn );
			return {result0, result1, result2};
		}

		template<typename... Integers>
		constexpr intmax_t sum_row( std::tuple<Integers...> const &vals ) noexcept {
			intmax_t result = 0;
			daw::tuple::for_each( vals, [&result]( auto v ) { result += v; } );
			return result;
		}

		struct max_power_t {
			intmax_t x = 0;
			intmax_t y = 0;
			intmax_t power_level = std::numeric_limits<intmax_t>::min( );
			size_t size = 0;
		};

		constexpr bool operator>( max_power_t const &lhs,
		                          max_power_t const &rhs ) noexcept {
			return lhs.power_level > rhs.power_level;
		}

		template<intmax_t Size>
		constexpr intmax_t find_power_level_row( intmax_t x, intmax_t y,
		                                         intmax_t sn ) noexcept {
			intmax_t result = 0;
			for( auto px = x; px < x + Size; ++px ) {
				result += find_power_level( px, y, sn );
			}
			return result;
		}
		constexpr intmax_t find_power_level_row( intmax_t x, intmax_t y,
		                                         size_t size,
		                                         data_t const &pow_lvl ) noexcept {
			intmax_t result = 0;
			for( auto px = x; px < x + static_cast<intmax_t>( size ); ++px ) {
				result += pow_lvl[static_cast<size_t>( y )][static_cast<size_t>( x )];
			}
			return result;
		}

		template<intmax_t Size>
		constexpr intmax_t find_power_level_NxN( intmax_t x, intmax_t y,
		                                         intmax_t sn ) noexcept {
			static_assert( Size > 0 );
			if( x + Size > 300LL ) {
				return 0LL;
			}
			if( y + Size > 300LL ) {
				return 0LL;
			}
			intmax_t result = 0;
			for( auto py = y; py <= y + Size; ++y ) {
				result += find_power_level_row<Size>( x, py, sn );
			}
			return result;
		}
		constexpr intmax_t find_power_level_NxN( intmax_t x, intmax_t y,
		                                         size_t size,
		                                         data_t const &pow_lvls ) noexcept {
			intmax_t result = 0;
			for( auto py = y; py <= y + static_cast<intmax_t>( size ); ++y ) {
				result += find_power_level_row( x, py, size, pow_lvls );
			}
			return result;
		}

		template<size_t N, size_t M,
		         std::enable_if_t<( N == M ), std::nullptr_t> = nullptr>
		constexpr max_power_t do_part_02( intmax_t, data_t const & ) noexcept {
			return max_power_t{};
		}

		constexpr auto get_all_powerlevels( intmax_t sn ) noexcept {
			std::pair<intmax_t, data_t> result( 0, {} );
			for( size_t y = 1; y <= 300; ++y ) {
				for( size_t x = 1; x <= 300; ++x ) {
					auto const tmp = find_power_level( static_cast<intmax_t>( x ),
					                                   static_cast<intmax_t>( y ), sn );
					result.first += tmp;
					result.second[y][x] = tmp;
				}
			}
			return result;
		}

		template<size_t N, size_t M,
		         std::enable_if_t<( N > M ), std::nullptr_t> = nullptr>
		constexpr max_power_t do_part_02( intmax_t,
		                                  data_t const &pow_lvls ) noexcept {
			max_power_t max_power{};
			max_power.size = N;
			for( intmax_t y = 1; y <= static_cast<intmax_t>( 300 - N ); ++y ) {
				for( intmax_t x = 1; x <= static_cast<intmax_t>( 300 - N ); ++x ) {
					auto tmp = find_power_level_NxN<N>( x, y, 0, pow_lvls );
					if( tmp > max_power.power_level ) {
						max_power.power_level = tmp;
						max_power.x = x;
						max_power.y = y;
					}
				}
			}
			auto tmp = do_part_02<N - 1, M>( 0, pow_lvls );
			if( tmp.power_level > max_power.power_level ) {
				return tmp;
			}
			return max_power;
		}

		constexpr intmax_t find_sum( size_t x, size_t y, size_t sz,
		                             data_t const &row_prefix_sum ) {
			// x-1 is always fine because the prefix rows start at 0, we don't use
			// column 0 or row 0
			auto const max_y = y + sz;
			intmax_t result = 0;
			for( size_t py = y; py < max_y; ++py ) {
				auto const left = row_prefix_sum[py][x - 1];
				auto const right = row_prefix_sum[py][x + sz - 1];
				result += right - left;
			}
			return result;
		}

		constexpr data_t
		build_row_prefix_sum_table( data_t const &power_levels ) noexcept {
			data_t row_prefix_sum{};
			for( size_t y = 1; y <= 300; ++y ) {
				daw::algorithm::partial_sum( power_levels[y].begin( ),
				                             power_levels[y].end( ),
				                             row_prefix_sum[y].begin( ) );
			}
			return row_prefix_sum;
		}

		max_power_t largest_subset_sum( intmax_t sn ) noexcept {
			max_power_t max_power{};
			auto const all_power_levels = get_all_powerlevels( sn );
			max_power.power_level = all_power_levels.first;
			max_power.x = 1;
			max_power.y = 1;
			max_power.size = 300;

			data_t row_prefix_sum =
			  build_row_prefix_sum_table( all_power_levels.second );

			for( size_t x = 1ULL; x < 300ULL; ++x ) {
				for( size_t y = 1ULL; y < 300ULL; ++y ) {
					auto const max_sz = 300ULL - std::max( x, y );
					for( size_t sz = 1ULL; sz < max_sz; ++sz ) {
						auto const tmp = find_sum( x, y, sz, row_prefix_sum );
						if( tmp > max_power.power_level ) {
							max_power.power_level = tmp;
							max_power.x = static_cast<intmax_t>( x );
							max_power.y = static_cast<intmax_t>( y );
							max_power.size = sz;
						}
					}
				}
			}
			return max_power;
		}

		max_power_t part_01( intmax_t sn ) noexcept {
			intmax_t max_power = std::numeric_limits<intmax_t>::min( );
			intmax_t max_x = 0;
			intmax_t max_y = 0;
			for( intmax_t y = 1; y <= 300; ++y ) {
				for( intmax_t x = 1; x <= 300; ++x ) {
					auto tmp = sum_row( find_power_level_3x3( x, y, sn ) );
					if( tmp > max_power ) {
						max_power = tmp;
						max_x = x;
						max_y = y;
					}
				}
			}
			std::cout << "max power: " << max_power << '\n';
			std::cout << "max_x: " << max_x << '\n';
			std::cout << "max_y: " << max_y << '\n';
			return {max_x, max_y};
		}

	} // namespace
} // namespace daw
