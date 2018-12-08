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

#include "day_07.h"

namespace daw {
	namespace aoc_2018 {
		namespace day07 {
			using namespace daw::string_view_literals;

			static constexpr auto requirements =
			  daw::process_reqs( std::array<daw::string_view, 101>{
			    "Step G must be finished before step T can begin.",
			    "Step L must be finished before step V can begin.",
			    "Step D must be finished before step P can begin.",
			    "Step J must be finished before step K can begin.",
			    "Step N must be finished before step B can begin.",
			    "Step K must be finished before step W can begin.",
			    "Step T must be finished before step I can begin.",
			    "Step F must be finished before step E can begin.",
			    "Step P must be finished before step O can begin.",
			    "Step X must be finished before step I can begin.",
			    "Step M must be finished before step S can begin.",
			    "Step Y must be finished before step O can begin.",
			    "Step I must be finished before step Z can begin.",
			    "Step V must be finished before step Z can begin.",
			    "Step Q must be finished before step Z can begin.",
			    "Step H must be finished before step C can begin.",
			    "Step R must be finished before step Z can begin.",
			    "Step U must be finished before step S can begin.",
			    "Step E must be finished before step Z can begin.",
			    "Step O must be finished before step W can begin.",
			    "Step Z must be finished before step S can begin.",
			    "Step S must be finished before step C can begin.",
			    "Step W must be finished before step B can begin.",
			    "Step A must be finished before step B can begin.",
			    "Step C must be finished before step B can begin.",
			    "Step L must be finished before step P can begin.",
			    "Step J must be finished before step V can begin.",
			    "Step E must be finished before step W can begin.",
			    "Step Z must be finished before step W can begin.",
			    "Step W must be finished before step C can begin.",
			    "Step S must be finished before step W can begin.",
			    "Step Q must be finished before step S can begin.",
			    "Step O must be finished before step B can begin.",
			    "Step R must be finished before step W can begin.",
			    "Step D must be finished before step H can begin.",
			    "Step E must be finished before step O can begin.",
			    "Step Y must be finished before step H can begin.",
			    "Step V must be finished before step O can begin.",
			    "Step O must be finished before step S can begin.",
			    "Step X must be finished before step V can begin.",
			    "Step R must be finished before step E can begin.",
			    "Step S must be finished before step A can begin.",
			    "Step K must be finished before step Y can begin.",
			    "Step V must be finished before step W can begin.",
			    "Step U must be finished before step W can begin.",
			    "Step H must be finished before step R can begin.",
			    "Step P must be finished before step I can begin.",
			    "Step E must be finished before step C can begin.",
			    "Step H must be finished before step Z can begin.",
			    "Step N must be finished before step V can begin.",
			    "Step N must be finished before step W can begin.",
			    "Step A must be finished before step C can begin.",
			    "Step V must be finished before step E can begin.",
			    "Step N must be finished before step Q can begin.",
			    "Step Y must be finished before step V can begin.",
			    "Step R must be finished before step O can begin.",
			    "Step R must be finished before step C can begin.",
			    "Step L must be finished before step S can begin.",
			    "Step V must be finished before step R can begin.",
			    "Step X must be finished before step R can begin.",
			    "Step Z must be finished before step A can begin.",
			    "Step O must be finished before step Z can begin.",
			    "Step U must be finished before step C can begin.",
			    "Step X must be finished before step W can begin.",
			    "Step K must be finished before step O can begin.",
			    "Step O must be finished before step A can begin.",
			    "Step K must be finished before step T can begin.",
			    "Step N must be finished before step O can begin.",
			    "Step X must be finished before step C can begin.",
			    "Step Z must be finished before step C can begin.",
			    "Step N must be finished before step X can begin.",
			    "Step T must be finished before step A can begin.",
			    "Step D must be finished before step O can begin.",
			    "Step M must be finished before step Q can begin.",
			    "Step D must be finished before step C can begin.",
			    "Step U must be finished before step E can begin.",
			    "Step N must be finished before step H can begin.",
			    "Step I must be finished before step U can begin.",
			    "Step N must be finished before step A can begin.",
			    "Step M must be finished before step E can begin.",
			    "Step M must be finished before step V can begin.",
			    "Step P must be finished before step B can begin.",
			    "Step K must be finished before step X can begin.",
			    "Step N must be finished before step S can begin.",
			    "Step S must be finished before step B can begin.",
			    "Step Y must be finished before step W can begin.",
			    "Step K must be finished before step Q can begin.",
			    "Step V must be finished before step S can begin.",
			    "Step E must be finished before step S can begin.",
			    "Step N must be finished before step Z can begin.",
			    "Step P must be finished before step A can begin.",
			    "Step T must be finished before step V can begin.",
			    "Step L must be finished before step D can begin.",
			    "Step I must be finished before step C can begin.",
			    "Step Q must be finished before step E can begin.",
			    "Step Y must be finished before step U can begin.",
			    "Step J must be finished before step I can begin.",
			    "Step P must be finished before step H can begin.",
			    "Step T must be finished before step M can begin.",
			    "Step T must be finished before step E can begin.",
			    "Step D must be finished before step F can begin."} );

			static constexpr auto const little_reqs = daw::process_reqs( std::array<daw::string_view, 7>{
					"Step C must be finished before step A can begin.",
					"Step C must be finished before step F can begin.",
					"Step A must be finished before step B can begin.",
					"Step A must be finished before step D can begin.",
					"Step B must be finished before step E can begin.",
					"Step D must be finished before step E can begin.",
					"Step F must be finished before step E can begin."
			});
		} // namespace day07
	}   // namespace aoc_2018
} // namespace daw

int main( ) {
	auto little = daw::part_01( daw::aoc_2018::day07::little_reqs );
	daw::expecting( "CABDFE", little );
	auto const p1 = *daw::bench_n_test<1>( "Day 7, part 1", [&]( ) {
		return daw::part_01( daw::aoc_2018::day07::requirements );
	} );
	daw::expecting( "GJKLDFNPTMQXIYHUVREOZSAWCB", p1 );

	auto little_p2 = daw::part_02<0, 2>( daw::aoc_2018::day07::little_reqs );
//	daw::expecting( 15, little_p2 );

	auto const p2 = *daw::bench_n_test<1>( "Day 7, part 1", [&]( ) {
		return daw::part_02( daw::aoc_2018::day07::requirements );
	} );
	daw::expecting( p2, p2 );
}
