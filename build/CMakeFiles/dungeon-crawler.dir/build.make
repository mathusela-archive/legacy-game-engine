# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.22

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Programming\C++\code\ALevelProgrammingProject\dungeon-crawler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Programming\C++\code\ALevelProgrammingProject\dungeon-crawler\build

# Include any dependencies generated for this target.
include CMakeFiles/dungeon-crawler.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/dungeon-crawler.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/dungeon-crawler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dungeon-crawler.dir/flags.make

CMakeFiles/dungeon-crawler.dir/source/main.cpp.obj: CMakeFiles/dungeon-crawler.dir/flags.make
CMakeFiles/dungeon-crawler.dir/source/main.cpp.obj: CMakeFiles/dungeon-crawler.dir/includes_CXX.rsp
CMakeFiles/dungeon-crawler.dir/source/main.cpp.obj: ../source/main.cpp
CMakeFiles/dungeon-crawler.dir/source/main.cpp.obj: CMakeFiles/dungeon-crawler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Programming\C++\code\ALevelProgrammingProject\dungeon-crawler\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/dungeon-crawler.dir/source/main.cpp.obj"
	C:\MinGW\bin\mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/dungeon-crawler.dir/source/main.cpp.obj -MF CMakeFiles\dungeon-crawler.dir\source\main.cpp.obj.d -o CMakeFiles\dungeon-crawler.dir\source\main.cpp.obj -c D:\Programming\C++\code\ALevelProgrammingProject\dungeon-crawler\source\main.cpp

CMakeFiles/dungeon-crawler.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dungeon-crawler.dir/source/main.cpp.i"
	C:\MinGW\bin\mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Programming\C++\code\ALevelProgrammingProject\dungeon-crawler\source\main.cpp > CMakeFiles\dungeon-crawler.dir\source\main.cpp.i

CMakeFiles/dungeon-crawler.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dungeon-crawler.dir/source/main.cpp.s"
	C:\MinGW\bin\mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Programming\C++\code\ALevelProgrammingProject\dungeon-crawler\source\main.cpp -o CMakeFiles\dungeon-crawler.dir\source\main.cpp.s

CMakeFiles/dungeon-crawler.dir/source/glad.c.obj: CMakeFiles/dungeon-crawler.dir/flags.make
CMakeFiles/dungeon-crawler.dir/source/glad.c.obj: CMakeFiles/dungeon-crawler.dir/includes_C.rsp
CMakeFiles/dungeon-crawler.dir/source/glad.c.obj: ../source/glad.c
CMakeFiles/dungeon-crawler.dir/source/glad.c.obj: CMakeFiles/dungeon-crawler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Programming\C++\code\ALevelProgrammingProject\dungeon-crawler\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/dungeon-crawler.dir/source/glad.c.obj"
	C:\MinGW\bin\mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/dungeon-crawler.dir/source/glad.c.obj -MF CMakeFiles\dungeon-crawler.dir\source\glad.c.obj.d -o CMakeFiles\dungeon-crawler.dir\source\glad.c.obj -c D:\Programming\C++\code\ALevelProgrammingProject\dungeon-crawler\source\glad.c

CMakeFiles/dungeon-crawler.dir/source/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dungeon-crawler.dir/source/glad.c.i"
	C:\MinGW\bin\mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\Programming\C++\code\ALevelProgrammingProject\dungeon-crawler\source\glad.c > CMakeFiles\dungeon-crawler.dir\source\glad.c.i

CMakeFiles/dungeon-crawler.dir/source/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dungeon-crawler.dir/source/glad.c.s"
	C:\MinGW\bin\mingw32-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\Programming\C++\code\ALevelProgrammingProject\dungeon-crawler\source\glad.c -o CMakeFiles\dungeon-crawler.dir\source\glad.c.s

# Object files for target dungeon-crawler
dungeon__crawler_OBJECTS = \
"CMakeFiles/dungeon-crawler.dir/source/main.cpp.obj" \
"CMakeFiles/dungeon-crawler.dir/source/glad.c.obj"

# External object files for target dungeon-crawler
dungeon__crawler_EXTERNAL_OBJECTS =

dungeon-crawler.exe: CMakeFiles/dungeon-crawler.dir/source/main.cpp.obj
dungeon-crawler.exe: CMakeFiles/dungeon-crawler.dir/source/glad.c.obj
dungeon-crawler.exe: CMakeFiles/dungeon-crawler.dir/build.make
dungeon-crawler.exe: CMakeFiles/dungeon-crawler.dir/linklibs.rsp
dungeon-crawler.exe: CMakeFiles/dungeon-crawler.dir/objects1.rsp
dungeon-crawler.exe: CMakeFiles/dungeon-crawler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Programming\C++\code\ALevelProgrammingProject\dungeon-crawler\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable dungeon-crawler.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\dungeon-crawler.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dungeon-crawler.dir/build: dungeon-crawler.exe
.PHONY : CMakeFiles/dungeon-crawler.dir/build

CMakeFiles/dungeon-crawler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\dungeon-crawler.dir\cmake_clean.cmake
.PHONY : CMakeFiles/dungeon-crawler.dir/clean

CMakeFiles/dungeon-crawler.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Programming\C++\code\ALevelProgrammingProject\dungeon-crawler D:\Programming\C++\code\ALevelProgrammingProject\dungeon-crawler D:\Programming\C++\code\ALevelProgrammingProject\dungeon-crawler\build D:\Programming\C++\code\ALevelProgrammingProject\dungeon-crawler\build D:\Programming\C++\code\ALevelProgrammingProject\dungeon-crawler\build\CMakeFiles\dungeon-crawler.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dungeon-crawler.dir/depend

