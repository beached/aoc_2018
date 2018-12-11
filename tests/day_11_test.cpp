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

#include "day_11.h"

namespace daw {
	using namespace daw::string_view_literals;

} // namespace daw

int main( ) {
	intmax_t p1_sn = 9306;

	auto const p1 =
	  *daw::bench_n_test<1>( "Day 11, p1", []( auto const & value ) { return daw::part_01( value ); }, p1_sn );

	daw::expecting( p1.first == 235 and p1.second == 38 );

	auto const p2 =
			*daw::bench_n_test<1>( "Day 11, p2", []( auto const & value ) { return daw::part_02( value ); }, p1_sn );

			std::cout << "max power: " << p2.power_level << '\n';
			std::cout << "max_x: " << p2.x << '\n';
			std::cout << "max_y: " << p2.y << '\n';
			std::cout << "size: " << p2.size << '\n';
	daw::expecting( p2.x == p2.x );
}
