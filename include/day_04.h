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

#pragma once

#include <algorithm>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_set>

#include <daw/daw_algorithm.h>
#include <daw/daw_parser_helper_sv.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

namespace daw {
	namespace {
		enum class action_t : uint8_t { begin_shift, fall_asleep, wake_up };
		struct record_t {
			uint32_t guard_id = std::numeric_limits<uint32_t>::max( );
			action_t action{};
			uint8_t minute = std::numeric_limits<uint8_t>::max( );
		};

		template<size_t N>
		constexpr auto parse_records( std::array<daw::string_view, N> str_reqs ) {
			// [1518-06-05 00:46] falls asleep
			// Ensure data is sorted
			std::sort( begin( str_reqs ), end( str_reqs ) );
			std::array<record_t, N> result{};
			std::transform(
			  begin( str_reqs ), end( str_reqs ), result.data( ),
			  []( daw::string_view sv ) {
				  record_t tmp{};
				  sv.remove_prefix( sv.find_first_of( '[' ) + 1 );
				  sv.remove_prefix( sv.find_first_of( '-' ) + 1 );
				 	sv.remove_prefix( sv.find_first_of( '-' ) + 1 );
				  sv.remove_prefix( sv.find_first_of( ' ' ) + 1 );
				  sv.remove_prefix( sv.find_first_of( ':' ) + 1 );
				  auto sv_min = sv.pop_front( sv.find_first_of( ']' ) );
				  sv.remove_prefix( sv.find_first_of( ' ' ) + 1 );
				  auto min = daw::parser::parse_unsigned_int<unsigned>( sv_min );
				  tmp.minute = static_cast<uint8_t>( min );

				  switch( sv.front( ) ) {
				  case 'G': {
					  sv.remove_prefix( 7 );
					  tmp.guard_id = daw::parser::parse_unsigned_int<uint32_t>(
					    sv.pop_front( sv.find_first_of( ' ' ) ) );
					  tmp.action = action_t::begin_shift;
					  break;
				  }
				  case 'w': {
					  tmp.action = action_t::wake_up;
					  break;
				  }
				  case 'f': {
					  tmp.action = action_t::fall_asleep;
					  break;
				  }
				  default:
					  std::terminate( );
				  }
				  return tmp;
			  } );
			// assumes first guard_id is in front( );
			uint32_t guard_id = -1;
			for( size_t n = 0; n < result.size( ); ++n ) {
				if( result[n].action == action_t::begin_shift ) {
					guard_id = result[n].guard_id;
				} else {
					result[n].guard_id = guard_id;
				}
			}
			return result;
		}

		template<size_t N>
		intmax_t choose_sleepy_guard( std::array<record_t, N> const &recs ) {
			std::unordered_map<uint32_t, std::array<uint16_t, 60>> sleepy_times{};

			auto const find_sleep_nums = [&]( intmax_t guard_id ) {
				auto const &g = sleepy_times[guard_id];
				struct {
					intmax_t total_minutes;
					int8_t max_minute;
				} result { g[0], 0 };

				uint16_t most = g[0];

				for( size_t minute = 1; minute < 60; ++minute ) {
					result.total_minutes += g[minute];
					if( g[minute] > most ) {
						most = g[minute];
						result.max_minute = static_cast<int8_t>( minute );
					}
				}
				return result;
			};

			auto pos = std::find_if( begin( recs ), end( recs ), []( auto const & value ) {
				return value.action == action_t::fall_asleep;
			});
			while( pos != end( recs ) ) {
				++sleepy_times[pos->guard_id][pos->minute];
				auto minute = next( pos )->minute - pos->minute;
				while( --minute >= 0 ) {
					++sleepy_times[pos->guard_id][( pos->minute + minute ) % 60];
					--minute;
				}

				pos = std::find_if( next( pos ), end( recs ), []( auto const & value ) {
					return value.action == action_t::fall_asleep;
				});
			}
			auto first = begin( sleepy_times );
			intmax_t guard_id = first->first;
			auto sleep_num = find_sleep_nums( guard_id );
			std::advance( first, 1 );
			while( first != end( sleepy_times ) ) {
				auto tmp = find_sleep_nums( first->first );
				if( tmp.max_minute > sleep_num.max_minute ) {
					guard_id = first->first;
					sleep_num = tmp;
				}
				++first;
			}
			return guard_id * sleep_num.total_minutes;
		}

		struct best_min_t {
			intmax_t id;
			int8_t minute;
			int count;

			constexpr best_min_t( intmax_t Id, int8_t Minute, int Count ) noexcept: id( Id ), minute( Minute ), count( Count ) {}
		};

		template<size_t N>
		intmax_t
		choose_most_sleepy_on_minute( std::array<record_t, N> const &recs ) {
			std::unordered_map<intmax_t, std::array<uint16_t, 60>> sleepy_times{};

			auto const find_sleep_nums = [&]( intmax_t guard_id ) {
				auto const &g = sleepy_times[guard_id];
				auto pos = std::max_element( begin( g ), end( g ) );
				return best_min_t{ guard_id, static_cast<int8_t>( std::distance( begin( g ), pos ) ), *pos };
			};

			auto pos = std::find_if( begin( recs ), end( recs ), []( auto const & value ) {
				return value.action == action_t::fall_asleep;
			});

			while( pos != end( recs ) ) {
				auto n = static_cast<size_t>( std::distance( begin( recs ), pos ) );
				++sleepy_times[recs[n].guard_id][static_cast<size_t>( recs[n].minute )];

				auto minute = recs[n+1U].minute - recs[n].minute;
				while( --minute >= 0 ) {
					++sleepy_times[recs[n].guard_id][( recs[n].minute + minute ) % 60];
					--minute;
				}

				pos = std::find_if( next( pos ), end( recs ), []( auto const & value ) {
					return value.action == action_t::fall_asleep;
				});
			}

			std::vector<best_min_t> sn{};
			std::transform( begin( sleepy_times ), end( sleepy_times ), std::back_inserter( sn ), [&]( auto const & st ) {
				return find_sleep_nums( st.first );
			});

			auto pos2 = std::max_element( begin( sn ), end( sn ), []( auto const & lhs, auto const & rhs ) {
				return lhs.count < rhs.count;
			});

			return pos2->id * pos2->minute;
		}
	} // namespace
} // namespace daw
