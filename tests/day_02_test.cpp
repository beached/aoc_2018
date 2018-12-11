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

#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <daw/daw_benchmark.h>
#include <daw/daw_string_view.h>

#include "day_02.h"

namespace daw {
	namespace aoc_2018 {
		namespace day02 {
			using namespace std::string_literals;
			static constexpr std::array<daw::string_view, 250> const box_ids = {
			  "oiwcdpbseqgxryfmlpktnupvza", "oiwddpbsuqhxryfmlgkznujvza",
			  "ziwcdpbsechxrvfmlgktnujvza", "oiwcgpbseqhxryfmmgktnhjvza",
			  "owwcdpbseqhxryfmlgktnqjvze", "oiscdkbseqhxrffmlgktnujvza",
			  "oiwcdibseqoxrnfmlgktnujvza", "oiwcdpbsejhxryfmlektnujiza",
			  "oewcdpbsephxryfmlgkwnujvza", "riwcdpbseqhxryfmlgktnujzaa",
			  "omwcdpbseqwxryfmlgktnujvqa", "oiwcdqqneqhxryfmlgktnujvza",
			  "oawcdvaseqhxryfmlgktnujvza", "oiwcdabseqhxcyfmlkktnujvza",
			  "oiwcdpbseqhxryfmlrktrufvza", "oiwcdpbseqhxdyfmlgqtnujkza",
			  "oiwcdpbseqhxrmfolgktnujvzy", "oiwcdpeseqhxnyfmlgktnejvza",
			  "oiwcdpbseqhxrynmlaktlujvza", "oiwcdpbseqixryfmlektncjvza",
			  "liwtdpbseqhxryfylgktnujvza", "ouwcdpbszqhxryfmlgktnajvza",
			  "oiwzdpbseqhxryfmngktnujvga", "wiwcfpbseqhxryfmlgktnuhvza",
			  "oiwcdpbselhfryfmlrktnujvza", "oywcdpbveqhxryfmlgktnujdza",
			  "oiwcdpbsiqhxryfmqiktnujvza", "obwcdhbseqhxryfmlgktnujvqa",
			  "oitcdpbseqhfryfmlyktnujvza", "oiwcdpbseqhxryfmlnutnujqza",
			  "oiwcdpbseqhxnyfmlhktnujtza", "oiwcdpbseqhbryfmlgkunuwvza",
			  "oiwcopbseqhiryfmlgktnkjvza", "oiwcdpsseqhxryfklrktnujvza",
			  "oiwcdpsrsqhxryfmlgktnujvza", "oiwcdpbsyqrxryfmlgktnujvzc",
			  "ojwcepbseqhxryfmlgktnujvfa", "oiwcdpbseqhxrlfmlgvtnujvzr",
			  "oiycdpbsethsryfmlgktnujvza", "eiwcdpbseqwxryfmlgktnujcza",
			  "oiocdpbseqhxryfxlgktaujvza", "qiwydpbseqhpryfmlgktnujvza",
			  "ziwcdpbseqhxryfmlgktsuuvza", "oiwcdpbseqheryfmygxtnujvza",
			  "oiwidpbseqhxryfulgktnujvzm", "oiscdpdseqhxryfmlgktnujvya",
			  "oiwmypbseqhxryfmlgktnuxvza", "oizcwbbseqhxryfmlgktnujvza",
			  "oiwcdpbseqpxryfmlgxfnujvza", "oiwpdpbscqhxryxmlgktnujvza",
			  "oiwcdpbseqhxrifrlgkxnujvza", "oiwcdpbsrqhxrifmlgktnzjvza",
			  "tiwcdpbseqhxryfmegkvjujvza", "oiwcddbseqhxryfingktnujvza",
			  "oiwcdpbsiqhiryfmlgktnucvza", "oiwcipbseqhxrkfmlgktnuvvza",
			  "kiwcdpbseqhxryfmlbkonujvza", "qiwcdhbsedhxryfmlgktnujvza",
			  "siwcdpbseqhxryfmsgktnujvua", "oqwcdpbseqhxryfmlyktndjvza",
			  "oiwcqnbseehxryfmlgktnujvza", "oiwcdybseqhxryfmlgbtnujvia",
			  "oiwcdpbsejhxryfdlgktngjvza", "oawcdpbseqhxrbfmlkktnujvza",
			  "oilcdpbseqhhrjfmlgktnujvza", "oibcdpbseqhxryfmngktnucvza",
			  "niwcdpbseqhxryfmlgkuaujvza", "oiwcdpbseqhxryfmqgmtnujvha",
			  "oiwcdpbseqhcryfxlgktnzjvza", "oiwcdpaseqhxryfmqgktnujvzl",
			  "oiwcdpbseqhxjyfmlgkonujvzx", "oiwmdzbseqlxryfmlgktnujvza",
			  "oiwhdpbsexhxryfmlgktnujvzw", "oiwctpbseqhxryfmlgktnummza",
			  "oiwcdpbseqoxrydmlgktnujvoa", "oiwkdpvseqhxeyfmlgktnujvza",
			  "oixcdpbsemhxryfmlgctnujvza", "oimcdpbseqhxryfmlgktnujvlr",
			  "oiwcdpbseehxrywmlgktnejvza", "oiwcdpbseqoxhyfmlgktnujyza",
			  "oiwcdpbsethxryfmlgktnrjvxa", "oiwcdpbxeqhxryfmlgktnrjvzb",
			  "ogeadpbseqhxryfmlgktnujvza", "eiwcdpbseqhxryfmlgktnvuvza",
			  "oiwcdpbseqhxryfmlgktnujaxv", "siwcdpbsuqhxryfmlgktnuavza",
			  "oixcdpbseqhxryfmlgatnujvzy", "oiwcdpbzeghmryfmlgktnujvza",
			  "oiwcdpbieqhxryfmlgktyujvzr", "oiwcdpbseqhxeyfhlgktngjvza",
			  "oiwcdpbseqhjoyrmlgktnujvza", "iiwcdpbseqhxryfmwhktnujvza",
			  "oixcdpbsiqhxryfmagktnujvza", "oiwcdpfljqhxryfmlgktnujvza",
			  "oivcdpbseqhxrqfmlgktnujvca", "ovwcdpbseqhxzyfmlgkenujvza",
			  "oiwxdpgseqhxryfmlgktnhjvza", "oibcdpbbeohxryfmlgktnujvza",
			  "oiwcrpbseqhxrygmlgwtnujvza", "jiwcdpbseqkxryfmlgntnujvza",
			  "oiwcdbbseqhxrywmlgktnujvra", "oiwcdpbteqyxoyfmlgktnujvza",
			  "oiwcdjbsvqvxryfmlgktnujvza", "obwcdukseqhxryfmlgktnujvza",
			  "oifcdpdseqhxryfmlgktnujsza", "oiwcdpbseqhxryfalgktnujyda",
			  "oiwcwpbseqhxrnfmkgktnujvza", "oswcdpbsethcryfmlgktnujvza",
			  "oiwcdpbieqhxryfmlgktnuoiza", "oiwcdibsehhxryfmzgktnujvza",
			  "oisjdpbseqhxryfmfgktnujvza", "oiwcjpbseqkxqyfmlgktnujvza",
			  "obwcdpbshqhgryfmlgktnujvza", "oiwcspbseqhxryxmlgktnujvzl",
			  "oiwcdvbswqhxryfmlgklnujvza", "oiwcdhuseqhxryfmlgdtnujvza",
			  "oiwcdpbkeqdxryfmlgktnujvzv", "oiwcdpzseqhxcyfmlgksnujvza",
			  "oiwcdpbseqhxryfmbkkvnujvza", "qiwcdpbseqhxrnfmlgktnujvha",
			  "okwcdpbseqhxryfmdgktgujvza", "oiwcdpbkeqhxryfmldktnujvzu",
			  "oiwctpxseqhxgyfmlgktnujvza", "oiwcdpbseqhxrykmlgktnujita",
			  "oiwcdpbseqhxryfmldktyujnza", "oiwcdpbszqhxrjfmlgktnuqvza",
			  "oiwcdpbeeqhxrykmlgktnujrza", "oiwcvpbseqhxryhmlgwtnujvza",
			  "oiwcdpbpeehxryfmlgktnujvzz", "oiwcdbbsxqhxyyfmlgktnujvza",
			  "oiwkdpbseqhxryfplgktnujeza", "opwcdpbseqhxdyfmlgctnujvza",
			  "oowcdpbseqhnryfmlgktnujvga", "oawzdibseqhxryfmlgktnujvza",
			  "oiwcdpbfeqzxrjfmlgktnujvza", "oiwcdpbseqaxryfmlgkonulvza",
			  "oiacdpbseqvxryfmlgktvujvza", "oiwudpbseqhxryfwlgktnujvka",
			  "oiwcdpbjeqhxryfymgktnujvza", "oiwcdpbweqhxrynmlgktnujaza",
			  "oiwcdpbseqdxryfclgvtnujvza", "oiwcdppseqhxryfmlgmtzujvza",
			  "oiwcdpbseqhxrhfelektnujvza", "kiwcdpbsnqhxryfmegktnujvza",
			  "oiwcdpbseqpxryfmlgzwnujvza", "eiwcdpbseqnxryfplgktnujvza",
			  "oiwcdbbseqhxryfmlmktnujvha", "oiwcdpbseqhxryfmlgktjhjvka",
			  "oiwcdpbseqhxnyfylgktnujvzs", "oiwcdpbbxqhxryfmzgktnujvza",
			  "opwcdpbseqhfryfmlgktnujzza", "oiwcdpbsjqpxryfmtgktnujvza",
			  "oiwcdpbseqhyqbfmlgktnujvza", "oxwcdpbseqhxrffmlgktiujvza",
			  "oiwcdpbseqhxgyfmlgytnujvzq", "oiwidpbseqhxryfmlgxtnujvzd",
			  "oiwcdpbshqhxryzmlpktnujvza", "oifcdpbseqbxryfmlgktdujvza",
			  "biwcdzbseqhxtyfmlgktnujvza", "oiwcdpbswqhxryfmlgutnujvca",
			  "xiwcdpbseqhxryxmlnktnujvza", "oiwcdpzseqhxryfrlgktdujvza",
			  "oiwudpbseqhxryfmlgkqnurvza", "oiwqdpbseihiryfmlgktnujvza",
			  "iiwjdpbseqhxryamlgktnujvza", "oiwcdplseqhqryfmmgktnujvza",
			  "oiwcdppseqhxrmfmlgklnujvza", "oiwcdobseqhxryfmmgkttujvza",
			  "odwcdpbseqhxryfmlgktnujvyk", "oiwcdpkseqhxrhfmlgktntjvza",
			  "oiocdpbseqhxryfmlgjknujvza", "oiicdpbieqhxryfmlgksnujvza",
			  "oiwcdpbseqhxryemlgktnujdla", "oiwcdpbseqdxryfmlgktsujvzt",
			  "oiwcdcksnqhxryfmlgktnujvza", "oowcdpbseqhxryfmlgsfnujvza",
			  "oawcdpbseqhxryfmljktnuevza", "oiwcdpbsaqhxrffmzgktnujvza",
			  "oiwcipbseqhcryfmlgktnujvga", "oiwcdpbsewhxrbfmlgktnuuvza",
			  "oiwcdpbsewhxryfmlgkunujvzc", "oiwcdpbseqhxryfmlgktiujkga",
			  "jiwcdpbseqhxrlfmlgktnurvza", "tiwcdpbseqoxryfmliktnujvza",
			  "oiwcdpbsenhxryfmlgkskujvza", "oiwcdpbseqhxvyfmlhktoujvza",
			  "oiwcdpbseqhxeyfmlgmtnunvza", "oiwcdpbseqhxdyfmloktnujvzu",
			  "oiwcdpbseqgxryfmlgkynejvza", "oudcdpbseqhxryfmlgktmujvza",
			  "oiwcdpbseqhxryfmvgktnucvzc", "oiwcdpbseqhxrysalgwtnujvza",
			  "odwodpbseqhgryfmlgktnujvza", "oiwcdpbseqheryzmlgktnujgza",
			  "oiwcdpbseqhxryfalgwtnujvba", "oiwcdpbseqhxryfmlgtdnuhvza",
			  "oiocdpbseqhxrefulgktnujvza", "kiwcdpbseqhxrywolgktnujvza",
			  "niwcdpbseksxryfmlgktnujvza", "oiwcdybseqexryfalgktnujvza",
			  "oiwcdpbbeqhxryamlgktnujpza", "oiecdqbseqhxryfmlgktnujcza",
			  "oiwcdpbsqqhxlyfmlpktnujvza", "oiwcdpbsaqheryfmlgktnujlza",
			  "oiecdpbseqhxryfmlgkknujvzz", "oiwcapbsdqhxryfmlgktvujvza",
			  "ojwcdxbseqhxryfmlgktrujvza", "oiwhdpbseqvxrzfmlgktnujvza",
			  "oiwcdppseqhtryfmlgktnujvzs", "oikcdpbsfqhxryfmdgktnujvza",
			  "owwczpbseqhxryfilgktnujvza", "oifwdpbseqhxryfmlgktnujfza",
			  "oowcdpbseqhxrprmlgktnujvza", "oiwcapbseqhxryfmjgktnujvze",
			  "oiwcdpaseqhdrybmlgktnujvza", "tiwcdpbseqhxryfmlgkvjujvza",
			  "xiwcdpbseqhoryfmlgktnujvqa", "eiwrdpbsyqhxryfmlgktnujvza",
			  "oiwcdpbseqhxranmlgktnujvzt", "oiwcdpbseqhxryfqlgktnudaza",
			  "oiwcdpbsvqhxrywmlgktnsjvza", "oewcdpbseqhxryfmlgkunujvma",
			  "oiwcdpbseqhjrywmlgktnujvzb", "omwcdpbseqhxryfmlgktwujsza",
			  "oiwcdpbyxqhxryfmlgrtnujvza", "oiwidpbseqhxryfhlgktnunvza",
			  "oqwcdpbweqhxrybmlgktnujvza", "oiwcdqbseqhxryfrlgktnujfza",
			  "oiacdpbseqhdryfmlgktaujvza", "oiwcdpbstqhxmyfmlgktyujvza",
			  "oiwcdpbseqhxeyfclgktjujvza", "wiwcdpeseqhxryfmlgktnujvzx",
			  "viwcdpbseqhxryfmlgvtvujvza", "oircdpbseqhxcyfmlgktnujvma",
			  "miwcdpbseqtwryfmlgktnujvza", "oiwcppbseqhxcyfmlgxtnujvza",
			  "giwcrpbseqhxryfmlgktnudvza", "onwcvpbseqhxryfmlgktnujqza",
			  "oiwcipbseqhxryfmlgitnuqvza", "oiwcdpbseqhxryjmlgkonufvza",
			  "oiwnwpbseqhxtyfmlgktnujvza", "oiwcypbseqhxryfmlgetnujvzv",
			  "oiwcdpbseqhxryqmljktnkjvza", "olwcdpbseqhxryfmlgkenujvba",
			  "biwcdpbseqwxrywmlgktnujvza", "oiwcdpbsevhmryjmlgktnujvza",
			  "oiwcdpbseqhxryfmlzktnkjvzv", "oiwudpbseqhxrefmlgktnujvia",
			  "oiicdpbseqhxryfdloktnujvza", "oihcjpbsxqhxryfmlgktnujvza",
			};

			void part_01( ) {
				auto const result = *daw::bench_n_test<100>(
				  "Day 2, part 1",
				  []( auto const &ids ) { return get_checksum( ids ); }, box_ids );

				daw::expecting( result, 5880 );
				static_assert( get_checksum( box_ids ) == 5880 );
			}

			void part_02( ) {
				auto const result = *daw::bench_n_test<100>(
				  "Day 2, part 2", []( auto const &ids ) { return get_match( ids ); },
				  box_ids );

				expecting( result, "tiwcdpbseqhxryfmgkvjujvza" );
			}
		} // namespace day02
	}   // namespace aoc_2018
} // namespace daw

int main( ) {
	daw::aoc_2018::day02::part_01( );
	daw::aoc_2018::day02::part_02( );
}
