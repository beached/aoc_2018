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
#include <optional>
#include <unordered_set>

#include <daw/daw_algorithm.h>
#include <daw/daw_hash_adaptor.h>
#include <daw/daw_fnv1a_hash.h>
#include <daw/daw_traits.h>

namespace daw {
	template<size_t N>
	constexpr intmax_t sum_values( std::array<intmax_t, N> values ) {
		return daw::algorithm::accumulate( values.begin( ), values.end( ),
		                                   static_cast<intmax_t>( 0 ) );
	}
	template<size_t N>
	intmax_t find_dups( std::array<intmax_t, N> const & values ) {
		std::unordered_set<intmax_t> sums{};
		intmax_t sum = 0;
		sums.insert(sum);
		while (true) {
			for (auto i : values) {
				sum += i;
				if (sums.count(sum) > 0) {
					return sum;
				}
				sums.insert(sum);
			}
		}
	}
	template<size_t N>
	intmax_t find_dups2( std::array<intmax_t, N> const & values ) {
		daw::static_hash_adaptor_t<intmax_t, N*200U> sums{};
		intmax_t sum = 0;
		sums.insert( sum );
		while( true ) {
			for( auto i : values ) {
				sum += i;
				if( sums.count( sum ) > 0 ) {
					return sum;
				}
				sums.insert( sum );
			}
		}
	}
	template<size_t N>
	constexpr intmax_t find_dups3( std::array<intmax_t, N> const & values ) {
		daw::static_hash_adaptor_t<intmax_t, N*200U, daw::fnv1a_hash_t> sums{};
		intmax_t sum = 0;
		sums.insert( sum );
		while( true ) {
			for( auto i : values ) {
				sum += i;
				if( sums.count( sum ) > 0 ) {
					return sum;
				}
				sums.insert( sum );
			}
		}
	}
} // namespace daw
