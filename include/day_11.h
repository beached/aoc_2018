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
		using value_t = int32_t;
		using data_t = std::array<std::array<value_t, 301>, 301>;

		constexpr value_t calculate_power_level( size_t x, size_t y,
		                                          value_t sn ) noexcept {
			value_t const _x = static_cast<value_t>( x );
			value_t const _y = static_cast<value_t>( y );
			auto const rack_id = _x + 10;
			auto power_level = rack_id * _y;
			power_level += sn;
			power_level *= rack_id;
			power_level /= 100;
			power_level %= 10;
			power_level -= 5;
			return power_level;
		}
		static_assert( calculate_power_level( 122, 79, 57 ) == -5 );
		static_assert( calculate_power_level( 217, 196, 39 ) == 0 );
		static_assert( calculate_power_level( 3, 5, 8 ) == 4 );

		struct max_power_t {
			value_t x = 0;
			value_t y = 0;
			value_t power_level = std::numeric_limits<value_t>::min( );
			size_t size = 0;
		};

		constexpr bool operator==( max_power_t const &lhs,
		                           max_power_t const &rhs ) noexcept {
			return std::tie( rhs.x, rhs.y, rhs.power_level, rhs.size ) ==
			       std::tie( rhs.x, rhs.y, rhs.power_level, rhs.size );
		}

		constexpr data_t build_summed_area_table( value_t sn ) noexcept {
			data_t result{};
			for( size_t y = 1U; y <= 300U; ++y ) {
				for( size_t x = 1U; x <= 300U; ++x ) {
					auto const power_level = calculate_power_level( x, y, sn );
					auto const RYXm1 = result[y][x - 1];
					auto const RYm1X = result[y - 1][x];
					auto const RYm1Xm1 = result[y - 1][x - 1];
					result[y][x] = power_level + RYXm1 + RYm1X - RYm1Xm1;
				}
			}
			return result;
		}

		constexpr value_t find_sum( size_t x, size_t y, size_t sz,
		                             data_t const &summed_area_table ) noexcept {
			/*
			A # B #
			# @ @ #
			D @ C #
			# # # #
			 Need Sum = C + A - (B + D)
			*/
			auto const max_x = x + sz - 1;
			auto const max_y = y + sz - 1;
			auto const A = summed_area_table[y - 1][x - 1];
			auto const B = summed_area_table[y - 1][max_x];
			auto const C = summed_area_table[max_y][max_x];
			auto const D = summed_area_table[max_y][x - 1];
			return ( A + C ) - ( B + D );
		}

		template<size_t MinSize = 1U, size_t MaxSize = 300U>
		constexpr max_power_t largest_subset_sum( value_t sn ) noexcept {
			max_power_t max_power{};
			max_power.power_level = calculate_power_level( 1, 1, sn );
			max_power.x = 1;
			max_power.y = 1;
			max_power.size = 1;

			data_t const summed_area_table = build_summed_area_table( sn );
			for( size_t sz = MinSize; sz <= MaxSize; ++sz ) {
				auto const max_idx = 300U - sz;
				for( size_t y = 1U; y <= max_idx; ++y ) {
					for( size_t x = 1U; x <= max_idx; ++x ) {
						auto const tmp =
						  find_sum( x, y, sz, summed_area_table ); // row_prefix_sum );
						if( tmp > max_power.power_level ) {
							max_power.power_level = tmp;
							max_power.x = static_cast<value_t>( x );
							max_power.y = static_cast<value_t>( y );
							max_power.size = sz;
						}
					}
				}
			}
			return max_power;
		}

		constexpr max_power_t part_01( value_t sn ) noexcept {
			return largest_subset_sum<3, 3>( sn );
		}
		// Cannot enable, too many steps
		// static_assert( part_01( 9302 ) == max_power_t{ 235U, 38U, 30, 3U } );
	} // namespace
} // namespace daw
