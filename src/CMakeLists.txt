#set(core_SRCS egabi.cpp)

# Generate the list of folders containing .h files
HEADER_DIRECTORIES(SYS_HDR_PATHS)
#ADD_SUBDIR(<Directory>) # remember to use the core_SRCS to add sources
ADD_SOURCES(core_SRCS OFF)
ADD_SUBDIR(gui)

HEADER_FILES(QT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/gui")
#message("headers: ${QT_HEADERS}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

###########################################################
#   Find the .ui files from the whole project
###########################################################
FORM_FILES(QT_FORMS)

###########################################################
#   Find the .qrc files from the whole project
###########################################################
#RESOURCE_FILES(QT_RESOURCES)
#set(QT_RESOURCES "")

###########################################################
#   Add the qt libraries for the linking process
###########################################################
list(APPEND FRAME_LIBS Qt5::Core)
list(APPEND FRAME_LIBS Qt5::Gui)
list(APPEND FRAME_LIBS Qt5::Widgets)

###########################################################
#   Generate additional sources with MOC and UIC
###########################################################
qt5_wrap_cpp(MOC_HEADERS ${QT_HEADERS})
qt5_wrap_ui(UIC_SOURCES ${QT_FORMS})
qt5_add_resources(RCC_SOURCES ${QT_RESOURCES})

include_directories(${SYS_HDR_PATHS})
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

#qt5_use_modules(${EXECUTABLE} Core Widgets Gui)

add_library (${EXECUTABLE}lib STATIC ${core_SRCS} ${MOC_HEADERS} ${UIC_SOURCES})
LINK_LIBS(${EXECUTABLE}lib FRAME_LIBS)

set(proj_sources ${core_SRCS} ${MOC_HEADERS} ${UIC_SOURCES} ${RCC_SOURCES})

add_executable(${EXECUTABLE} WIN32 ${proj_sources})
LINK_LIBS(${EXECUTABLE} FRAME_LIBS)
