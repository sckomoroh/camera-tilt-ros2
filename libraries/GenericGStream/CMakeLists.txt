cmake_minimum_required(VERSION 3.8)

project(nnpackage)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GST REQUIRED gstreamer-1.0)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)
pkg_check_modules(JSON_GLIB REQUIRED json-glib-1.0)
pkg_check_modules(GST_BASE REQUIRED gstreamer-base-1.0)
pkg_check_modules(GIO REQUIRED gio-2.0)
pkg_check_modules(GIO_UNIX REQUIRED gio-unix-2.0)

find_package(yaml-cpp REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(
    include
    src
    ${GST_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}

    ${JSON_GLIB_INCLUDE_DIRS}
)

add_library(nnconvert SHARED
    src/convert/Convert.cpp
)
add_library(nninfer SHARED
    src/infer/Infer.cpp
)
add_library(nnoverlay SHARED
    src/overlay/Overlay.cpp
)
add_library(nntracking SHARED
    src/tracking/Tracking.cpp
)

target_link_libraries(nnconvert
    ${GST_LIBRARIES}
    ${GST_BASE_LIBRARIES}
    ${OpenCV_LIBS}
)

target_link_libraries(nninfer
    ${GST_LIBRARIES}
    ${GST_BASE_LIBRARIES}
    ${OpenCV_LIBS}
)

target_link_libraries(nnoverlay
    ${GST_LIBRARIES}
    ${GST_BASE_LIBRARIES}
    ${OpenCV_LIBS}
)

target_link_libraries(nntracking
    ${GST_LIBRARIES}
    ${GST_BASE_LIBRARIES}
    ${OpenCV_LIBS}
)
