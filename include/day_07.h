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

		template<size_t N>
		constexpr std::array<reqs_t, N>
		process_reqs( std::array<daw::string_view, N> req_strs ) noexcept {
			std::array<reqs_t, N> result{};
			// "Step G must be finished before step T can begin."
			//  0123456789012345678901234567890123456
			//            1         2         3
			daw::algorithm::transform( begin( req_strs ), end( req_strs ),
			                           result.begin( ), []( auto sv ) -> reqs_t {
				                           return {sv[5], sv[36]};
			                           } );
			return result;
		}

		struct node_t {
			char self;
			std::vector<char> parents{};
			std::vector<char> children{};
			bool walked = false;

			node_t( char Self ) noexcept
			  : self( Self ) {}

			void add_child( char c ) {
				children.push_back( c - 'A' );
			}

			void add_parent( char c ) {
				parents.push_back( c - 'A' );
			}
		};

		template<size_t Count>
		std::string walk( std::array<std::optional<node_t>, Count> &priorities,
		                  size_t N ) {
			auto &node = priorities[N];
			if( !node ) {
				return {};
			}
			if( node->walked ) {
				return {};
			}
			node->walked = true;
			std::string result{};
			for( auto const &parent : node->parents ) {
				if( priorities[static_cast<size_t>( parent )]->walked ) {
					continue;
				}
				result += walk( priorities, static_cast<size_t>( parent ) );
			}
			result.push_back( 'A' + static_cast<char>( N ) );
			return result;
		}

		struct pnode_t {
			size_t self;
			bool has_parent = false;
			std::vector<size_t> children{};

			pnode_t( size_t id ) noexcept: self( id ) { }
		};

		template<size_t N>
		std::string part_01( std::array<reqs_t, N> const &reqs ) {
			std::vector<pnode_t> nodes{};

			auto const find_node = [&]( char k ) -> std::optional<size_t> {
				k -= 'A';
				auto pos = std::find_if( begin( nodes ), end( nodes ), [k]( auto v ) {
					return k == v.self;
				});
				if( pos == end( nodes ) ) {
					return {};
				}
				return static_cast<size_t>( std::distance( begin( nodes ), pos ) );
			};

			auto const add_child = [&]( char parent, char child ) {
				parent -= 'A';
				child -= 'A';
				
			};

			for( auto const & req: reqs ) {
				auto node = find_node( reqs.parent );
				if( !node ) {
					nodes.emplace_back( reqs.parent );
				}
			}
		}

	} // namespace
} // namespace daw
