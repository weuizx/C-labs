# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.29

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
CMAKE_SOURCE_DIR = C:\Users\Alexandr\Desktop\AISD\Lab4b

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Alexandr\Desktop\AISD\Lab4b\build

# Include any dependencies generated for this target.
include CMakeFiles/lab4b.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/lab4b.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/lab4b.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lab4b.dir/flags.make

CMakeFiles/lab4b.dir/functions.c.obj: CMakeFiles/lab4b.dir/flags.make
CMakeFiles/lab4b.dir/functions.c.obj: CMakeFiles/lab4b.dir/includes_C.rsp
CMakeFiles/lab4b.dir/functions.c.obj: C:/Users/Alexandr/Desktop/AISD/Lab4b/functions.c
CMakeFiles/lab4b.dir/functions.c.obj: CMakeFiles/lab4b.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\Alexandr\Desktop\AISD\Lab4b\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/lab4b.dir/functions.c.obj"
	C:\SysGCC\mingw32\bin\cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/lab4b.dir/functions.c.obj -MF CMakeFiles\lab4b.dir\functions.c.obj.d -o CMakeFiles\lab4b.dir\functions.c.obj -c C:\Users\Alexandr\Desktop\AISD\Lab4b\functions.c

CMakeFiles/lab4b.dir/functions.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/lab4b.dir/functions.c.i"
	C:\SysGCC\mingw32\bin\cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Alexandr\Desktop\AISD\Lab4b\functions.c > CMakeFiles\lab4b.dir\functions.c.i

CMakeFiles/lab4b.dir/functions.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/lab4b.dir/functions.c.s"
	C:\SysGCC\mingw32\bin\cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Alexandr\Desktop\AISD\Lab4b\functions.c -o CMakeFiles\lab4b.dir\functions.c.s

CMakeFiles/lab4b.dir/main.c.obj: CMakeFiles/lab4b.dir/flags.make
CMakeFiles/lab4b.dir/main.c.obj: CMakeFiles/lab4b.dir/includes_C.rsp
CMakeFiles/lab4b.dir/main.c.obj: C:/Users/Alexandr/Desktop/AISD/Lab4b/main.c
CMakeFiles/lab4b.dir/main.c.obj: CMakeFiles/lab4b.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\Alexandr\Desktop\AISD\Lab4b\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/lab4b.dir/main.c.obj"
	C:\SysGCC\mingw32\bin\cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/lab4b.dir/main.c.obj -MF CMakeFiles\lab4b.dir\main.c.obj.d -o CMakeFiles\lab4b.dir\main.c.obj -c C:\Users\Alexandr\Desktop\AISD\Lab4b\main.c

CMakeFiles/lab4b.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/lab4b.dir/main.c.i"
	C:\SysGCC\mingw32\bin\cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Alexandr\Desktop\AISD\Lab4b\main.c > CMakeFiles\lab4b.dir\main.c.i

CMakeFiles/lab4b.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/lab4b.dir/main.c.s"
	C:\SysGCC\mingw32\bin\cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Alexandr\Desktop\AISD\Lab4b\main.c -o CMakeFiles\lab4b.dir\main.c.s

# Object files for target lab4b
lab4b_OBJECTS = \
"CMakeFiles/lab4b.dir/functions.c.obj" \
"CMakeFiles/lab4b.dir/main.c.obj"

# External object files for target lab4b
lab4b_EXTERNAL_OBJECTS =

lab4b.exe: CMakeFiles/lab4b.dir/functions.c.obj
lab4b.exe: CMakeFiles/lab4b.dir/main.c.obj
lab4b.exe: CMakeFiles/lab4b.dir/build.make
lab4b.exe: CMakeFiles/lab4b.dir/linkLibs.rsp
lab4b.exe: CMakeFiles/lab4b.dir/objects1.rsp
lab4b.exe: CMakeFiles/lab4b.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\Alexandr\Desktop\AISD\Lab4b\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable lab4b.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\lab4b.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lab4b.dir/build: lab4b.exe
.PHONY : CMakeFiles/lab4b.dir/build

CMakeFiles/lab4b.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\lab4b.dir\cmake_clean.cmake
.PHONY : CMakeFiles/lab4b.dir/clean

CMakeFiles/lab4b.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Alexandr\Desktop\AISD\Lab4b C:\Users\Alexandr\Desktop\AISD\Lab4b C:\Users\Alexandr\Desktop\AISD\Lab4b\build C:\Users\Alexandr\Desktop\AISD\Lab4b\build C:\Users\Alexandr\Desktop\AISD\Lab4b\build\CMakeFiles\lab4b.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/lab4b.dir/depend

