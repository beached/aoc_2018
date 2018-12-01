// The MIT License (MIT)
//
// Copyright (c) 2018 Darrell Wright
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
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
#include <unordered_map>
#include <vector>

#include <daw/daw_algorithm.h>

namespace daw {
	constexpr intmax_t sum_values( std::initializer_list<intmax_t> values ) {
		return daw::algorithm::accumulate( values.begin( ), values.end( ), static_cast<intmax_t>( 0 ) );
	}

	template<typename Container>
	intmax_t find_dups( Container && values ) {
		std::unordered_map<intmax_t, size_t> sum_count{};
		intmax_t sum = 0;
		sum_count[sum] = 1;
		while( true ) {
			for (auto i: values) {
				sum += i;
				if (sum_count[sum] > 0) {
					return sum;
				}
				++sum_count[sum];
			}
		}
		throw std::exception{};
	}
}


