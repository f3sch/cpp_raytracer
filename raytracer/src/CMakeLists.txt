# Interface Library
add_library(raytracer INTERFACE)
# target_link_libraries(raytracer INTERFACE )
target_include_directories(
  raytracer
  INTERFACE ${CMAKE_SOURCE_DIR}/include
)


add_executable(main main.cpp)
target_link_libraries(main PRIVATE raytracer OpenMP::OpenMP_CXX Threads::Threads)
