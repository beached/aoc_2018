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
#include <unordered_map>

#include <daw/daw_algorithm.h>
#include <daw/daw_parser_helper_sv.h>
#include <daw/daw_string_view.h>
#include <daw/daw_traits.h>

namespace daw {
	struct request_t {
		size_t id;
		int32_t left;
		int32_t top;
		int32_t width;
		int32_t height;
	};

	template<typename Container>
	std::vector<request_t> parse_request( Container &&requests ) {
		std::vector<request_t> result{};
		for( daw::string_view sv : requests ) {
			// format: #123 @ 3,2: 5x4
			sv.remove_prefix( );
			request_t tmp{};
			auto sv_tmp = sv.pop_front( sv.find_first_of( ' ' ) );
			tmp.id = daw::parser::parse_int<size_t>( sv_tmp );
			sv.remove_prefix( sv.find_first_of( '@' ) + 2 );
			sv_tmp = sv.pop_front( sv.find_first_of( ',' ) );
			tmp.left = daw::parser::parse_int<int32_t>( sv_tmp );
			sv.remove_prefix( );
			sv_tmp = sv.pop_front( sv.find_first_of( ':' ) );
			tmp.top = daw::parser::parse_int<int32_t>( sv_tmp );
			sv.remove_prefix( 2 );
			sv_tmp = sv.pop_front( sv.find_first_of( 'x' ) );
			tmp.width = daw::parser::parse_int<int32_t>( sv_tmp );
			sv.remove_prefix( );
			tmp.height = daw::parser::parse_int<int32_t>( sv );
			result.push_back( std::move( tmp ) );
		}
		return result;
	}

	template<typename T, intmax_t width, intmax_t height>
	struct fabric_t {
		std::vector<T> m_values = {};

		constexpr fabric_t( ) {
			m_values.resize( static_cast<size_t>( width * height ), T{} );
		}

		constexpr T &operator[]( size_t n ) noexcept {
			return m_values[n];
		}

		constexpr T const &operator[]( size_t n ) const noexcept {
			return m_values[n];
		}

		constexpr T &operator( )( intmax_t x, intmax_t y ) noexcept {
			return m_values[static_cast<size_t>( x ) +
			                static_cast<size_t>( y ) * width];
		}

		constexpr T const &operator( )( intmax_t x, intmax_t y ) const noexcept {
			return m_values[static_cast<size_t>( x ) +
			                static_cast<size_t>( y ) * width];
		}
	};

	template<typename Container>
	auto apply_reqs_to_fabric( Container &&requests ) {
		fabric_t<std::vector<size_t>, 1000, 1000> fabric{};
		for( auto const &req : requests ) {
			for( auto x = req.left; x < ( req.left + req.width ); ++x ) {
				for( auto y = req.top; y < ( req.top + req.height ); ++y ) {
					fabric( x, y ).push_back( req.id );
				}
			}
		}
		return fabric;
	}

	template<typename Container>
	intmax_t calc_conflicted_area( Container &&reqs ) {
		auto fabric = apply_reqs_to_fabric( reqs );
		intmax_t area = 0;
		for( intmax_t x = 0; x < 1000; ++x ) {
			for( intmax_t y = 0; y < 1000; ++y ) {
				if( fabric( x, y ).size( ) > 1 ) {
					++area;
				}
			}
		}
		return area;
	}

	template<typename Container>
	size_t find_unconflicted_area( Container &&reqs ) {
		auto fabric = apply_reqs_to_fabric( reqs );

		for( auto const &req : reqs ) {
			bool found = true;
			for( auto x = req.left; found and x < ( req.left + req.width ); ++x ) {
				for( auto y = req.top; y < ( req.top + req.height ); ++y ) {
					if( fabric( x, y ).size( ) != 1 ) {
						found = false;
						break;
					}
				}
			}
			if( found ) {
				return req.id;
			}
		}
		std::terminate( );
	}

} // namespace daw
