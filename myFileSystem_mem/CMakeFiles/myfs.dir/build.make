# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_SOURCE_DIR = /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem

# Include any dependencies generated for this target.
include CMakeFiles/myfs.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/myfs.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/myfs.dir/flags.make

CMakeFiles/myfs.dir/src/Buffer.cpp.o: CMakeFiles/myfs.dir/flags.make
CMakeFiles/myfs.dir/src/Buffer.cpp.o: src/Buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/myfs.dir/src/Buffer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myfs.dir/src/Buffer.cpp.o -c /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/Buffer.cpp

CMakeFiles/myfs.dir/src/Buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myfs.dir/src/Buffer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/Buffer.cpp > CMakeFiles/myfs.dir/src/Buffer.cpp.i

CMakeFiles/myfs.dir/src/Buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myfs.dir/src/Buffer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/Buffer.cpp -o CMakeFiles/myfs.dir/src/Buffer.cpp.s

CMakeFiles/myfs.dir/src/direntry.cpp.o: CMakeFiles/myfs.dir/flags.make
CMakeFiles/myfs.dir/src/direntry.cpp.o: src/direntry.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/myfs.dir/src/direntry.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myfs.dir/src/direntry.cpp.o -c /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/direntry.cpp

CMakeFiles/myfs.dir/src/direntry.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myfs.dir/src/direntry.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/direntry.cpp > CMakeFiles/myfs.dir/src/direntry.cpp.i

CMakeFiles/myfs.dir/src/direntry.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myfs.dir/src/direntry.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/direntry.cpp -o CMakeFiles/myfs.dir/src/direntry.cpp.s

CMakeFiles/myfs.dir/src/inode.cpp.o: CMakeFiles/myfs.dir/flags.make
CMakeFiles/myfs.dir/src/inode.cpp.o: src/inode.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/myfs.dir/src/inode.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myfs.dir/src/inode.cpp.o -c /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/inode.cpp

CMakeFiles/myfs.dir/src/inode.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myfs.dir/src/inode.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/inode.cpp > CMakeFiles/myfs.dir/src/inode.cpp.i

CMakeFiles/myfs.dir/src/inode.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myfs.dir/src/inode.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/inode.cpp -o CMakeFiles/myfs.dir/src/inode.cpp.s

CMakeFiles/myfs.dir/src/main.cpp.o: CMakeFiles/myfs.dir/flags.make
CMakeFiles/myfs.dir/src/main.cpp.o: src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/myfs.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myfs.dir/src/main.cpp.o -c /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/main.cpp

CMakeFiles/myfs.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myfs.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/main.cpp > CMakeFiles/myfs.dir/src/main.cpp.i

CMakeFiles/myfs.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myfs.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/main.cpp -o CMakeFiles/myfs.dir/src/main.cpp.s

CMakeFiles/myfs.dir/src/myfs.cpp.o: CMakeFiles/myfs.dir/flags.make
CMakeFiles/myfs.dir/src/myfs.cpp.o: src/myfs.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/myfs.dir/src/myfs.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myfs.dir/src/myfs.cpp.o -c /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/myfs.cpp

CMakeFiles/myfs.dir/src/myfs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myfs.dir/src/myfs.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/myfs.cpp > CMakeFiles/myfs.dir/src/myfs.cpp.i

CMakeFiles/myfs.dir/src/myfs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myfs.dir/src/myfs.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/myfs.cpp -o CMakeFiles/myfs.dir/src/myfs.cpp.s

CMakeFiles/myfs.dir/src/superblock.cpp.o: CMakeFiles/myfs.dir/flags.make
CMakeFiles/myfs.dir/src/superblock.cpp.o: src/superblock.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/myfs.dir/src/superblock.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myfs.dir/src/superblock.cpp.o -c /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/superblock.cpp

CMakeFiles/myfs.dir/src/superblock.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myfs.dir/src/superblock.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/superblock.cpp > CMakeFiles/myfs.dir/src/superblock.cpp.i

CMakeFiles/myfs.dir/src/superblock.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myfs.dir/src/superblock.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/src/superblock.cpp -o CMakeFiles/myfs.dir/src/superblock.cpp.s

# Object files for target myfs
myfs_OBJECTS = \
"CMakeFiles/myfs.dir/src/Buffer.cpp.o" \
"CMakeFiles/myfs.dir/src/direntry.cpp.o" \
"CMakeFiles/myfs.dir/src/inode.cpp.o" \
"CMakeFiles/myfs.dir/src/main.cpp.o" \
"CMakeFiles/myfs.dir/src/myfs.cpp.o" \
"CMakeFiles/myfs.dir/src/superblock.cpp.o"

# External object files for target myfs
myfs_EXTERNAL_OBJECTS =

myfs: CMakeFiles/myfs.dir/src/Buffer.cpp.o
myfs: CMakeFiles/myfs.dir/src/direntry.cpp.o
myfs: CMakeFiles/myfs.dir/src/inode.cpp.o
myfs: CMakeFiles/myfs.dir/src/main.cpp.o
myfs: CMakeFiles/myfs.dir/src/myfs.cpp.o
myfs: CMakeFiles/myfs.dir/src/superblock.cpp.o
myfs: CMakeFiles/myfs.dir/build.make
myfs: /usr/lib/libboost_serialization.so
myfs: CMakeFiles/myfs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable myfs"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/myfs.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/myfs.dir/build: myfs

.PHONY : CMakeFiles/myfs.dir/build

CMakeFiles/myfs.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/myfs.dir/cmake_clean.cmake
.PHONY : CMakeFiles/myfs.dir/clean

CMakeFiles/myfs.dir/depend:
	cd /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem /home/wings/OneDrive/Labs/HUST_OS_CourseDesign/myFileSystem_mem/CMakeFiles/myfs.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/myfs.dir/depend

