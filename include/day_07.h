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
			daw::algorithm::transform( begin( req_strs ), end( req_strs ),
			                           result.begin( ), []( auto sv ) -> reqs_t {
				                           return {static_cast<char>( sv[5] ),
				                                   static_cast<char>( sv[36] )};
			                           } );

			return result;
		}

		template<size_t N>
		daw::graph_t<char> convert_to_graph( std::array<reqs_t, N> const & reqs ) {
			daw::graph_t<char> graph{};
			for( reqs_t req: reqs ) {
				daw::node_id_t parent_id{};
				daw::node_id_t child_id{};
				if( auto parent = graph.find_by_value(req.parent); parent.empty( ) ) {
					parent_id = graph.add_node( req.parent );
				} else {
					parent_id = parent.front();
				}
				if( auto child = graph.find_by_value(req.child); child.empty( ) ) {
					child_id = graph.add_node( req.child );
				} else {
					child_id = child.front();
				}
				graph.add_directed_edge( parent_id, child_id );
			}
			return graph;
		}

		struct node_less_by_value {
			template<typename T>
			constexpr bool operator( )( T const & lhs, T const & rhs ) const noexcept {
				return lhs.value( ) < rhs.value( );
			}
		};

		template<typename Graph, typename Node>
		auto get_sorted_edges( Graph && graph, Node && node ) {
			using node_t = std::remove_reference_t<decltype( graph.get_node( node.id( ) ) )>;
			std::set<node_t, node_less_by_value> result{};
			for( auto m_id: node.outgoing_edges( ) ) {
				result.insert( graph.get_node( m_id ) );
			}
			return result;
		}

		template<size_t N>
		std::string part_01( std::array<reqs_t, N> const & reqs ) {
			auto graph = convert_to_graph( reqs );

			auto root_nodes = graph.find( []( auto const & node ) {
				return node.incoming_edges( ).empty( );
			});

			std::set<daw::graph_node_t<char>, node_less_by_value> S{};
			for( auto node_id: root_nodes ) {
				S.insert( graph.get_node( node_id ) );
			}

			std::string result{};
			while( !S.empty( ) ) {
				auto node = *std::begin( S );
				S.erase( node );
				result.push_back( node.value( ) );

				for( auto child: get_sorted_edges( graph, node ) ) {
					graph.remove_edges( node.id( ), child.id( ) );
					if( child.incoming_edges( ).empty( ) ) {
						S.insert( child );
					}
				}
			}
			return result;
		}
	} // namespace
} // namespace daw
