# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /snap/clion/67/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/67/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chaperone/projects/cpp/CarbonCopySignatureMatching

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chaperone/projects/cpp/CarbonCopySignatureMatching/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ccsm-track.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ccsm-track.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ccsm-track.dir/flags.make

CMakeFiles/ccsm-track.dir/ccsm-track.cpp.o: CMakeFiles/ccsm-track.dir/flags.make
CMakeFiles/ccsm-track.dir/ccsm-track.cpp.o: ../ccsm-track.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chaperone/projects/cpp/CarbonCopySignatureMatching/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ccsm-track.dir/ccsm-track.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ccsm-track.dir/ccsm-track.cpp.o -c /home/chaperone/projects/cpp/CarbonCopySignatureMatching/ccsm-track.cpp

CMakeFiles/ccsm-track.dir/ccsm-track.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ccsm-track.dir/ccsm-track.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chaperone/projects/cpp/CarbonCopySignatureMatching/ccsm-track.cpp > CMakeFiles/ccsm-track.dir/ccsm-track.cpp.i

CMakeFiles/ccsm-track.dir/ccsm-track.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ccsm-track.dir/ccsm-track.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chaperone/projects/cpp/CarbonCopySignatureMatching/ccsm-track.cpp -o CMakeFiles/ccsm-track.dir/ccsm-track.cpp.s

CMakeFiles/ccsm-track.dir/TrackPad.cpp.o: CMakeFiles/ccsm-track.dir/flags.make
CMakeFiles/ccsm-track.dir/TrackPad.cpp.o: ../TrackPad.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chaperone/projects/cpp/CarbonCopySignatureMatching/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ccsm-track.dir/TrackPad.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ccsm-track.dir/TrackPad.cpp.o -c /home/chaperone/projects/cpp/CarbonCopySignatureMatching/TrackPad.cpp

CMakeFiles/ccsm-track.dir/TrackPad.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ccsm-track.dir/TrackPad.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chaperone/projects/cpp/CarbonCopySignatureMatching/TrackPad.cpp > CMakeFiles/ccsm-track.dir/TrackPad.cpp.i

CMakeFiles/ccsm-track.dir/TrackPad.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ccsm-track.dir/TrackPad.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chaperone/projects/cpp/CarbonCopySignatureMatching/TrackPad.cpp -o CMakeFiles/ccsm-track.dir/TrackPad.cpp.s

# Object files for target ccsm-track
ccsm__track_OBJECTS = \
"CMakeFiles/ccsm-track.dir/ccsm-track.cpp.o" \
"CMakeFiles/ccsm-track.dir/TrackPad.cpp.o"

# External object files for target ccsm-track
ccsm__track_EXTERNAL_OBJECTS =

ccsm-track: CMakeFiles/ccsm-track.dir/ccsm-track.cpp.o
ccsm-track: CMakeFiles/ccsm-track.dir/TrackPad.cpp.o
ccsm-track: CMakeFiles/ccsm-track.dir/build.make
ccsm-track: /usr/lib/x86_64-linux-gnu/libsfml-system.so
ccsm-track: /usr/lib/x86_64-linux-gnu/libsfml-window.so
ccsm-track: /usr/lib/x86_64-linux-gnu/libsfml-graphics.so
ccsm-track: /usr/lib/x86_64-linux-gnu/libsfml-network.so
ccsm-track: /usr/lib/x86_64-linux-gnu/libsfml-audio.so
ccsm-track: CMakeFiles/ccsm-track.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chaperone/projects/cpp/CarbonCopySignatureMatching/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ccsm-track"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ccsm-track.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ccsm-track.dir/build: ccsm-track

.PHONY : CMakeFiles/ccsm-track.dir/build

CMakeFiles/ccsm-track.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ccsm-track.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ccsm-track.dir/clean

CMakeFiles/ccsm-track.dir/depend:
	cd /home/chaperone/projects/cpp/CarbonCopySignatureMatching/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chaperone/projects/cpp/CarbonCopySignatureMatching /home/chaperone/projects/cpp/CarbonCopySignatureMatching /home/chaperone/projects/cpp/CarbonCopySignatureMatching/cmake-build-debug /home/chaperone/projects/cpp/CarbonCopySignatureMatching/cmake-build-debug /home/chaperone/projects/cpp/CarbonCopySignatureMatching/cmake-build-debug/CMakeFiles/ccsm-track.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ccsm-track.dir/depend

