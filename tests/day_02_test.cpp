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

#define BOOST_TEST_MODULE aoc_2018_day_02
#include <daw/boost_test.h>

#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <daw/daw_benchmark.h>

#include "day_02.h"

namespace daw {
	namespace aoc_2018 {
		namespace day02 {
			using namespace std::string_literals;
			static std::array<std::string, 250> const box_ids = {
			  "oiwcdpbseqgxryfmlpktnupvza"s, "oiwddpbsuqhxryfmlgkznujvza"s,
			  "ziwcdpbsechxrvfmlgktnujvza"s, "oiwcgpbseqhxryfmmgktnhjvza"s,
			  "owwcdpbseqhxryfmlgktnqjvze"s, "oiscdkbseqhxrffmlgktnujvza"s,
			  "oiwcdibseqoxrnfmlgktnujvza"s, "oiwcdpbsejhxryfmlektnujiza"s,
			  "oewcdpbsephxryfmlgkwnujvza"s, "riwcdpbseqhxryfmlgktnujzaa"s,
			  "omwcdpbseqwxryfmlgktnujvqa"s, "oiwcdqqneqhxryfmlgktnujvza"s,
			  "oawcdvaseqhxryfmlgktnujvza"s, "oiwcdabseqhxcyfmlkktnujvza"s,
			  "oiwcdpbseqhxryfmlrktrufvza"s, "oiwcdpbseqhxdyfmlgqtnujkza"s,
			  "oiwcdpbseqhxrmfolgktnujvzy"s, "oiwcdpeseqhxnyfmlgktnejvza"s,
			  "oiwcdpbseqhxrynmlaktlujvza"s, "oiwcdpbseqixryfmlektncjvza"s,
			  "liwtdpbseqhxryfylgktnujvza"s, "ouwcdpbszqhxryfmlgktnajvza"s,
			  "oiwzdpbseqhxryfmngktnujvga"s, "wiwcfpbseqhxryfmlgktnuhvza"s,
			  "oiwcdpbselhfryfmlrktnujvza"s, "oywcdpbveqhxryfmlgktnujdza"s,
			  "oiwcdpbsiqhxryfmqiktnujvza"s, "obwcdhbseqhxryfmlgktnujvqa"s,
			  "oitcdpbseqhfryfmlyktnujvza"s, "oiwcdpbseqhxryfmlnutnujqza"s,
			  "oiwcdpbseqhxnyfmlhktnujtza"s, "oiwcdpbseqhbryfmlgkunuwvza"s,
			  "oiwcopbseqhiryfmlgktnkjvza"s, "oiwcdpsseqhxryfklrktnujvza"s,
			  "oiwcdpsrsqhxryfmlgktnujvza"s, "oiwcdpbsyqrxryfmlgktnujvzc"s,
			  "ojwcepbseqhxryfmlgktnujvfa"s, "oiwcdpbseqhxrlfmlgvtnujvzr"s,
			  "oiycdpbsethsryfmlgktnujvza"s, "eiwcdpbseqwxryfmlgktnujcza"s,
			  "oiocdpbseqhxryfxlgktaujvza"s, "qiwydpbseqhpryfmlgktnujvza"s,
			  "ziwcdpbseqhxryfmlgktsuuvza"s, "oiwcdpbseqheryfmygxtnujvza"s,
			  "oiwidpbseqhxryfulgktnujvzm"s, "oiscdpdseqhxryfmlgktnujvya"s,
			  "oiwmypbseqhxryfmlgktnuxvza"s, "oizcwbbseqhxryfmlgktnujvza"s,
			  "oiwcdpbseqpxryfmlgxfnujvza"s, "oiwpdpbscqhxryxmlgktnujvza"s,
			  "oiwcdpbseqhxrifrlgkxnujvza"s, "oiwcdpbsrqhxrifmlgktnzjvza"s,
			  "tiwcdpbseqhxryfmegkvjujvza"s, "oiwcddbseqhxryfingktnujvza"s,
			  "oiwcdpbsiqhiryfmlgktnucvza"s, "oiwcipbseqhxrkfmlgktnuvvza"s,
			  "kiwcdpbseqhxryfmlbkonujvza"s, "qiwcdhbsedhxryfmlgktnujvza"s,
			  "siwcdpbseqhxryfmsgktnujvua"s, "oqwcdpbseqhxryfmlyktndjvza"s,
			  "oiwcqnbseehxryfmlgktnujvza"s, "oiwcdybseqhxryfmlgbtnujvia"s,
			  "oiwcdpbsejhxryfdlgktngjvza"s, "oawcdpbseqhxrbfmlkktnujvza"s,
			  "oilcdpbseqhhrjfmlgktnujvza"s, "oibcdpbseqhxryfmngktnucvza"s,
			  "niwcdpbseqhxryfmlgkuaujvza"s, "oiwcdpbseqhxryfmqgmtnujvha"s,
			  "oiwcdpbseqhcryfxlgktnzjvza"s, "oiwcdpaseqhxryfmqgktnujvzl"s,
			  "oiwcdpbseqhxjyfmlgkonujvzx"s, "oiwmdzbseqlxryfmlgktnujvza"s,
			  "oiwhdpbsexhxryfmlgktnujvzw"s, "oiwctpbseqhxryfmlgktnummza"s,
			  "oiwcdpbseqoxrydmlgktnujvoa"s, "oiwkdpvseqhxeyfmlgktnujvza"s,
			  "oixcdpbsemhxryfmlgctnujvza"s, "oimcdpbseqhxryfmlgktnujvlr"s,
			  "oiwcdpbseehxrywmlgktnejvza"s, "oiwcdpbseqoxhyfmlgktnujyza"s,
			  "oiwcdpbsethxryfmlgktnrjvxa"s, "oiwcdpbxeqhxryfmlgktnrjvzb"s,
			  "ogeadpbseqhxryfmlgktnujvza"s, "eiwcdpbseqhxryfmlgktnvuvza"s,
			  "oiwcdpbseqhxryfmlgktnujaxv"s, "siwcdpbsuqhxryfmlgktnuavza"s,
			  "oixcdpbseqhxryfmlgatnujvzy"s, "oiwcdpbzeghmryfmlgktnujvza"s,
			  "oiwcdpbieqhxryfmlgktyujvzr"s, "oiwcdpbseqhxeyfhlgktngjvza"s,
			  "oiwcdpbseqhjoyrmlgktnujvza"s, "iiwcdpbseqhxryfmwhktnujvza"s,
			  "oixcdpbsiqhxryfmagktnujvza"s, "oiwcdpfljqhxryfmlgktnujvza"s,
			  "oivcdpbseqhxrqfmlgktnujvca"s, "ovwcdpbseqhxzyfmlgkenujvza"s,
			  "oiwxdpgseqhxryfmlgktnhjvza"s, "oibcdpbbeohxryfmlgktnujvza"s,
			  "oiwcrpbseqhxrygmlgwtnujvza"s, "jiwcdpbseqkxryfmlgntnujvza"s,
			  "oiwcdbbseqhxrywmlgktnujvra"s, "oiwcdpbteqyxoyfmlgktnujvza"s,
			  "oiwcdjbsvqvxryfmlgktnujvza"s, "obwcdukseqhxryfmlgktnujvza"s,
			  "oifcdpdseqhxryfmlgktnujsza"s, "oiwcdpbseqhxryfalgktnujyda"s,
			  "oiwcwpbseqhxrnfmkgktnujvza"s, "oswcdpbsethcryfmlgktnujvza"s,
			  "oiwcdpbieqhxryfmlgktnuoiza"s, "oiwcdibsehhxryfmzgktnujvza"s,
			  "oisjdpbseqhxryfmfgktnujvza"s, "oiwcjpbseqkxqyfmlgktnujvza"s,
			  "obwcdpbshqhgryfmlgktnujvza"s, "oiwcspbseqhxryxmlgktnujvzl"s,
			  "oiwcdvbswqhxryfmlgklnujvza"s, "oiwcdhuseqhxryfmlgdtnujvza"s,
			  "oiwcdpbkeqdxryfmlgktnujvzv"s, "oiwcdpzseqhxcyfmlgksnujvza"s,
			  "oiwcdpbseqhxryfmbkkvnujvza"s, "qiwcdpbseqhxrnfmlgktnujvha"s,
			  "okwcdpbseqhxryfmdgktgujvza"s, "oiwcdpbkeqhxryfmldktnujvzu"s,
			  "oiwctpxseqhxgyfmlgktnujvza"s, "oiwcdpbseqhxrykmlgktnujita"s,
			  "oiwcdpbseqhxryfmldktyujnza"s, "oiwcdpbszqhxrjfmlgktnuqvza"s,
			  "oiwcdpbeeqhxrykmlgktnujrza"s, "oiwcvpbseqhxryhmlgwtnujvza"s,
			  "oiwcdpbpeehxryfmlgktnujvzz"s, "oiwcdbbsxqhxyyfmlgktnujvza"s,
			  "oiwkdpbseqhxryfplgktnujeza"s, "opwcdpbseqhxdyfmlgctnujvza"s,
			  "oowcdpbseqhnryfmlgktnujvga"s, "oawzdibseqhxryfmlgktnujvza"s,
			  "oiwcdpbfeqzxrjfmlgktnujvza"s, "oiwcdpbseqaxryfmlgkonulvza"s,
			  "oiacdpbseqvxryfmlgktvujvza"s, "oiwudpbseqhxryfwlgktnujvka"s,
			  "oiwcdpbjeqhxryfymgktnujvza"s, "oiwcdpbweqhxrynmlgktnujaza"s,
			  "oiwcdpbseqdxryfclgvtnujvza"s, "oiwcdppseqhxryfmlgmtzujvza"s,
			  "oiwcdpbseqhxrhfelektnujvza"s, "kiwcdpbsnqhxryfmegktnujvza"s,
			  "oiwcdpbseqpxryfmlgzwnujvza"s, "eiwcdpbseqnxryfplgktnujvza"s,
			  "oiwcdbbseqhxryfmlmktnujvha"s, "oiwcdpbseqhxryfmlgktjhjvka"s,
			  "oiwcdpbseqhxnyfylgktnujvzs"s, "oiwcdpbbxqhxryfmzgktnujvza"s,
			  "opwcdpbseqhfryfmlgktnujzza"s, "oiwcdpbsjqpxryfmtgktnujvza"s,
			  "oiwcdpbseqhyqbfmlgktnujvza"s, "oxwcdpbseqhxrffmlgktiujvza"s,
			  "oiwcdpbseqhxgyfmlgytnujvzq"s, "oiwidpbseqhxryfmlgxtnujvzd"s,
			  "oiwcdpbshqhxryzmlpktnujvza"s, "oifcdpbseqbxryfmlgktdujvza"s,
			  "biwcdzbseqhxtyfmlgktnujvza"s, "oiwcdpbswqhxryfmlgutnujvca"s,
			  "xiwcdpbseqhxryxmlnktnujvza"s, "oiwcdpzseqhxryfrlgktdujvza"s,
			  "oiwudpbseqhxryfmlgkqnurvza"s, "oiwqdpbseihiryfmlgktnujvza"s,
			  "iiwjdpbseqhxryamlgktnujvza"s, "oiwcdplseqhqryfmmgktnujvza"s,
			  "oiwcdppseqhxrmfmlgklnujvza"s, "oiwcdobseqhxryfmmgkttujvza"s,
			  "odwcdpbseqhxryfmlgktnujvyk"s, "oiwcdpkseqhxrhfmlgktntjvza"s,
			  "oiocdpbseqhxryfmlgjknujvza"s, "oiicdpbieqhxryfmlgksnujvza"s,
			  "oiwcdpbseqhxryemlgktnujdla"s, "oiwcdpbseqdxryfmlgktsujvzt"s,
			  "oiwcdcksnqhxryfmlgktnujvza"s, "oowcdpbseqhxryfmlgsfnujvza"s,
			  "oawcdpbseqhxryfmljktnuevza"s, "oiwcdpbsaqhxrffmzgktnujvza"s,
			  "oiwcipbseqhcryfmlgktnujvga"s, "oiwcdpbsewhxrbfmlgktnuuvza"s,
			  "oiwcdpbsewhxryfmlgkunujvzc"s, "oiwcdpbseqhxryfmlgktiujkga"s,
			  "jiwcdpbseqhxrlfmlgktnurvza"s, "tiwcdpbseqoxryfmliktnujvza"s,
			  "oiwcdpbsenhxryfmlgkskujvza"s, "oiwcdpbseqhxvyfmlhktoujvza"s,
			  "oiwcdpbseqhxeyfmlgmtnunvza"s, "oiwcdpbseqhxdyfmloktnujvzu"s,
			  "oiwcdpbseqgxryfmlgkynejvza"s, "oudcdpbseqhxryfmlgktmujvza"s,
			  "oiwcdpbseqhxryfmvgktnucvzc"s, "oiwcdpbseqhxrysalgwtnujvza"s,
			  "odwodpbseqhgryfmlgktnujvza"s, "oiwcdpbseqheryzmlgktnujgza"s,
			  "oiwcdpbseqhxryfalgwtnujvba"s, "oiwcdpbseqhxryfmlgtdnuhvza"s,
			  "oiocdpbseqhxrefulgktnujvza"s, "kiwcdpbseqhxrywolgktnujvza"s,
			  "niwcdpbseksxryfmlgktnujvza"s, "oiwcdybseqexryfalgktnujvza"s,
			  "oiwcdpbbeqhxryamlgktnujpza"s, "oiecdqbseqhxryfmlgktnujcza"s,
			  "oiwcdpbsqqhxlyfmlpktnujvza"s, "oiwcdpbsaqheryfmlgktnujlza"s,
			  "oiecdpbseqhxryfmlgkknujvzz"s, "oiwcapbsdqhxryfmlgktvujvza"s,
			  "ojwcdxbseqhxryfmlgktrujvza"s, "oiwhdpbseqvxrzfmlgktnujvza"s,
			  "oiwcdppseqhtryfmlgktnujvzs"s, "oikcdpbsfqhxryfmdgktnujvza"s,
			  "owwczpbseqhxryfilgktnujvza"s, "oifwdpbseqhxryfmlgktnujfza"s,
			  "oowcdpbseqhxrprmlgktnujvza"s, "oiwcapbseqhxryfmjgktnujvze"s,
			  "oiwcdpaseqhdrybmlgktnujvza"s, "tiwcdpbseqhxryfmlgkvjujvza"s,
			  "xiwcdpbseqhoryfmlgktnujvqa"s, "eiwrdpbsyqhxryfmlgktnujvza"s,
			  "oiwcdpbseqhxranmlgktnujvzt"s, "oiwcdpbseqhxryfqlgktnudaza"s,
			  "oiwcdpbsvqhxrywmlgktnsjvza"s, "oewcdpbseqhxryfmlgkunujvma"s,
			  "oiwcdpbseqhjrywmlgktnujvzb"s, "omwcdpbseqhxryfmlgktwujsza"s,
			  "oiwcdpbyxqhxryfmlgrtnujvza"s, "oiwidpbseqhxryfhlgktnunvza"s,
			  "oqwcdpbweqhxrybmlgktnujvza"s, "oiwcdqbseqhxryfrlgktnujfza"s,
			  "oiacdpbseqhdryfmlgktaujvza"s, "oiwcdpbstqhxmyfmlgktyujvza"s,
			  "oiwcdpbseqhxeyfclgktjujvza"s, "wiwcdpeseqhxryfmlgktnujvzx"s,
			  "viwcdpbseqhxryfmlgvtvujvza"s, "oircdpbseqhxcyfmlgktnujvma"s,
			  "miwcdpbseqtwryfmlgktnujvza"s, "oiwcppbseqhxcyfmlgxtnujvza"s,
			  "giwcrpbseqhxryfmlgktnudvza"s, "onwcvpbseqhxryfmlgktnujqza"s,
			  "oiwcipbseqhxryfmlgitnuqvza"s, "oiwcdpbseqhxryjmlgkonufvza"s,
			  "oiwnwpbseqhxtyfmlgktnujvza"s, "oiwcypbseqhxryfmlgetnujvzv"s,
			  "oiwcdpbseqhxryqmljktnkjvza"s, "olwcdpbseqhxryfmlgkenujvba"s,
			  "biwcdpbseqwxrywmlgktnujvza"s, "oiwcdpbsevhmryjmlgktnujvza"s,
			  "oiwcdpbseqhxryfmlzktnkjvzv"s, "oiwudpbseqhxrefmlgktnujvia"s,
			  "oiicdpbseqhxryfdloktnujvza"s, "oihcjpbsxqhxryfmlgktnujvza"s,
			};

			size_t part_01( ) {
				daw::do_not_optimize( box_ids );
				size_t result;
				daw::bench_test( "Part 1",
				                 [&]( ) { result = get_checksum( box_ids ); } );
				daw::do_not_optimize( result );
				return result;
			}

			BOOST_AUTO_TEST_CASE( part_01_test ) {
				BOOST_REQUIRE_EQUAL( part_01( ), 5880 );
			}

			std::string part_02( ) {
				std::string result{};
				daw::do_not_optimize( box_ids );
				daw::bench_test( "Part 2", [&]( ) { result = get_match( box_ids ); } );
				daw::do_not_optimize( result );
				return result;
			}

			BOOST_AUTO_TEST_CASE( part_02_test ) {
				BOOST_REQUIRE_EQUAL( part_02( ), "tiwcdpbseqhxryfmgkvjujvza" );
			}
		} // namespace day02
	}   // namespace aoc_2018
} // namespace daw
