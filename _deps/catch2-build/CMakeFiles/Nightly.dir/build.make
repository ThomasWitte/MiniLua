# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua

# Utility rule file for Nightly.

# Include the progress variables for this target.
include _deps/catch2-build/CMakeFiles/Nightly.dir/progress.make

_deps/catch2-build/CMakeFiles/Nightly:
	cd /home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps/catch2-build && /usr/bin/ctest -D Nightly

Nightly: _deps/catch2-build/CMakeFiles/Nightly
Nightly: _deps/catch2-build/CMakeFiles/Nightly.dir/build.make

.PHONY : Nightly

# Rule to build all files generated by this target.
_deps/catch2-build/CMakeFiles/Nightly.dir/build: Nightly

.PHONY : _deps/catch2-build/CMakeFiles/Nightly.dir/build

_deps/catch2-build/CMakeFiles/Nightly.dir/clean:
	cd /home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps/catch2-build && $(CMAKE_COMMAND) -P CMakeFiles/Nightly.dir/cmake_clean.cmake
.PHONY : _deps/catch2-build/CMakeFiles/Nightly.dir/clean

_deps/catch2-build/CMakeFiles/Nightly.dir/depend:
	cd /home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua /home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps/catch2-src /home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua /home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps/catch2-build /home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps/catch2-build/CMakeFiles/Nightly.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : _deps/catch2-build/CMakeFiles/Nightly.dir/depend

