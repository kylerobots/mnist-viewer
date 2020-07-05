cmake_minimum_required(VERSION 3.5)

project(mnist-viewer VERSION 1.0.0.0 LANGUAGES CXX)

# These are all needed throughout the project, so are set globally.
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(torch REQUIRED)

# A library for all the GUI elements
add_library(gui
	src/mainwindow.cpp
	src/trainingprogress.cpp
	# These aren't necessary, but are done to get them to show up in the IDE.
	include/mainwindow.h
	include/trainingprogress.h
	ui/mainwindow.ui
	ui/trainingprogress.ui
)
target_include_directories(gui
	PUBLIC
	include
)
target_link_libraries(gui
	PRIVATE
	Qt5::Widgets
)

# A library for the network backend.
add_library(network
	src/network.cpp
	include/network.h
)
target_include_directories(network
	PUBLIC
	include
)
target_link_libraries(network
	PUBLIC
	${TORCH_LIBRARIES}
	PRIVATE
	Qt5::Widgets
)

# The main function that brings everything together.
add_executable(mnist-viewer
	src/main.cpp
)
target_link_libraries(mnist-viewer
	PRIVATE
	gui
	network
	Qt5::Widgets
)

# Unit tests
enable_testing()
find_package(Qt5Test REQUIRED)

add_executable(test-gui
	test/test_mainwindow.cpp
)
target_link_libraries(test-gui
	PRIVATE
	gui
	Qt5::Test
	Qt5::Gui
	Qt5::Widgets
)
add_test(NAME test-gui COMMAND test-gui)

add_executable(test-network
	test/test_network.cpp
)
target_link_libraries(test-network
	PRIVATE
	network
	Qt5::Test
	Qt5::Gui
	Qt5::Widgets
)
add_test(NAME test-network COMMAND test-network)
