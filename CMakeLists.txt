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

enable_testing()
find_package(Qt5Test REQUIRED)

add_executable(TestMainWindow
	test_mainwindow.cpp
	mainwindow.cpp
)
add_test(NAME TestMainWindow COMMAND TestMainWindow)
target_link_libraries(TestMainWindow PRIVATE Qt5::Gui Qt5::Test Qt5::Widgets ${TORCH_LIBRARIES})

add_executable(TestNetwork
	test_network.cpp
)
add_test(NAME TestNetwork COMMAND TestNetwork)
target_link_libraries(TestNetwork PRIVATE Qt5::Test ${TORCH_LIBRARIES})
