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
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_set>

#include <daw/daw_algorithm.h>
#include <daw/daw_parser_helper_sv.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

namespace daw {
	namespace {
		template<typename CharT>
		bool match( CharT lhs, CharT rhs ) noexcept {
			return lhs != rhs and std::toupper( lhs ) == std::toupper( rhs );
		}

		template<typename CharT>
		size_t alchemical_reduction( daw::basic_string_view<CharT> sv ) noexcept {
			bool changed = true;
			std::basic_string<CharT> result{};
			std::basic_string<CharT> tmp{};

			while( changed ) {
				changed = false;
				tmp.reserve( sv.size( ) );

				CharT last_c = sv.front( );
				auto const matcher = [&last_c]( auto c ) {
					auto r = match( last_c, c );
					last_c = c;
					return r;
				};
				auto pos = sv.find_first_of_if( matcher, 1 );
				while( pos != sv.npos ) {
					 changed = true;
					tmp += sv.pop_front( pos - 1 ).to_string( );
					sv.remove_prefix( 2 );
					while( !tmp.empty( ) and !sv.empty( ) and match( tmp.back( ), sv.front( ) ) ) {
						tmp.pop_back( );
						sv.pop_front( );
					}
					if( !sv.empty( ) ) {
						last_c = sv.front();
						pos = sv.find_first_of_if(matcher, 1);
					} else {
						pos = sv.npos;
					}
				}
				if( changed ) {
					if (!sv.empty()) {
						tmp += sv.to_string();
					}
					result = std::move(tmp);
					sv = basic_string_view<CharT>(result.data(), result.size());
					tmp.clear();
				}
			}
			if( result.empty( ) ) {
				return sv.size( );
			}
			return result.size( );
		}

		template<typename CharT>
		size_t smallest( daw::basic_string_view<CharT> sv ) {
			// Find all unit types
			constexpr auto const unit_types = []( ) constexpr {
				std::array<char, 26> result{};
				for( char n = 'A'; n <= 'Z'; ++n ) {
					result[static_cast<size_t>( n-'A' )] = n;
				}
				return result;
			}( );

			return std::accumulate( begin( unit_types ), end( unit_types ), std::numeric_limits<size_t>::max( ), [sv]( auto cur_min, auto u ) {
				auto poly = sv.to_string();
				poly.erase( std::remove_if( begin( poly ), end( poly ), [u]( auto c ) {
					return std::toupper( c ) == u;
				}), end( poly ) );
				auto tmp = alchemical_reduction( daw::string_view( poly.data( ), poly.size( ) ) );
				if( tmp < cur_min ) {
					return tmp;
				}
				return cur_min;
			} );
		}
	} // namespace
} // namespace daw
