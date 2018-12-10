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
#include <daw/daw_graph.h>
#include <daw/daw_graph_algorithm.h>
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

		template<size_t N>
		constexpr std::array<reqs_t, N>
		process_reqs( std::array<daw::string_view, N> req_strs ) noexcept {
			std::array<reqs_t, N> result{};
			// "Step G must be finished before step T can begin."
			//  0123456789012345678901234567890123456
			//            1         2         3
			daw::algorithm::transform(
			  begin( req_strs ), end( req_strs ), result.begin( ),
			  []( auto sv ) -> reqs_t {
				  return {static_cast<char>( sv[5] ), static_cast<char>( sv[36] )};
			  } );

			return result;
		}

		template<size_t N>
		daw::graph_t<char> convert_to_graph( std::array<reqs_t, N> const &reqs ) {
			daw::graph_t<char> graph{};
			for( reqs_t req : reqs ) {
				daw::node_id_t parent_id{};
				daw::node_id_t child_id{};
				if( auto parent = graph.find_by_value( req.parent ); parent.empty( ) ) {
					parent_id = graph.add_node( req.parent );
				} else {
					parent_id = parent.front( );
				}
				if( auto child = graph.find_by_value( req.child ); child.empty( ) ) {
					child_id = graph.add_node( req.child );
				} else {
					child_id = child.front( );
				}
				graph.add_directed_edge( parent_id, child_id );
			}
			return graph;
		}

		struct node_less_by_value {
			template<typename T>
			constexpr bool operator( )( T const &lhs, T const &rhs ) const noexcept {
				return lhs.value( ) < rhs.value( );
			}
		};

		template<typename Graph, typename Node>
		auto get_sorted_edges( Graph &&graph, Node &&node ) {
			using node_t =
			  std::remove_reference_t<decltype( graph.get_node( node.id( ) ) )>;
			std::set<node_t, node_less_by_value> result{};
			for( auto m_id : node.outgoing_edges( ) ) {
				result.insert( graph.get_node( m_id ) );
			}
			return result;
		}

		template<size_t N>
		std::string part_01( std::array<reqs_t, N> const &reqs ) {
			auto graph = convert_to_graph( reqs );
			std::string result{};
			daw::topological_sorted_walk(
			  graph,
			  [&result]( auto const &node ) { result.push_back( node.value( ) ); },
			  []( auto const &lhs, auto const &rhs ) {
				  return lhs.value( ) < rhs.value( );
			  } );
			return result;
		}

		template<size_t BaseTime, typename T>
		class work_item {
			size_t m_tick = 0;
			graph_node_t<T> m_node{};

		public:
			constexpr work_item( ) noexcept = default;
			constexpr work_item( size_t tick, graph_node_t<T> node ) noexcept
			  : m_tick( tick )
			  , m_node( std::move( node ) ) {}

			constexpr size_t cost( ) const {
				if( !m_node ) {
					return 0;
				}
				return static_cast<size_t>( m_node.value( ) - 'A' ) + 1U + BaseTime;
			}

			constexpr bool empty( ) const {
				return !static_cast<bool>( m_node );
			}

			constexpr graph_node_t<T> remove_node( ) noexcept {
				auto result = std::move( m_node );
				m_node = graph_node_t<T>( );
				m_tick = 0;
				return result;
			}

			constexpr bool done( size_t tick ) const {
				return !m_node or tick >= m_tick + cost( );
			}

			constexpr graph_node_t<T> &get_node( ) noexcept {
				return m_node;
			}

			constexpr graph_node_t<T> const &get_node( ) const noexcept {
				return m_node;
			}
		};

		template<size_t BaseTime, size_t NumWorkers>
		struct work_queue_t {
			std::array<work_item<BaseTime, char>, NumWorkers> workers{};

			decltype( auto ) find_free_queue( size_t tick ) {
				return std::find_if(
				  begin( workers ), end( workers ),
				  [tick]( auto const &w ) { return w.empty( ) or w.done( tick ); } );
			}

			decltype( auto ) find_free_queue( size_t tick ) const {
				return std::find_if(
				  begin( workers ), end( workers ),
				  [tick]( auto const &w ) { return w.empty( ) or w.done( tick ); } );
			}

			bool has_room( size_t tick ) const {
				return find_free_queue( tick ) != std::end( workers );
			}

			bool all_done( size_t tick ) const {
				for( work_item<BaseTime, char> const &w : workers ) {
					if( !w.done( tick ) ) {
						return false;
					}
				}
				return true;
			}

			bool empty( ) const {
				for( work_item<BaseTime, char> const &w : workers ) {
					if( !w.empty( ) ) {
						return false;
					}
				}
				return true;
			}
		};

		template<size_t BaseTime = 60U, size_t NumWorkers = 5U, size_t N>
		size_t part_02( std::array<reqs_t, N> const &reqs ) {
			auto graph = convert_to_graph( reqs );

			auto root_nodes = graph.find(
			  []( auto const &node ) { return node.incoming_edges( ).empty( ); } );

			std::set<daw::graph_node_t<char>, node_less_by_value> S{};
			for( auto node_id : root_nodes ) {
				S.insert( graph.get_node( node_id ) );
			}

			work_queue_t<BaseTime, NumWorkers> work_queue{};
			size_t tick = 0;

			auto const work_to_do = [&]( ) {
				return !S.empty( ) or !work_queue.empty( );
			};
			auto const need_to_wait = [&]( ) { return !work_queue.has_room( tick ); };
			auto const completed_work = [&]( ) {
				std::vector<graph_node_t<char>> result{};
				for( work_item<BaseTime, char> &w : work_queue.workers ) {
					if( w.done( tick ) ) {
						auto n = w.remove_node( );
						if( n ) {
							result.push_back( std::move( n ) );
						}
					}
				}
				return result;
			};

			while( work_to_do( ) ) {
				while( need_to_wait( ) ) {
					++tick;
				}
				auto old_items = completed_work( );
				while( !old_items.empty( ) ) {
					graph_node_t<char> old_node = std::move( old_items.back( ) );
					old_items.pop_back( );
					for( graph_node_t<char> child :
					     get_sorted_edges( graph, old_node ) ) {

						graph.remove_edges( old_node.id( ), child.id( ) );
						if( child.incoming_edges( ).empty( ) ) {
							S.insert( child );
						}
					}
				}
				while( !S.empty( ) and work_queue.has_room( tick ) ) {
					auto q = work_queue.find_free_queue( tick );
					*q = work_item<BaseTime, char>( tick, *std::begin( S ) );
					S.erase( S.begin( ) );
				}
				++tick;
			}
			return tick - 1;
		}
	} // namespace
} // namespace daw
