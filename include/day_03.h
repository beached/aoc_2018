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
		struct request_t {
			uint16_t id;
			uint16_t left;
			uint16_t top;
			uint16_t width;
			uint16_t height;
		};

		template<typename Container>
		std::vector<request_t> parse_request( Container &&requests ) {
			std::vector<request_t> result{};
			for( daw::string_view sv : requests ) {
				// format: #123 @ 3,2: 5x4
				sv.remove_prefix( );
				request_t tmp{};
				auto sv_tmp = sv.pop_front( sv.find_first_of( ' ' ) );
				tmp.id = daw::parser::parse_int<uint16_t>( sv_tmp );
				sv.remove_prefix( sv.find_first_of( '@' ) + 2 );
				sv_tmp = sv.pop_front( sv.find_first_of( ',' ) );
				tmp.left = daw::parser::parse_int<uint16_t>( sv_tmp );
				sv.remove_prefix( );
				sv_tmp = sv.pop_front( sv.find_first_of( ':' ) );
				tmp.top = daw::parser::parse_int<uint16_t>( sv_tmp );
				sv.remove_prefix( 2 );
				sv_tmp = sv.pop_front( sv.find_first_of( 'x' ) );
				tmp.width = daw::parser::parse_int<uint16_t>( sv_tmp );
				sv.remove_prefix( );
				tmp.height = daw::parser::parse_int<uint16_t>( sv );
				result.push_back( tmp );
			}
			return result;
		}

		template<uint16_t width, uint16_t height>
		class fabric_t {
			static inline constexpr size_t const m_size =
			  static_cast<size_t>( width * height );

			std::array<uint16_t, m_size> m_values{};

		public:
			using size_type = uint16_t;

			fabric_t( ) = default;

			constexpr auto &operator( )( size_type x, size_type y ) noexcept {
				return m_values[static_cast<size_t>( x ) +
				                ( static_cast<size_t>( y ) *
				                  static_cast<size_t>( width ) )];
			}

			constexpr auto const &operator( )( size_type x, size_type y ) const
			  noexcept {
				return m_values[static_cast<size_t>( x ) +
				                ( static_cast<size_t>( y ) *
				                  static_cast<size_t>( width ) )];
			}

			constexpr bool is_conflicted( size_type x, size_type y ) const noexcept {
				return operator( )( x, y ) > 1;
			}
		};

		template<typename Container>
		uint32_t calc_conflicted_area( Container &&requests ) {
			fabric_t<1000, 1000> fabric{};
			uint32_t conflict_area = 0;

			for( auto const req : requests ) {
				for( auto x = req.left; x < ( req.left + req.width ); ++x ) {
					for( auto y = req.top; y < ( req.top + req.height ); ++y ) {
						if( ++fabric( x, y ) == 2U ) {
							++conflict_area;
						}
					}
				}
			}
			return conflict_area;
		}

		template<typename Container>
		auto apply_reqs_to_fabric( Container &&requests ) {
			struct result_t {
				fabric_t<1000, 1000> fabric{};
				std::vector<bool> conflicts{};
			};

			result_t result{};
			// Assume req id's match size, if not grow later
			result.conflicts.resize( std::size( requests ) + 1, false );

			for( auto const req : requests ) {
				for( auto x = req.left; x < ( req.left + req.width ); ++x ) {
					for( auto y = req.top; y < ( req.top + req.height ); ++y ) {
						++result.fabric( x, y );
						if( result.fabric( x, y ) > 1U ) {
							result.conflicts[req.id] = true;
						}
					}
				}
			}
			return result;
		}

		template<typename Request, typename Fabric>
		constexpr bool is_conflict_free( Request &&req, Fabric &&fabric ) noexcept {
			uint16_t const max_x = req.left + req.width - 1U;
			uint16_t const max_y = req.top + req.height - 1U;
			for( uint16_t x = req.left; x <= max_x; ++x ) {
				if( fabric.is_conflicted( x, req.top ) or
				    fabric.is_conflicted( x, max_y ) ) {
					return false;
				}
			}
			for( uint16_t y = req.top + 1U; y < max_y; ++y ) {
				if( fabric.is_conflicted( req.left, y ) or
				    fabric.is_conflicted( max_x, y ) ) {
					return false;
				}
			}
			// Ensure no request is enclosed by this one
			for( uint16_t x = req.left + 1U; x < max_x; ++x ) {
				for( uint16_t y = req.top + 1U; y < max_y; ++y ) {
					if( fabric.is_conflicted( x, y ) ) {
						return false;
					}
				}
			}
			return true;
		}

		template<typename Container>
		uint16_t find_unconflicted_area( Container &&reqs ) {
			auto const result = apply_reqs_to_fabric( reqs );

			for( request_t const req : reqs ) {
				if( result.conflicts[req.id] ) {
					continue;
				}
				if( is_conflict_free( req, result.fabric ) ) {
					return req.id;
				}
			}
			std::terminate( );
		}
	} // namespace
} // namespace daw
