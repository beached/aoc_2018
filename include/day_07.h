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

#include <cstdint>

#include <daw/daw_algorithm.h>
#include <daw/daw_keep_n.h>
#include <daw/daw_parser_helper_sv.h>
#include <daw/daw_static_string.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

namespace daw {
	namespace {
		struct reqs_t {
			char parent;
			char child;
		};

		constexpr bool operator<( reqs_t const & lhs, reqs_t const & rhs ) noexcept {
			return lhs.parent < rhs.parent;
		}

		template<size_t N>
		constexpr std::array<reqs_t, N>
		process_reqs( std::array<daw::string_view, N> req_strs ) noexcept {
			std::array<reqs_t, N> result{};
			// "Step G must be finished before step T can begin."
			//  0123456789012345678901234567890123456
			//            1         2         3
			daw::algorithm::transform( begin( req_strs ), end( req_strs ),
			                           result.begin( ), []( auto sv ) -> reqs_t {
				                           return {static_cast<char>( sv[5] - 'A' ), static_cast<char>( sv[36] - 'A' )};
			                           } );

			return result;
		}

		struct node_t {
			char self;
			std::set<char> parents{};
			std::set<char> children{};
			bool visited = false;

			node_t( ) noexcept: self( -1 ) { }

			node_t( char Self ) noexcept
			  : self( Self ) {}

			void add_child( char c ) {
				children.insert( c );
			}

			void add_parent( char c ) {
				parents.insert( c );
			}
		};

		bool operator<( node_t const & lhs, node_t const & rhs ) noexcept {
			return lhs.self < rhs.self;
		}

		inline std::string walk( std::map<int, node_t> & nodes, char id ) {
			auto & node = nodes[id];
			if( node.visited ) {
				return {};
			}
			node.visited = true;
			std::string result{};
			for( auto & parent: node.parents ) {
					result += walk( nodes, parent );
			}
			result += id + 'A';
			for( auto & child: node.children ) {
				result += walk( nodes, child );
			}
			return result;
		}

		void add_child ( std::map<int, node_t> & nodes, char parent, char child ) {
			std::cout << nodes.size( ) << '\n';
			if( nodes.count( parent ) == 0 ) {
				nodes[parent].self = parent;
			}
			nodes[parent].add_child( child );

			if( nodes.count( child ) == 0 ) {
				nodes[child].self = child;
			}
			nodes[child].add_parent( parent );
			std::cout << nodes.size( ) << '\n';
		};

		template<size_t N>
		std::string part_01( std::array<reqs_t, N> reqs ) {
			std::sort( begin( reqs ), end( reqs ) );
			std::map<int, node_t> nodes{};

			for( auto const & req: reqs ) {
				add_child( nodes, req.parent, req.child );
			}

			auto const find_root_nodes = []( auto const & n ) {
				auto const & node = n.second;
				return !node.walked and node.parents.empty( );
			};

			std::string result{};
			for( auto const & req: reqs ) {
				if( !nodes[req.parent].walked and nodes[req.parent].parents.empty( ) ) {
					result += walk( nodes, req.parent );
				}
			}
			return result;
		}

	} // namespace
} // namespace daw
