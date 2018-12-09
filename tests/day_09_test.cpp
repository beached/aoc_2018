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

int main( ) {
	auto test_001 = daw::day_09_solver<9U, 25U>( );
	daw::expecting( 32U, test_001 );
	auto test_002 = daw::day_09_solver<10U, 1618U>( );
	daw::expecting( 8317U, test_002 );
	auto test_003 = daw::day_09_solver<13U, 7999U>( );
	daw::expecting( 146373U, test_003 );

	auto const p1 = *daw::bench_n_test<10>(
	  "Day 9, part 1", [&]( ) { return daw::day_09_solver<459U, 72'103U>( ); } );
	daw::expecting( 388131U, p1 );

	auto const p2 = *daw::bench_n_test<10>( "Day 9, part 2", [&]( ) {
		return daw::day_09_solver<459U, 7'210'300U>( );
	} );
	daw::expecting( 3239376988ULL, p2 );
}
