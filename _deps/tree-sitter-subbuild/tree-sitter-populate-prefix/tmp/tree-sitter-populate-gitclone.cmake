
if(NOT "/home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps/tree-sitter-subbuild/tree-sitter-populate-prefix/src/tree-sitter-populate-stamp/tree-sitter-populate-gitinfo.txt" IS_NEWER_THAN "/home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps/tree-sitter-subbuild/tree-sitter-populate-prefix/src/tree-sitter-populate-stamp/tree-sitter-populate-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps/tree-sitter-subbuild/tree-sitter-populate-prefix/src/tree-sitter-populate-stamp/tree-sitter-populate-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps/tree-sitter-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps/tree-sitter-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"  clone --no-checkout "https://github.com/tree-sitter/tree-sitter.git" "tree-sitter-src"
    WORKING_DIRECTORY "/home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/tree-sitter/tree-sitter.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"  checkout 0.16.9 --
  WORKING_DIRECTORY "/home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps/tree-sitter-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '0.16.9'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps/tree-sitter-src"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps/tree-sitter-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps/tree-sitter-subbuild/tree-sitter-populate-prefix/src/tree-sitter-populate-stamp/tree-sitter-populate-gitinfo.txt"
    "/home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps/tree-sitter-subbuild/tree-sitter-populate-prefix/src/tree-sitter-populate-stamp/tree-sitter-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/tim/Documents/Anwendungsprojekt/Bachelor/MiniLua/_deps/tree-sitter-subbuild/tree-sitter-populate-prefix/src/tree-sitter-populate-stamp/tree-sitter-populate-gitclone-lastrun.txt'")
endif()
