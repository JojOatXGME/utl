cmake_minimum_required(VERSION 3.1)
project( "Utility Tool Library" )

set(LIBNAME "utl")
set(VERSION_MAJOR 0 CACHE STRING "Project major version number.")
set(VERSION_MINOR 1 CACHE STRING "Project minor version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR)

## Change default build type to 'Debug'
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)

## Create targets in subdirectories
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "lib")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "lib")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

## Load packages for cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(Doxygen)
find_package(GTest)

## Create some variables
file(GLOB_RECURSE SOURCE_FILES
	"src/*.cpp")
file(GLOB_RECURSE HEADER_FILES
	"include/utl/*.h" "include/utl/*.hpp")
file(GLOB_RECURSE UTEST_FILES
	"test/*.cpp")

## Create source groups (for Visual Studio)
source_group("Headers"    FILES ${HEADER_FILES})
source_group("Sources"    FILES ${SOURCE_FILES})
source_group("Unit-Tests" FILES ${UTEST_FILES})

## Use C++11
set(CMAKE_LINKER_LANGUAGE CXX)
set(CMAKE_CXX_STANDARD 11)

## Add targets
add_library("${LIBNAME}" ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories("${LIBNAME}" PUBLIC "include")

## Create header with build information
configure_file(
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/include/utl/config.h" ESCAPE_QUOTES)
target_include_directories("${LIBNAME}" PUBLIC "${PROJECT_BINARY_DIR}/include")

## Add rule for Doxygen
option(DOC_TARGET
	"Add a target to create the HTML based documentation (requires Doxygen)"
	${DOXYGEN_FOUND})

if (DOC_TARGET)
	if (NOT DOXYGEN_FOUND)
		message(FATAL_ERROR "Doxygen not found")
	endif()

	configure_file(
		"${PROJECT_SOURCE_DIR}/Doxyfile.in"
		"${PROJECT_BINARY_DIR}/Doxyfile" ESCAPE_QUOTES @ONLY)
	add_custom_target(doc
		"${DOXYGEN_EXECUTABLE}" "${PROJECT_BINARY_DIR}/Doxyfile"
		WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
		COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif()

## Add unit tests
option(UNIT_TEST
	"Build unit tests (requires GTest)"
	${GTEST_FOUND})

if (UNIT_TEST)
	if (NOT GTEST_FOUND)
		message(FATAL_ERROR "GTest not found")
	endif()

	enable_testing()
	add_executable("utl_utests" ${UTEST_FILES})
	target_include_directories("utl_utests" PRIVATE "${GTEST_INCLUDE_DIRS}")
	target_link_libraries("utl_utests" "${LIBNAME}" ${GTEST_BOTH_LIBRARIES})
	foreach (TEST_FILE ${UTEST_FILES})
		get_filename_component(TNAME "${TEST_FILE}" NAME_WE)
		add_test("${TNAME}" "utl_utests" "--gtest_filter=${TNAME}.*")
	endforeach()
endif()
