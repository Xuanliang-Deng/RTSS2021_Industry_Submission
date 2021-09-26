CMAKE_MINIMUM_REQUIRED(VERSION 3.13)
PROJECT(TaskTests LANGUAGES CXX)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
set(CMAKE_CXX_STANDARD 14)

add_executable(TaskTests common.cpp subtask.cpp task.cpp taskset.cpp vPath.cpp tests.cpp platform.cpp processor.cpp bbNode.cpp BnB.cpp)

target_include_directories(TaskTests
	PRIVATE
	${GTEST_INCLUDE_DIRS}
)

target_link_libraries(TaskTests
	PRIVATE
	GTest::GTest
	Threads::Threads
)

target_compile_options(TaskTests PRIVATE -Wall -Werror)
