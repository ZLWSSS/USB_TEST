# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/lwchong/Sirius_git/USB_TEST

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lwchong/Sirius_git/USB_TEST/build

# Include any dependencies generated for this target.
include CMakeFiles/USBTEST.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/USBTEST.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/USBTEST.dir/flags.make

CMakeFiles/USBTEST.dir/main.cpp.o: CMakeFiles/USBTEST.dir/flags.make
CMakeFiles/USBTEST.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lwchong/Sirius_git/USB_TEST/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/USBTEST.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/USBTEST.dir/main.cpp.o -c /home/lwchong/Sirius_git/USB_TEST/main.cpp

CMakeFiles/USBTEST.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/USBTEST.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lwchong/Sirius_git/USB_TEST/main.cpp > CMakeFiles/USBTEST.dir/main.cpp.i

CMakeFiles/USBTEST.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/USBTEST.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lwchong/Sirius_git/USB_TEST/main.cpp -o CMakeFiles/USBTEST.dir/main.cpp.s

CMakeFiles/USBTEST.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/USBTEST.dir/main.cpp.o.requires

CMakeFiles/USBTEST.dir/main.cpp.o.provides: CMakeFiles/USBTEST.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/USBTEST.dir/build.make CMakeFiles/USBTEST.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/USBTEST.dir/main.cpp.o.provides

CMakeFiles/USBTEST.dir/main.cpp.o.provides.build: CMakeFiles/USBTEST.dir/main.cpp.o


CMakeFiles/USBTEST.dir/usb_f407.cpp.o: CMakeFiles/USBTEST.dir/flags.make
CMakeFiles/USBTEST.dir/usb_f407.cpp.o: ../usb_f407.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lwchong/Sirius_git/USB_TEST/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/USBTEST.dir/usb_f407.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/USBTEST.dir/usb_f407.cpp.o -c /home/lwchong/Sirius_git/USB_TEST/usb_f407.cpp

CMakeFiles/USBTEST.dir/usb_f407.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/USBTEST.dir/usb_f407.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lwchong/Sirius_git/USB_TEST/usb_f407.cpp > CMakeFiles/USBTEST.dir/usb_f407.cpp.i

CMakeFiles/USBTEST.dir/usb_f407.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/USBTEST.dir/usb_f407.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lwchong/Sirius_git/USB_TEST/usb_f407.cpp -o CMakeFiles/USBTEST.dir/usb_f407.cpp.s

CMakeFiles/USBTEST.dir/usb_f407.cpp.o.requires:

.PHONY : CMakeFiles/USBTEST.dir/usb_f407.cpp.o.requires

CMakeFiles/USBTEST.dir/usb_f407.cpp.o.provides: CMakeFiles/USBTEST.dir/usb_f407.cpp.o.requires
	$(MAKE) -f CMakeFiles/USBTEST.dir/build.make CMakeFiles/USBTEST.dir/usb_f407.cpp.o.provides.build
.PHONY : CMakeFiles/USBTEST.dir/usb_f407.cpp.o.provides

CMakeFiles/USBTEST.dir/usb_f407.cpp.o.provides.build: CMakeFiles/USBTEST.dir/usb_f407.cpp.o


# Object files for target USBTEST
USBTEST_OBJECTS = \
"CMakeFiles/USBTEST.dir/main.cpp.o" \
"CMakeFiles/USBTEST.dir/usb_f407.cpp.o"

# External object files for target USBTEST
USBTEST_EXTERNAL_OBJECTS =

USBTEST: CMakeFiles/USBTEST.dir/main.cpp.o
USBTEST: CMakeFiles/USBTEST.dir/usb_f407.cpp.o
USBTEST: CMakeFiles/USBTEST.dir/build.make
USBTEST: CMakeFiles/USBTEST.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lwchong/Sirius_git/USB_TEST/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable USBTEST"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/USBTEST.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/USBTEST.dir/build: USBTEST

.PHONY : CMakeFiles/USBTEST.dir/build

CMakeFiles/USBTEST.dir/requires: CMakeFiles/USBTEST.dir/main.cpp.o.requires
CMakeFiles/USBTEST.dir/requires: CMakeFiles/USBTEST.dir/usb_f407.cpp.o.requires

.PHONY : CMakeFiles/USBTEST.dir/requires

CMakeFiles/USBTEST.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/USBTEST.dir/cmake_clean.cmake
.PHONY : CMakeFiles/USBTEST.dir/clean

CMakeFiles/USBTEST.dir/depend:
	cd /home/lwchong/Sirius_git/USB_TEST/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lwchong/Sirius_git/USB_TEST /home/lwchong/Sirius_git/USB_TEST /home/lwchong/Sirius_git/USB_TEST/build /home/lwchong/Sirius_git/USB_TEST/build /home/lwchong/Sirius_git/USB_TEST/build/CMakeFiles/USBTEST.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/USBTEST.dir/depend

