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

		constexpr bool operator<( reqs_t const &lhs, reqs_t const &rhs ) noexcept {
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
				                           return {static_cast<char>( sv[5] - 'A' ),
				                                   static_cast<char>( sv[36] - 'A' )};
			                           } );

			return result;
		}

		struct node_t {
			char self;
			std::set<char> parents{};
			std::set<char> children{};
			bool visited = false;

			node_t( ) noexcept
			  : self( -1 ) {}

			node_t( char Self ) noexcept
			  : self( Self ) {}

			void add_child( char c ) {
				children.insert( c );
			}

			void add_parent( char c ) {
				parents.insert( c );
			}
		};

		bool operator<( node_t const &lhs, node_t const &rhs ) noexcept {
			return lhs.self < rhs.self;
		}

		inline std::string walk( std::map<int, node_t> &nodes, char id,
		                         intmax_t depth ) {
			auto &node = nodes[id];
			bool const visited = node.visited;
			std::string result{};
			/*
			for( auto & parent: node.parents ) {
			    result += walk( nodes, parent );
			}
			 */
			if( depth == 0 ) {
				if( !visited ) {
					node.visited = true;
					result += id + 'A';
				}
			} else {
				for( auto &child : node.children ) {
					result += walk( nodes, child, --depth );
				}
			}
			return result;
		}

		void add_child( std::map<int, node_t> &nodes, char parent, char child ) {
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

			for( auto const &req : reqs ) {
				add_child( nodes, req.parent, req.child );
			}

			auto const find_root_nodes = []( auto const &n ) {
				auto const &node = n.second;
				return !node.visited and node.parents.empty( );
			};

			std::set<char> S{};
			for( auto const &node : nodes ) {
				if( node.second.parents.empty( ) ) {
					S.insert( node.second.self );
				}
			}

			auto cur_nodes = nodes;
			std::string result{};
			while( !S.empty( ) ) {
				auto n = cur_nodes[*std::begin( S )];
				S.erase( n.self );
				result.push_back( n.self + 'A' );
				for( auto &m_id : cur_nodes[n.self].children ) {
					auto &m = cur_nodes[m_id];
					m.parents.erase( n.self );
					n.children.erase( m.self );
					if( m.parents.empty( ) ) {
						S.insert( m.self );
					}
				}
			}

			return result;
		}

		template<size_t BaseTime = 60, size_t NumWorkers = 5, size_t N>
		size_t part_02( std::array<reqs_t, N> reqs ) {
			std::sort( begin( reqs ), end( reqs ) );

			std::map<char, std::set<char>> nodes{};

			for( reqs_t const &req : reqs ) {
				nodes[req.child].insert( req.parent );
				if( nodes.count( req.parent ) == 0 ) {
					nodes[req.parent] = {};
				}
			}

			std::map<char, std::set<char>> S{};
			for( auto &node : nodes ) {
				if( node.second.empty( ) ) {
					S[node.first] = node.second;
				}
			}

			std::string L{};
			while( !S.empty( ) ) {
				auto n = *std::begin( S );
				S.erase( n.first );
				L.push_back( n.first + 'A' );
				for( auto m_id : nodes[n.first] ) {
					auto & m = nodes[m_id];
					if( m.empty( ) ) {
						S[m_id] = {};
					}
				}
			}
			struct work_item_t {
				char id;
				size_t load;
			};

			auto const pop_front = []( auto & container ) {
				auto result = *std::begin( container );
				container.erase( container.begin( ) );
				return result;
			};

			size_t result = 0;
			std::vector<work_item_t> workers{};
			while( !L.empty( ) ) {
				++result;
				for( auto & w: workers ) {
					if( w.load == 0 and !L.empty( ) ) {
						auto d = pop_front( L );
						w.id = d;
						w.load = static_cast<size_t>(d - 'A') + 1U + BaseTime;
					}
				}
			}
			return result;
		}
	} // namespace
} // namespace daw
