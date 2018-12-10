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

#include "day_10.h"

namespace daw {
	using namespace daw::string_view_literals;
	static constexpr auto const lights = daw::process_data(
	  std::array<daw::string_view, 31>{"position=< 9,  1> velocity=< 0,  2>",
	                                   "position=< 7,  0> velocity=<-1,  0>",
	                                   "position=< 3, -2> velocity=<-1,  1>",
	                                   "position=< 6, 10> velocity=<-2, -1>",
	                                   "position=< 2, -4> velocity=< 2,  2>",
	                                   "position=<-6, 10> velocity=< 2, -2>",
	                                   "position=< 1,  8> velocity=< 1, -1>",
	                                   "position=< 1,  7> velocity=< 1,  0>",
	                                   "position=<-3, 11> velocity=< 1, -2>",
	                                   "position=< 7,  6> velocity=<-1, -1>",
	                                   "position=<-2,  3> velocity=< 1,  0>",
	                                   "position=<-4,  3> velocity=< 2,  0>",
	                                   "position=<10, -3> velocity=<-1,  1>",
	                                   "position=< 5, 11> velocity=< 1, -2>",
	                                   "position=< 4,  7> velocity=< 0, -1>",
	                                   "position=< 8, -2> velocity=< 0,  1>",
	                                   "position=<15,  0> velocity=<-2,  0>",
	                                   "position=< 1,  6> velocity=< 1,  0>",
	                                   "position=< 8,  9> velocity=< 0, -1>",
	                                   "position=< 3,  3> velocity=<-1,  1>",
	                                   "position=< 0,  5> velocity=< 0, -1>",
	                                   "position=<-2,  2> velocity=< 2,  0>",
	                                   "position=< 5, -2> velocity=< 1,  2>",
	                                   "position=< 1,  4> velocity=< 2,  1>",
	                                   "position=<-2,  7> velocity=< 2, -2>",
	                                   "position=< 3,  6> velocity=<-1, -1>",
	                                   "position=< 5,  0> velocity=< 1,  0>",
	                                   "position=<-6,  0> velocity=< 2,  0>",
	                                   "position=< 5,  9> velocity=< 1, -2>",
	                                   "position=<14,  7> velocity=<-2,  0>",
	                                   "position=<-3,  6> velocity=< 2, -1>"} );

} // namespace daw

int main( ) {
	auto str = daw::part_01( daw::lights );
	/*
	auto const p1 = *daw::bench_n_test<1>(
	  "Day 10, part 1", [&]( ) { return daw::part_01( daw::lights ); } );
	daw::expecting( p1, p1 );

	auto const p2 = *daw::bench_n_test<1>(
	  "Day 10, part 2", [&]( ) { return daw::part_02( ); } );
	daw::expecting( p2, p2 );
	*/
}
