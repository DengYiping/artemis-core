cmake_minimum_required(VERSION 3.5)
project(artemis-core)
include_directories(include)
file(GLOB SOURCE "src/*.cpp")
file(GLOB HEADERS "include/*.*")
add_executable(artemis-core ${SOURCE} ${HEADERS})
target_compile_features(artemis-core PRIVATE cxx_range_for)
set(
  CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -pthread"
)
