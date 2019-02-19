cmake_minimum_required(VERSION 3.6)
cmake_policy(SET CMP0048 NEW)
message("CMAKE: ${CMAKE_VERSION}")
message("Generator: ${CMAKE_GENERATOR}")
message("SYSTEM NAME: ${CMAKE_SYSTEM_NAME}")
message("HOST SYSTEM: '${CMAKE_HOST_SYSTEM_NAME}'")

project(qt_tests VERSION 0.0.1 LANGUAGES CXX)

###########################################################
#   Setup variables for the build process
###########################################################
set(EXECUTABLE "qt_tests")      # Name of the executable
set(LIB_MOD "STATIC")       # options ${LIB_MOD} | STATIC

set(FINAL_SUMMARY OFF)
set(PROJ_SRC "${CMAKE_SOURCE_DIR}")
set(PROJ_DEP "${CMAKE_SOURCE_DIR}/external")        # 3rd party libraries folder
set(PROJ_LIB "${PROJ_SRC}/lib")                     # Libraries created in the project
set(PROJ_DEP_LIB "${PROJ_DEP}/lib")                 # Libraries in the libs

set(PROJ_LIBS "")           # List of internal libs to be included
set(FRAME_LIBS "")          # List of framework libs to be included
set(TEST_LIBS "")           # List of test libs to be included
set(DEPEND_LIBS "")         # List of external libs to be included
set(DEPEND_DIRS "")         # List of external directories to be included

set(SYS_HDR_PATHS "")       # Will contain the path to all the .h files in the main code
set(TST_HDR_PATHS "")       # Will contain the path to all the .h files in the tests
set(EXT_HDR_PATHS "")       # Will contain the path to all the .h files in the external sources

set(COMPILER_FLAGS "")      # Will contain compiling flags
set(LINKER_FLAGS "")        # Will contain the flags for the linker

#   Flags for logging the build process
set(COMP_ERRS OFF)
set(COMP_ERRS_LOG "")


if (NOT WIN32)
    set(CMAKE_MODULE_PATH /usr/local/lib/cmake /usr/local/lib/x86_64-linux-gnu/cmake)
    set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw)
endif (NOT WIN32)
###########################################################
#   Setup OpenGL
###########################################################
find_package (PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)

find_package(Qt5Core)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

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
set(COMPILER_FLAGS "-std=c++11 -pthread")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS})
###########################################################
#   Add headers and libraries from libs
###########################################################
if (WIN32)
    list(APPEND EXT_HDR_PATHS "${PROJ_DEP}/glew-2.1.0/include")
    list(APPEND EXT_HDR_PATHS "${PROJ_DEP}/glfw-3.2.1/include")
    list(APPEND EXT_HDR_PATHS "${PROJ_DEP}/glm-0.9.6.3")
    #include_directories("C:/GLFW/include" "C:/glm" "C:/glew/include")
    list(APPEND DEPEND_DIRS "${PROJ_DEP}/glew-2.1.0/lib/Release/x64/")
    #list(APPEND DEPEND_DIRS "${PROJ_DEP}/glew-2.1.0/bin/Release/x64/")
    list(APPEND DEPEND_DIRS "${PROJ_DEP}/glfw-3.2.1/lib-mingw-w64/")
    link_directories(${DEPEND_DIRS})
    #link_directories("C:/glew/lib/Release/x64" "C:/GLFW/lib-mingw")
else (WIN32)
    find_package (glfw3 REQUIRED)
    find_package (GLM REQUIRED)
    find_package (GLEW REQUIRED STATIC)
endif (WIN32)
if (APPLE)
    include_directories(/usr/local/include)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVID_LIBRARY CoreVideo REQUIRED)
    list(APPEND FRAME_LIBS ${COCOA_LIBRARY})
    list(APPEND FRAME_LIBS ${COREVID_LIBRARY})
    list(APPEND FRAME_LIBS ${IOKIT_LIBRARY})
endif (APPLE)

# how to add headers
CHECK_INCLUDES(EXT_HDR_PATHS)
INCLUDE_DIRS(EXT_HDR_PATHS)

# How to add libraries
#list(APPEND DEPEND_LIBS "flapack")
#list(APPEND DEPEND_LIBS "fblas")
CHECK_LIBRARIES(DEPEND_LIBS)
IMPORT_LIBS(${PROJ_DEP_LIB} DEPEND_LIBS)

list(APPEND FRAME_LIBS ${OPENGL_LIBRARIES})
list(APPEND FRAME_LIBS ${GLFW3_LIBRARY})
list(APPEND FRAME_LIBS ${GLEW_LIBRARY})
if (WIN32)
    list(APPEND FRAME_LIBS glfw3)
    list(APPEND FRAME_LIBS glew32s)
    list(APPEND FRAME_LIBS glu32)
    list(APPEND FRAME_LIBS opengl32)
    #target_link_libraries (sample ${OPENGL_LIBRARIES} glfw3 glew32s glu32 opengl32)
endif (WIN32)
if (UNIX)
    list(APPEND FRAME_LIBS Xxf86vm)
    list(APPEND FRAME_LIBS pthread)
    list(APPEND FRAME_LIBS Xrandr)
    list(APPEND FRAME_LIBS Xinerama)
    list(APPEND FRAME_LIBS Xi)
    list(APPEND FRAME_LIBS Xcursor)
    #target_link_libraries (sample Xxf86vm pthread Xrandr Xinerama Xi Xcursor)
endif (UNIX)
###########################################################
#   Setup QT for the GUI
###########################################################
#set(CMAKE_PREFIX_PATH "/opt/Qt5.7.0/5.7/gcc_64/")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set up a mapping so that the Release configuration for the Qt imported target is
# used in the COVERAGE CMake configuration.
#set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

add_definitions(-DGLEW_STATIC)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
###########################################################
#   Add the sources of the system
###########################################################
ADD_SUBDIR(src)

