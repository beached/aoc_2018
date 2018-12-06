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
		constexpr char toupper( char c ) noexcept {
			c &= ~( 'a' - 'A' );
			return c;
		}

		template<typename CharT>
		constexpr bool match( CharT lhs, CharT rhs ) noexcept {
			return lhs != rhs and toupper( lhs ) == toupper( rhs );
		}

		template<size_t BuffSize, typename CharT>
		size_t
		alchemical_reduction( daw::basic_string_view<CharT> sv ) noexcept {
			daw::basic_static_string<CharT, BuffSize> result{};

			CharT last_c = sv.front( );
			auto const matcher = [&last_c]( auto c ) constexpr {
				auto r = match( last_c, c );
				last_c = c;
				return r;
			};
			auto pos = sv.find_first_of_if( matcher, 1 );
			while( pos != sv.npos ) {
				result.append( sv.pop_front( pos - 1 ) );
				sv.remove_prefix( 2 );
				while( !result.empty( ) and !sv.empty( ) and
				       match( result.back( ), sv.front( ) ) ) {
					result.pop_back( );
					sv.pop_front( );
				}
				if( !sv.empty( ) ) {
					last_c = sv.front( );
					pos = sv.find_first_of_if( matcher, 1 );
				} else {
					pos = sv.npos;
				}
			}
			while( !result.empty( ) and !sv.empty( ) and
			       match( result.back( ), sv.front( ) ) ) {
				result.pop_back( );
				sv.pop_front( );
			}
			result.append( sv );
			return result.size( );
		}

		template<size_t BuffSize, typename CharT>
		size_t smallest( daw::basic_string_view<CharT> sv ) {
			// Find all unit types
			constexpr auto const unit_types = []( ) {
				std::array<char, 26> result{};
				daw::algorithm::iota( begin( result ), end( result ), 'A' );
				return result;
			}( );
			std::array<size_t, 26> results{};
			for( auto &i : results ) {
				i = std::numeric_limits<size_t>::max( );
			}

			for( size_t n = 0; n < 26; ++n ) {
				daw::basic_static_string<CharT, BuffSize> poly{};
				daw::algorithm::copy_if(
				  begin( sv ), end( sv ), daw::back_inserter( poly ),
				  [&]( auto c ) { return toupper( c ) != unit_types[n]; } );

				results[n] = alchemical_reduction<BuffSize>(
				  daw::string_view( poly.data( ), poly.size( ) ) );
			}

			return *std::min_element( begin( results ), end( results ) );
		}
	} // namespace
} // namespace daw
