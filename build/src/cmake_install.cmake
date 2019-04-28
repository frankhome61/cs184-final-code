# Install script for directory: /Users/Frank/Documents/Documents - SuperBook/Academics/UC Berkeley/Courses/CS 184 - Foundations of Computer Graphics/Projects/Final Project/code/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/Frank/Documents/Documents - SuperBook/Academics/UC Berkeley/Courses/CS 184 - Foundations of Computer Graphics/Projects/Final Project/code/clothsim")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/Frank/Documents/Documents - SuperBook/Academics/UC Berkeley/Courses/CS 184 - Foundations of Computer Graphics/Projects/Final Project/code" TYPE EXECUTABLE FILES "/Users/Frank/Documents/Documents - SuperBook/Academics/UC Berkeley/Courses/CS 184 - Foundations of Computer Graphics/Projects/Final Project/code/build/clothsim")
  if(EXISTS "$ENV{DESTDIR}/Users/Frank/Documents/Documents - SuperBook/Academics/UC Berkeley/Courses/CS 184 - Foundations of Computer Graphics/Projects/Final Project/code/clothsim" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/Frank/Documents/Documents - SuperBook/Academics/UC Berkeley/Courses/CS 184 - Foundations of Computer Graphics/Projects/Final Project/code/clothsim")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/Frank/Documents/Documents - SuperBook/Academics/UC Berkeley/Courses/CS 184 - Foundations of Computer Graphics/Projects/Final Project/code/build/ext/nanogui"
      "$ENV{DESTDIR}/Users/Frank/Documents/Documents - SuperBook/Academics/UC Berkeley/Courses/CS 184 - Foundations of Computer Graphics/Projects/Final Project/code/clothsim")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Library/Frameworks"
      "$ENV{DESTDIR}/Users/Frank/Documents/Documents - SuperBook/Academics/UC Berkeley/Courses/CS 184 - Foundations of Computer Graphics/Projects/Final Project/code/clothsim")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -u -r "$ENV{DESTDIR}/Users/Frank/Documents/Documents - SuperBook/Academics/UC Berkeley/Courses/CS 184 - Foundations of Computer Graphics/Projects/Final Project/code/clothsim")
    endif()
  endif()
endif()

