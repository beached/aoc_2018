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
		template<typename Integer, typename CharT>
		constexpr Integer get_unsigned( daw::basic_string_view<CharT> & sv ) noexcept {
			auto result = daw::parser::parse_unsigned_int<Integer>( sv.pop_front( sv.find_first_of( ' ' ) ) );
			sv.remove_prefix( );
			return result;
		}

		template<typename CharT>
		size_t part_01(daw::basic_string_view<CharT> &sv) {
			auto num_children = get_unsigned<uint8_t>( sv );
			auto num_meta = get_unsigned<uint8_t>( sv );
			size_t result = 0;
			for( size_t n=0; n<num_children; ++n ) {
				result += part_01(sv);
			}
			for( size_t n=0; n<num_meta; ++n ) {
				result += get_unsigned<size_t>( sv );
			}
			return result;
		}

		template<typename CharT>
		size_t part_02(daw::basic_string_view<CharT> &sv) {
			auto num_children = get_unsigned<uint8_t>( sv );
			auto num_meta = get_unsigned<uint8_t>( sv );
			std::vector<size_t> child_values{};
			child_values.reserve( num_children );
			for( size_t n=0; n<num_children; ++n ) {
				child_values.push_back( part_02( sv ) );
			}
			size_t result = 0;
			for( size_t n=0; n<num_meta; ++n ) {
				auto md = get_unsigned<size_t>(sv);
				if( num_children > 0 ) {
					if (md <= child_values.size()) {
						result += child_values[md-1];
					}
				} else {
					result += md;
				}
			}
			return result;
		}
	}
} // namespace daw
