# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/mok/바탕화면/llvm/bogo/TxRace_mpx/TxHooks

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mok/바탕화면/llvm/bogo/TxRace_mpx/TxHooks/build

# Include any dependencies generated for this target.
include CMakeFiles/TxHooks.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TxHooks.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TxHooks.dir/flags.make

CMakeFiles/TxHooks.dir/TxHooks.o: CMakeFiles/TxHooks.dir/flags.make
CMakeFiles/TxHooks.dir/TxHooks.o: ../TxHooks.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mok/바탕화면/llvm/bogo/TxRace_mpx/TxHooks/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TxHooks.dir/TxHooks.o"
	clang  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TxHooks.dir/TxHooks.o -c /home/mok/바탕화면/llvm/bogo/TxRace_mpx/TxHooks/TxHooks.cpp

CMakeFiles/TxHooks.dir/TxHooks.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TxHooks.dir/TxHooks.i"
	clang $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mok/바탕화면/llvm/bogo/TxRace_mpx/TxHooks/TxHooks.cpp > CMakeFiles/TxHooks.dir/TxHooks.i

CMakeFiles/TxHooks.dir/TxHooks.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TxHooks.dir/TxHooks.s"
	clang $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mok/바탕화면/llvm/bogo/TxRace_mpx/TxHooks/TxHooks.cpp -o CMakeFiles/TxHooks.dir/TxHooks.s

CMakeFiles/TxHooks.dir/TxHooks.o.requires:

.PHONY : CMakeFiles/TxHooks.dir/TxHooks.o.requires

CMakeFiles/TxHooks.dir/TxHooks.o.provides: CMakeFiles/TxHooks.dir/TxHooks.o.requires
	$(MAKE) -f CMakeFiles/TxHooks.dir/build.make CMakeFiles/TxHooks.dir/TxHooks.o.provides.build
.PHONY : CMakeFiles/TxHooks.dir/TxHooks.o.provides

CMakeFiles/TxHooks.dir/TxHooks.o.provides.build: CMakeFiles/TxHooks.dir/TxHooks.o


# Object files for target TxHooks
TxHooks_OBJECTS = \
"CMakeFiles/TxHooks.dir/TxHooks.o"

# External object files for target TxHooks
TxHooks_EXTERNAL_OBJECTS =

libTxHooks.a: CMakeFiles/TxHooks.dir/TxHooks.o
libTxHooks.a: CMakeFiles/TxHooks.dir/build.make
libTxHooks.a: CMakeFiles/TxHooks.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mok/바탕화면/llvm/bogo/TxRace_mpx/TxHooks/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libTxHooks.a"
	$(CMAKE_COMMAND) -P CMakeFiles/TxHooks.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TxHooks.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TxHooks.dir/build: libTxHooks.a

.PHONY : CMakeFiles/TxHooks.dir/build

CMakeFiles/TxHooks.dir/requires: CMakeFiles/TxHooks.dir/TxHooks.o.requires

.PHONY : CMakeFiles/TxHooks.dir/requires

CMakeFiles/TxHooks.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TxHooks.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TxHooks.dir/clean

CMakeFiles/TxHooks.dir/depend:
	cd /home/mok/바탕화면/llvm/bogo/TxRace_mpx/TxHooks/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mok/바탕화면/llvm/bogo/TxRace_mpx/TxHooks /home/mok/바탕화면/llvm/bogo/TxRace_mpx/TxHooks /home/mok/바탕화면/llvm/bogo/TxRace_mpx/TxHooks/build /home/mok/바탕화면/llvm/bogo/TxRace_mpx/TxHooks/build /home/mok/바탕화면/llvm/bogo/TxRace_mpx/TxHooks/build/CMakeFiles/TxHooks.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TxHooks.dir/depend

