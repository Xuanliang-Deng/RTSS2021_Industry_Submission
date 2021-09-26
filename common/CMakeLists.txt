CMAKE_MINIMUM_REQUIRED(VERSION 3.13)
PROJECT(CommonTests LANGUAGES CXX)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
set(CMAKE_CXX_STANDARD 14)

add_executable(CommonTests common.cpp tests.cpp)

target_include_directories(CommonTests
	PRIVATE
	${GTEST_INCLUDE_DIRS}
)

target_link_libraries(CommonTests
	PRIVATE
	GTest::GTest
	Threads::Threads
)

target_compile_options(CommonTests PRIVATE -Wall -Werror)
