# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common

# Include any dependencies generated for this target.
include CMakeFiles/CommonTests.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CommonTests.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CommonTests.dir/flags.make

CMakeFiles/CommonTests.dir/common.cpp.o: CMakeFiles/CommonTests.dir/flags.make
CMakeFiles/CommonTests.dir/common.cpp.o: common.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CommonTests.dir/common.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CommonTests.dir/common.cpp.o -c /home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common/common.cpp

CMakeFiles/CommonTests.dir/common.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CommonTests.dir/common.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common/common.cpp > CMakeFiles/CommonTests.dir/common.cpp.i

CMakeFiles/CommonTests.dir/common.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CommonTests.dir/common.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common/common.cpp -o CMakeFiles/CommonTests.dir/common.cpp.s

CMakeFiles/CommonTests.dir/tests.cpp.o: CMakeFiles/CommonTests.dir/flags.make
CMakeFiles/CommonTests.dir/tests.cpp.o: tests.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/CommonTests.dir/tests.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CommonTests.dir/tests.cpp.o -c /home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common/tests.cpp

CMakeFiles/CommonTests.dir/tests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CommonTests.dir/tests.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common/tests.cpp > CMakeFiles/CommonTests.dir/tests.cpp.i

CMakeFiles/CommonTests.dir/tests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CommonTests.dir/tests.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common/tests.cpp -o CMakeFiles/CommonTests.dir/tests.cpp.s

# Object files for target CommonTests
CommonTests_OBJECTS = \
"CMakeFiles/CommonTests.dir/common.cpp.o" \
"CMakeFiles/CommonTests.dir/tests.cpp.o"

# External object files for target CommonTests
CommonTests_EXTERNAL_OBJECTS =

CommonTests: CMakeFiles/CommonTests.dir/common.cpp.o
CommonTests: CMakeFiles/CommonTests.dir/tests.cpp.o
CommonTests: CMakeFiles/CommonTests.dir/build.make
CommonTests: /usr/local/lib/libgtest.a
CommonTests: CMakeFiles/CommonTests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable CommonTests"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CommonTests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CommonTests.dir/build: CommonTests

.PHONY : CMakeFiles/CommonTests.dir/build

CMakeFiles/CommonTests.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CommonTests.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CommonTests.dir/clean

CMakeFiles/CommonTests.dir/depend:
	cd /home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common /home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common /home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common /home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common /home/xuanliang/Fall2021/CPS-CAA/SBH-DAG/common/CMakeFiles/CommonTests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CommonTests.dir/depend

