cmake_minimum_required(VERSION 3.10)
project( aoc_2018 )

set(CMAKE_CXX_STANDARD 17)

find_package( Boost 1.58.0 COMPONENTS system iostreams filesystem regex unit_test_framework REQUIRED )

if( ${CMAKE_CXX_COMPILER_ID} STREQUAL 'MSVC' )
	add_definitions( -DNOMINMAX -DD_WIN32_WINNT=0x0601 )
	add_compile_options( /W2 )
else( )
	if( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang" )
		add_compile_options( -Weverything -Wno-c++98-compat -Wno-covered-switch-default -Wno-padded -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -Wno-unused-parameter -Wno-missing-noreturn -Wno-missing-prototypes -Wno-disabled-macro-expansion -Wno-missing-braces -Wno-float-equal )
	elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
		add_compile_options( -Wall -Wno-deprecated-declarations -Wduplicated-cond -Wlogical-op -Wnull-dereference -Wold-style-cast -Wshadow)
	endif( )
endif( )

if( EXISTS "${CMAKE_SOURCE_DIR}/dependent_projects/build" )
	add_custom_target( dependency_stub )
	include_directories( SYSTEM "${CMAKE_SOURCE_DIR}/dependent_projects/build/install/include" )
	#	link_directories( "${CMAKE_SOURCE_DIR}/dependent_projects/build/install/lib" )
else( )
	set( SHOULD_BUILD_DEPS ON )
	include( "${CMAKE_SOURCE_DIR}/dependent_projects/dependencies.txt" )

	add_library( dependency_stub STATIC "${CMAKE_SOURCE_DIR}/dependent_projects/stub.cpp" )
	add_dependencies( dependency_stub header_libraries_prj )
include_directories( SYSTEM "${CMAKE_BINARY_DIR}/install/include" )
endif( )

set( HEADER_FOLDER "include" )
set( SOURCE_FOLDER "src" )
set( TEST_FOLDER "tests" )

include_directories( ${HEADER_FOLDER} )

include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )
link_directories( ${Boost_LIBRARY_DIRS} )
add_definitions( -DBOOST_TEST_DYN_LINK -DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK )

set( HEADER_FILES
	${HEADER_FOLDER}/day_01.h
)

set( SOURCE_FILES
	${SOURCE_FOLDER}/day_03.cpp
)

#add_library ( aoc_2018_lib STATIC ${HEADER_FILES} ${SOURCE_FILES} )
#add_dependencies( aoc_2018_lib dependency_stub )

add_executable( day_01_test ${HEADER_FILES} ${TEST_FOLDER}/day_01_test.cpp )
target_link_libraries( day_01_test ${Boost_LIBRARIES} ${CMAKE_DL_LIBS} ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT} )

add_executable( day_02_test ${HEADER_FILES} ${TEST_FOLDER}/day_02_test.cpp )
target_link_libraries( day_02_test ${Boost_LIBRARIES} ${CMAKE_DL_LIBS} ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT} )

add_executable( day_03_test ${HEADER_FILES} ${TEST_FOLDER}/day_03_test.cpp )
target_link_libraries( day_03_test ${Boost_LIBRARIES} ${CMAKE_DL_LIBS} ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT} )

