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

#include "day_06.h"

namespace daw {
	namespace aoc_2018 {
		namespace day06 {
			using namespace daw::string_view_literals;

			/*static constexpr auto const grid_pts_sm =
			  parse_grid( std::array<daw::string_view, 6>{"1, 1", "1, 6", "8, 3",
			                                              "3, 4", "5, 5", "8, 9"} );
			                                              */
			static constexpr auto const grid_pts =
			  parse_grid( std::array<daw::string_view, 50>{
			    "152, 292", "163, 90",  "258, 65",  "123, 147", "342, 42",
			    "325, 185", "69, 45",   "249, 336", "92, 134",  "230, 241",
			    "74, 262",  "241, 78",  "299, 58",  "231, 146", "239, 87",
			    "44, 157",  "156, 340", "227, 226", "212, 318", "194, 135",
			    "235, 146", "171, 197", "160, 59",  "218, 205", "323, 102",
			    "290, 356", "244, 214", "174, 250", "70, 331",  "288, 80",
			    "268, 128", "359, 98",  "78, 249",  "221, 48",  "321, 228",
			    "52, 225",  "151, 302", "183, 150", "142, 327", "172, 56",
			    "72, 321",  "225, 298", "265, 300", "86, 288",  "78, 120",
			    "146, 345", "268, 181", "243, 235", "262, 268", "40, 60"} );
		} // namespace day06
	}   // namespace aoc_2018
} // namespace daw

int main( ) {
	auto const p1 = *daw::bench_n_test<100>(
	  "Day 6, part 1", []( auto const &pts ) { return daw::part_01( pts ); },
	  daw::aoc_2018::day06::grid_pts );
	daw::expecting( 3687U, p1 );
	auto const p2 = *daw::bench_n_test<100>(
	  "Day 6, part2",
	  []( auto const &pts ) { return daw::part_02<10'000>( pts ); },
	  daw::aoc_2018::day06::grid_pts );
	daw::expecting( 40134U, p2 );
}
