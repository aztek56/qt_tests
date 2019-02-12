cmake_minimum_required(VERSION 3.6)
cmake_policy(SET CMP0048 NEW)
message("CMAKE: ${CMAKE_VERSION}")
message("SYSTEM NAME: ${CMAKE_SYSTEM_NAME}")
message("HOST SYSTEM: '${CMAKE_HOST_SYSTEM_NAME}'")

project(qt_tests VERSION 0.0.1 LANGUAGES CXX)
set ( CMAKE_PREFIX_PATH "C:\\Qt\\5.11.0\\mingw53_32\\" )
#set ( CMAKE_PREFIX_PATH "C:\\Qt\\5.11.0\\mingw53_32\\lib\\cmake" )

###########################################################
#   Setup variables for the build process
###########################################################
set(EXECUTABLE "qt_tests")      # Name of the executable
set(LIB_MOD "STATIC")       # options ${LIB_MOD} | STATIC

set(FINAL_SUMMARY OFF)
set(PROJ_SRC "${CMAKE_SOURCE_DIR}")
set(PROJ_DEP "${CMAKE_SOURCE_DIR}/dependencies")    # 3rd party libraries folder
set(PROJ_LIB "${PROJ_SRC}/lib")                     # Libraries created in the project
set(PROJ_DEP_LIB "${PROJ_DEP}/lib")                 # Libraries in the libs

set(PROJ_LIBS "")           # List of internal libs to be included
set(FRAME_LIBS "")          # List of framework libs to be included
set(TEST_LIBS "")           # List of test libs to be included
set(DEPEND_LIBS "")         # List of external libs to be included

set(SYS_HDR_PATHS "")       # Will contain the path to all the .h files in the main code
set(TST_HDR_PATHS "")       # Will contain the path to all the .h files in the tests
set(EXT_HDR_PATHS "")       # Will contain the path to all the .h files in the external sources

set(COMPILER_FLAGS "")      # Will contain compiling flags
set(LINKER_FLAGS "")        # Will contain the flags for the linker

#   Flags for logging the build process
set(COMP_ERRS OFF)
set(COMP_ERRS_LOG "")

###########################################################
#   Load build tools
###########################################################
set(BUILD_TOOLS ${CMAKE_SOURCE_DIR}/build-tools)
include(${BUILD_TOOLS}/modules.cmake)   # Load modules
# Add the modules required by the make process
list(APPEND CMAKE_MODULE_PATH "${BUILD_TOOLS}")

###########################################################
#   Setup compilation flags
###########################################################
set(CMAKE_CXX_STANDARD 14)

###########################################################
#   Add headers and libraries from libs
###########################################################
# how to add headers
# list(APPEND EXT_HDR_PATHS "${PROJ_DEP}/petsc-3.6.0/include")
CHECK_INCLUDES(EXT_HDR_PATHS)
INCLUDE_DIRS(EXT_HDR_PATHS)

# How to add libraries
#list(APPEND DEPEND_LIBS "flapack")
#list(APPEND DEPEND_LIBS "fblas")
CHECK_LIBRARIES(DEPEND_LIBS)
IMPORT_LIBS(${PROJ_DEP_LIB} DEPEND_LIBS)

###########################################################
#   Setup QT for the GUI
###########################################################
#set(CMAKE_PREFIX_PATH "/opt/Qt5.7.0/5.7/gcc_64/")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# set up a mapping so that the Release configuration for the Qt imported target is
# used in the COVERAGE CMake configuration.
#set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
###########################################################
#   Add the sources of the system
###########################################################
ADD_SUBDIR(src)

