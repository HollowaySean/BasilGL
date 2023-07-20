cmake_minimum_required(VERSION 3.0.0)
project(BlackHoleViz VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(BlackHoleViz src/main.cpp)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

## User added
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(BlackHoleViz ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
