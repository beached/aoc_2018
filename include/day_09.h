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
#include <set>

#include <daw/daw_algorithm.h>
#include <daw/daw_keep_n.h>
#include <daw/daw_parser_helper_sv.h>
#include <daw/daw_static_string.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

namespace daw {
	namespace {
		template<size_t PlayerCount, size_t MarbleCount>
		size_t day_09_solver() {
			std::list<size_t> board{};
			auto board_pos = board.begin( );
			size_t current_elf = 0;
			std::array<size_t, PlayerCount> elf_scores{};

			auto get_marble = [cur_marble=0ULL]( ) mutable {
				return cur_marble++;
			};

			auto const next = [&]( auto it, size_t count ) {
				while( count-- > 0 ) {
					++it;
					if( it == std::end( board ) ) {
						it = std::begin( board );
					}
				}
				return it;
			};

			auto const prev = [&]( auto it, size_t count ) {
				while( count-- > 0 ) {
					if( it == std::begin( board ) ) {
						it = std::end( board );
					}
					--it;
				}
				return it;
			};

			auto const place_marble = [&]( size_t m ) {
				board_pos = board.insert( next( board_pos, 2 ), m );
			};

			board.push_back( get_marble( ) );

			auto const mod_23 = [&]( auto it, size_t m ) {
				it = prev( it, 7 );
				elf_scores[current_elf] += m + *it;
				it = board.erase( it );
				return it;
			};

			auto m = get_marble( );
			while( m <= MarbleCount ) {
				if( m % 23 == 0 ) {
					current_elf %= PlayerCount;
					board_pos = mod_23( board_pos, m );
				} else {
					place_marble( m );
				}
				++current_elf;
				m = get_marble( );
			}
			return *std::max_element( begin( elf_scores ), end( elf_scores ) );
		}

		template<typename CharT>
		auto part_02( daw::basic_string_view<CharT> sv ) {
			return 0;
		}

	} // namespace
} // namespace daw
