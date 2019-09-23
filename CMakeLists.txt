cmake_minimum_required(VERSION 3.10)
project( aoc_2015 )

find_package( Boost 1.58.0 COMPONENTS system iostreams filesystem regex unit_test_framework REQUIRED )
find_package( Threads )
include( "${CMAKE_SOURCE_DIR}/dependent_projects/CMakeListsCompiler.txt" )

add_definitions( -DBOOST_TEST_DYN_LINK -DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK )

if( EXISTS "${CMAKE_SOURCE_DIR}/dependent_projects/build" )
	add_custom_target( dependency_stub )
	include_directories( SYSTEM "${CMAKE_SOURCE_DIR}/dependent_projects/build/install/include" )
	link_directories( "${CMAKE_SOURCE_DIR}/dependent_projects/build/install/lib" )
else( )
	set( SHOULD_BUILD_DEPS ON )
	include( "${CMAKE_SOURCE_DIR}/dependent_projects/dependencies.txt" )

	add_library( dependency_stub STATIC "${CMAKE_SOURCE_DIR}/dependent_projects/stub.cpp" )
 	add_dependencies( dependency_stub function_stream_prj header_libraries_prj date_prj )
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
)

set( SOURCE_FILES
#	${SOURCE_FOLDER}/day_03.cpp
)

#add_library ( aoc_2015_lib STATIC ${HEADER_FILES} ${SOURCE_FILES} )
#add_dependencies( aoc_2015_lib dependency_stub )

add_executable( day_09_test ${HEADER_FOLDER}/day_09.h ${HEADER_FILES} ${TEST_FOLDER}/day_09_test.cpp )
target_link_libraries( day_09_test ${CMAKE_DL_LIBS} ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( day_09_test dependency_stub )

