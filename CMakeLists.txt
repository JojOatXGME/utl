cmake_minimum_required(VERSION 3.1)
project( utl )

#add_subdirectory(test)

#SET(CMAKE_BUILD_TYPE "None")
#SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_BUILD_TYPE "Release")

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "lib")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "lib")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
endif()

# Create some variables
set(INCLUDE_DIR "include/")
set(SOURCE_DIR "src/")

aux_source_directory("${SOURCE_DIR}" SOURCE_FILES)
file(GLOB HEADER_FILES
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	"include/utl/*.h" "include/utl/*.hpp"
)

# Create source groups (for Visual Studio)
source_group("Headers" FILES ${HEADER_FILES})
source_group("Sources" FILES ${SOURCE_FILES})

# Add binaries
add_library("${PROJECT_NAME}" ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories("${PROJECT_NAME}" PUBLIC "${INCLUDE_DIR}")

# Use C++11
set_target_properties("${PROJECT_NAME}" PROPERTIES CXX_STANDARD 11)
