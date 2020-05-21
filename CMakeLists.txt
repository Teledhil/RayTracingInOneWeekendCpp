cmake_minimum_required(VERSION 3.16)

set(LIBRARY_NAME ray_tracer)
project(${LIBRARY_NAME})
LIST(APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Threads)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

# Colors!
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_compile_options (-fcolor-diagnostics)
endif ()

# Enable Link Time optimization
if(CMAKE_BUILD_TYPE MATCHES Release)
  if(${CMAKE_VERSION} VERSION_GREATER "3.8.0")
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result)
    if(result)
      message(STATUS "LTO enabled")
      set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
  endif()
endif()

# Release flags
#
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffast-math")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")

# Debug flags
#
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Og")
# Make code easier to debug with GDB
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -ggdb")
endif()
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -glldb")
endif()
# ASAN
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
# Make code easier to profile
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer")

# Library
#
include_directories(
  ${INCLUDE_DIRS}
  )

list(APPEND SOURCES ${PROJECT_SOURCE_DIR}/camera.cc)
list(APPEND SOURCES ${PROJECT_SOURCE_DIR}/color.cc)
list(APPEND SOURCES ${PROJECT_SOURCE_DIR}/dielectric.cc)
list(APPEND SOURCES ${PROJECT_SOURCE_DIR}/hittable.cc)
list(APPEND SOURCES ${PROJECT_SOURCE_DIR}/hittable_list.cc)
list(APPEND SOURCES ${PROJECT_SOURCE_DIR}/lambertian.cc)
list(APPEND SOURCES ${PROJECT_SOURCE_DIR}/metal.cc)
list(APPEND SOURCES ${PROJECT_SOURCE_DIR}/sphere.cc)
list(APPEND SOURCES ${PROJECT_SOURCE_DIR}/utility.cc)
list(APPEND SOURCES ${PROJECT_SOURCE_DIR}/vec3.cc)

## Generate the static library from the sources
add_library(${LIBRARY_NAME} STATIC ${SOURCES})

## rtx
#
set(RTX_NAME rtx)
set(RTX_SOURCES ${PROJECT_SOURCE_DIR}/rtx.cc)
add_executable(${RTX_NAME} ${RTX_SOURCES})
target_link_libraries(${RTX_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${RTX_NAME} ${LIBRARY_NAME})

add_custom_target(rtx_target DEPENDS ${RTX_NAME})
