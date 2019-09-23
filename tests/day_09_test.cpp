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

#include "day_09.h"

namespace daw {
	using namespace daw::string_view_literals;
	static auto const init_state =
	  std::array<daw::string_view, 28> {"AlphaCentauri to Snowdin = 66",
	                                      "AlphaCentauri to Tambi = 28",
	                                      "AlphaCentauri to Faerun = 60",
	                                      "AlphaCentauri to Norrath = 34",
	                                      "AlphaCentauri to Straylight = 34",
	                                      "AlphaCentauri to Tristram = 3",
	                                      "AlphaCentauri to Arbre = 108",
	                                      "Snowdin to Tambi = 22",
	                                      "Snowdin to Faerun = 12",
	                                      "Snowdin to Norrath = 91",
	                                      "Snowdin to Straylight = 121",
	                                      "Snowdin to Tristram = 111",
	                                      "Snowdin to Arbre = 71",
	                                      "Tambi to Faerun = 39",
	                                      "Tambi to Norrath = 113",
	                                      "Tambi to Straylight = 130",
	                                      "Tambi to Tristram = 35",
	                                      "Tambi to Arbre = 40",
	                                      "Faerun to Norrath = 63",
	                                      "Faerun to Straylight = 21",
	                                      "Faerun to Tristram = 57",
	                                      "Faerun to Arbre = 83",
	                                      "Norrath to Straylight = 9",
	                                      "Norrath to Tristram = 50",
	                                      "Norrath to Arbre = 60",
	                                      "Straylight to Tristram = 27",
	                                      "Straylight to Arbre = 81",
	                                      "Tristram to Arbre = 90" };
} // namespace daw

int main( ) {
	auto const p1 = *daw::bench_n_test<1>(
	  "Day 09, p1",
	  []( auto const &value ) {
	  	return daw::part_01( value );
	  },
	  daw::init_state );
	std::cout << "Part 1: " << p1 << '\n';
	daw::expecting( p1, p1 );
}
