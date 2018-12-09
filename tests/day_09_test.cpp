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

#include <daw/daw_benchmark.h>
#include <daw/daw_string_view.h>

#include "day_08.h"

namespace daw {
	namespace aoc_2018 {
		namespace day09 {
			using namespace daw::string_view_literals;

		} // namespace day09
	}   // namespace aoc_2018
} // namespace daw

int main( ) {
	auto const p1 = *daw::bench_n_test<1>( "Day 9, part 1", [&]( ) {
		return 0;//daw::part_01( daw::aoc_2018::day09::data );

	} );
	daw::expecting( p1, p1 );

	auto const p2 = *daw::bench_n_test<1>( "Day 9, part 2", [&]( ) {
		return 0;//daw::part_02( daw::aoc_2018::day09::data );
	} );
	daw::expecting( p2, p2 );

}
