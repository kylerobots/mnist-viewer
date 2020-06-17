cmake_minimum_required(VERSION 3.5)

project(mnist-viewer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Torch REQUIRED)

add_executable(mnist-viewer
	main.cpp
	mainwindow.cpp
	mainwindow.h
	mainwindow.ui
)
target_link_libraries(mnist-viewer PRIVATE Qt5::Widgets ${TORCH_LIBRARIES})
