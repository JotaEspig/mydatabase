cmake_minimum_required(VERSION 2.8.5...3.27.5)
project(mydatabase-project)

# Debug mode
set(FLAGS "-Wall -Wextra")
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

# CMake settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${FLAGS} -g3 -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${FLAGS} -O3")

option(DEBUG "Enable debug mode" ON)
if (DEBUG)
    message("DEBUG MODE ON")
    add_definitions(-DDEBUG)
else()
    message("DEBUG MODE OFF")
endif()

# Find packages
find_package(Doxygen OPTIONAL_COMPONENTS dot)


# =-=-=-=-=-=-= DOCS =-=-=-=-=-=-=
if (DOXYGEN_FOUND)
    message("Doxygen found. You can use `make docs`")

    add_custom_target(
        docs
        COMMAND ${DOXYGEN_EXECUTABLE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        VERBATIM
    )
else (DOXYGEN_FOUND)
    message("Doxygen not found. You won't be able to use `make docs`")
endif (DOXYGEN_FOUND)


# =-=-=-=-=-=-= LIBRARY =-=-=-=-=-=-=
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# Source files
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(
    SOURCE_FILES
    ${SOURCE_DIR}/bla.cpp
)

# Adding mydatabase shared library
add_library(mydatabase SHARED ${SOURCE_FILES})

target_link_libraries(
    mydatabase
    PRIVATE
    stdc++
)

# Header files
target_include_directories(
    mydatabase
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/include
)


# =-=-=-=-=-=-= TESTS =-=-=-=-=-=-=
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# ------ MAIN ------

# Source files
set(
    MAIN_TEST_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/tests/main.cpp
)

# Adding executables for testing
add_executable(main-test ${MAIN_TEST_SOURCE_FILES})

# Libraries
target_link_directories(
    main-test
    PRIVATE
    ./lib
)
target_link_libraries(
    main-test
    PRIVATE
    mydatabase
    stdc++
)

# Header files
target_include_directories(
    main-test
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
