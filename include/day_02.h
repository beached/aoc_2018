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
#include <unordered_map>

#include <daw/daw_algorithm.h>
#include <daw/daw_static_string.h>
#include <daw/daw_traits.h>

namespace daw {
	template<typename Iterator>
	constexpr size_t count_len( Iterator first, Iterator last ) {
		size_t result = 1;
		auto pos = first++;
		while( pos != last && *pos == *first ) {
			++result;
		}
		return result;
	}

	template<typename Container>
	constexpr size_t get_checksum( Container &&ids ) {
		size_t two_of_same = 0;
		size_t three_of_same = 0;
		for( auto id : ids ) {
			std::array<uint8_t, 26> counts{};
			for( auto c : id ) {
				++counts[static_cast<uint8_t>( c - 'a' )];
			}
			bool has_two = false;
			bool has_three = false;

			for( auto const &c : counts ) {
				switch( c ) {
				case 2:
					has_two = true;
					break;
				case 3:
					has_three = true;
					break;
				}
			}
			if( has_two ) {
				++two_of_same;
			}
			if( has_three ) {
				++three_of_same;
			}
		}
		return two_of_same * three_of_same;
	}

	template<size_t N>
	auto get_match( std::array<daw::string_view, N> const & ids ) {
		std::string result{};
//		daw::static_string result{};
		for( size_t n = 0; n < ids.size( ); ++n ) {
			for( size_t m = n + 1; m < ids.size( ); ++m ) {
				uint_fast8_t sum = 0;
				result = "";
				for( size_t pos = 0; pos < ids[n].size( ); ++pos ) {
					if( ids[n][pos] != ids[m][pos] ) {
						++sum;
						if( sum > 1 ) {
							break;
						}
						result = ids[n];
						result.erase( pos, 1 );
					}
				}
				if( sum == 1 ) {
					return result;
				}
			}
		}
		std::terminate( );
	}

	template<typename String>
	constexpr intmax_t sum_string( String &&str ) {
		return daw::algorithm::accumulate(
		  begin( str ), end( str ), static_cast<intmax_t>( 0 ),
		  []( auto init, auto c ) { return init + ( c - 'a' + 1 ); } );
	}

	template<typename Container>
	auto get_container_info( Container &&c ) {
		struct info_t {
			size_t index;
			intmax_t sum;
		};
		std::vector<info_t> value_info{};
		value_info.reserve( std::size( c ) );
		size_t idx = 0;
		std::transform(
		  begin( c ), end( c ), std::back_inserter( value_info ),
		  [&idx]( auto &&v ) {
			  return info_t{idx++, sum_string( std::forward<decltype( v )>( v ) )};
		  } );
		std::sort( begin( value_info ), end( value_info ),
		           []( auto &&lhs, auto &&rhs ) { return lhs.sum < rhs.sum; } );
		return value_info;
	}
} // namespace daw
