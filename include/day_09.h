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
#include <array>
#include <cstdint>
#include <future>
#include <iostream>
#include <iterator>
#include <list>
#include <set>
#include <tuple>

#include <daw/daw_graph.h>
#include <daw/daw_hash_adaptor.h>
#include <daw/daw_keep_n.h>
#include <daw/daw_parser_helper_sv.h>
#include <daw/daw_static_string.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>
#include <daw/daw_tuple_helper.h>
#include <daw/fs/algorithms.h>

namespace daw {
	namespace {
		struct route_t {
			daw::string_view from;
			daw::string_view to;
			size_t distance;
		};

		constexpr route_t parse_route( daw::string_view sv_route ) noexcept {
			auto from = sv_route.pop_front( sv_route.find_first_of( ' ' ) );
			sv_route.remove_prefix( 4 );
			auto to = sv_route.pop_front( sv_route.find_first_of( ' ' ) );
			sv_route.remove_prefix( 3 );
			auto dist = daw::parser::parse_unsigned_int<size_t>( sv_route );
			return {from, to, dist};
		}

		template<size_t N>
		daw::graph<daw::string_view> parse_input(
		  std::array<daw::string_view, N> const &sv_locations ) noexcept {
			daw::hash_adaptor_t<daw::string_view> location_counter( N );
			for( auto sv: sv_locations ) {
				auto result = parse_route( sv );
				location_set.insert( result.from )
				location_set.insert( result.to )
			};
			daw::graph<daw::string_view> graph{};
			daw::hash_adaptor_t<daw::string_view
			return result;
		}

		template<size_t N, typename Compare = std::less<>>
		constexpr auto make_graph( size_t sz, Compare comp = Compare{} ) noexcept {
			daw::fixed_stack_t<daw::fixed_stack_t<size_t, N>, N> graph{};
			auto const MaxValue =
			  std::max( std::numeric_limits<size_t>::min( ),
			            std::numeric_limits<size_t>::max( ), comp );
			for( size_t n = 0; n < sz; ++n ) {
				graph.emplace_back( );
				for( size_t m = 0; m < sz; ++m ) {
					graph[n].push_back( MaxValue );
				}
			}
			return graph;
		}

		template<size_t N, typename ProbInput, typename Compare = std::less<>>
		constexpr auto make_dist_graph( ProbInput const &prob_input,
		                                Compare comp = Compare{} ) noexcept {
			auto dist_graph =
			  make_graph<N>( prob_input.location_names.size( ), comp );
			for( auto const &route : prob_input.routes ) {
				auto pos0 = static_cast<size_t>( std::distance(
				  begin( prob_input.location_names ),
				  std::find( begin( prob_input.location_names ),
				             end( prob_input.location_names ), route.from ) ) );

				auto pos1 = static_cast<size_t>( std::distance(
				  begin( prob_input.location_names ),
				  std::find( begin( prob_input.location_names ),
				             end( prob_input.location_names ), route.to ) ) );

				dist_graph[pos0][pos1] = route.distance;
				dist_graph[pos1][pos0] = route.distance;
			}

			return dist_graph;
		}

		template<size_t N, typename ProbInput, typename Graph,
		         typename Compare = std::less<>>
		constexpr size_t find_shortest( size_t start, ProbInput const &prob_input,
		                                Graph const &graph,
		                                Compare comp = Compare{} ) noexcept {
			daw::static_hash_adaptor_t<size_t, N> visited{};
			daw::fixed_stack_t<size_t, N> path{};
			path.push_back( start );
			size_t depth = 0;
			auto cur_cost = std::min( 0, std::numeric_limits<size_t>::max( ), comp );
			daw::fixed_stack_t<size_t, N> min_path{};
			while( !path.empty( ) ) {
				++depth;
				auto current_node = path.back( );
				path.pop_back( );
				if( )
					for( size_t n = )
			}
		}

		return 0;
	} // namespace

	template<size_t N>
	constexpr size_t part_01( std::array<daw::string_view, N> const &routes ) {
		auto const prob_input = parse_input( routes );
		auto const dist_graph = make_dist_graph<N>( prob_input );
		Unused( dist_graph );
		return 0;
	}
} // namespace daw
} // namespace daw
