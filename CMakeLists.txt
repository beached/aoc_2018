cmake_minimum_required(VERSION 3.10)
project( aoc_2018 )

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

#add_library ( aoc_2018_lib STATIC ${HEADER_FILES} ${SOURCE_FILES} )
#add_dependencies( aoc_2018_lib dependency_stub )

add_executable( day_01_test ${HEADER_FOLDER}/day_01.h ${HEADER_FILES} ${TEST_FOLDER}/day_01_test.cpp )
target_link_libraries( day_01_test ${Boost_LIBRARIES} ${CMAKE_DL_LIBS} ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( day_01_test dependency_stub )

add_executable( day_02_test ${HEADER_FOLDER}/day_02.h ${HEADER_FILES} ${TEST_FOLDER}/day_02_test.cpp )
target_link_libraries( day_02_test ${CMAKE_DL_LIBS} ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( day_02_test dependency_stub )

add_executable( day_03_test ${HEADER_FOLDER}/day_03.h ${HEADER_FILES} ${TEST_FOLDER}/day_03_test.cpp )
target_link_libraries( day_03_test ${CMAKE_DL_LIBS} ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( day_03_test dependency_stub )

add_executable( day_04_test ${HEADER_FOLDER}/day_04.h ${HEADER_FILES} ${TEST_FOLDER}/day_04_test.cpp )
target_link_libraries( day_04_test ${CMAKE_DL_LIBS} ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( day_04_test dependency_stub )

add_executable( day_05_test ${HEADER_FOLDER}/day_05.h ${HEADER_FILES} ${TEST_FOLDER}/day_05_test.cpp )
target_link_libraries( day_05_test ${CMAKE_DL_LIBS} ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( day_05_test dependency_stub )

add_executable( day_06_test ${HEADER_FOLDER}/day_06.h ${HEADER_FILES} ${TEST_FOLDER}/day_06_test.cpp )
target_link_libraries( day_06_test ${CMAKE_DL_LIBS} ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( day_06_test dependency_stub )

add_executable( day_07_test ${HEADER_FOLDER}/day_07.h ${HEADER_FILES} ${TEST_FOLDER}/day_07_test.cpp )
target_link_libraries( day_07_test ${CMAKE_DL_LIBS} ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( day_07_test dependency_stub )

add_executable( day_08_test ${HEADER_FOLDER}/day_08.h ${HEADER_FILES} ${TEST_FOLDER}/day_08_test.cpp )
target_link_libraries( day_08_test ${CMAKE_DL_LIBS} ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( day_08_test dependency_stub )

add_executable( day_09_test ${HEADER_FOLDER}/day_09.h ${HEADER_FILES} ${TEST_FOLDER}/day_09_test.cpp )
target_link_libraries( day_09_test ${CMAKE_DL_LIBS} ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( day_09_test dependency_stub )

add_executable( day_10_test ${HEADER_FOLDER}/day_10.h ${HEADER_FILES} ${TEST_FOLDER}/day_10_test.cpp )
target_link_libraries( day_10_test ${CMAKE_DL_LIBS} ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( day_10_test dependency_stub )

add_executable( day_11_test ${HEADER_FOLDER}/day_11.h ${HEADER_FILES} ${TEST_FOLDER}/day_11_test.cpp )
target_link_libraries( day_11_test task_scheduler_lib ${CMAKE_DL_LIBS} ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( day_11_test dependency_stub )

add_executable( day_12_test ${HEADER_FOLDER}/day_11.h ${HEADER_FILES} ${TEST_FOLDER}/day_11_test.cpp )
target_link_libraries( day_12_test task_scheduler_lib ${CMAKE_DL_LIBS} ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( day_12_test dependency_stub )

