cmake_minimum_required(VERSION 3.5)

set(Lib3rdParty "/home/galois/MySpace/3rdParty")
# opengl
find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    message(STATUS "found opengl:${OPENGL_LIBRARY}")
endif()
# glfw
if (WIN32)
    set(GLFW3_DIR ${Lib3rdParty}/GLFW/lib/cmake/glfw3)
    set(GLFW3_INCLUDE_DIRS ${Lib3rdParty}/GLFW/include)
    file(GLOB GLFW3_LIBRARIES ${Lib3rdParty}/GLFW/lib/glfw3.lib)
    include_directories(${GLFW3_INCLUDE_DIRS})
elseif (UNIX)
    set(GLFW3_DIR ${Lib3rdParty}/glfw34/lib/cmake/glfw3)
    set(GLFW3_INCLUDE_DIRS ${Lib3rdParty}/glfw34/include)
    file(GLOB GLFW3_LIBRARIES ${Lib3rdParty}/glfw34/lib/libglfw3.a)
    include_directories(${GLFW3_INCLUDE_DIRS})
endif()
# glm
if (WIN32)
    set(GLM_DIR ${Lib3rdParty}/glm/share/glm)
    set(GLM_INCLUDE_DIRS ${Lib3rdParty}/glm/include)
    file(GLOB GLM_LIBRARIES ${Lib3rdParty}/glm/lib/glm.lib)
    include_directories(${GLM_INCLUDE_DIRS})
endif()

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
# GLAD
include_directories(${SRC_DIR}/glad/include)
file(GLOB GLAD_FILES ${SRC_DIR}/glad/src/*.cpp)
set(GL_LIBS ${OPENGL_LIBRARY} ${GLFW3_LIBRARIES})
# test opengl
# gl01
file(GLOB GL01_FILE ${SRC_DIR}/gl01.cpp)
add_executable(gl01 ${GL01_FILE} ${GLAD_FILES})
target_link_libraries(gl01 PRIVATE ${GL_LIBS})
# gl02
file(GLOB GL02_FILE ${SRC_DIR}/gl02.cpp)
add_executable(gl02 ${GL02_FILE} ${GLAD_FILES})
target_link_libraries(gl02 PRIVATE ${GL_LIBS})
# gl03
file(GLOB GL03_FILE ${SRC_DIR}/gl03.cpp)
add_executable(gl03 ${GL03_FILE} ${GLAD_FILES})
target_link_libraries(gl03 PRIVATE ${GL_LIBS})
# gl04
file(GLOB GL04_FILE ${SRC_DIR}/gl04.cpp)
add_executable(gl04 ${GL04_FILE} ${GLAD_FILES})
target_link_libraries(gl04 PRIVATE ${GL_LIBS})
# gl05
file(GLOB GL05_FILE ${SRC_DIR}/gl05.cpp)
add_executable(gl05 ${GL05_FILE} ${GLAD_FILES})
target_link_libraries(gl05 PRIVATE ${GL_LIBS})
# gl06
file(GLOB GL06_FILE ${SRC_DIR}/gl06.cpp)
add_executable(gl06 ${GL06_FILE} ${GLAD_FILES})
target_link_libraries(gl06 PRIVATE ${GL_LIBS})
# gl07
file(GLOB GL07_FILE ${SRC_DIR}/gl07.cpp)
add_executable(gl07 ${GL07_FILE} ${GLAD_FILES})
target_link_libraries(gl07 PRIVATE ${GL_LIBS})
# gl08
file(GLOB GL08_FILE ${SRC_DIR}/gl08.cpp shader.h shader.cpp)
add_executable(gl08 ${GL08_FILE} ${GLAD_FILES})
target_link_libraries(gl08 PRIVATE ${GL_LIBS})
# gl09
file(GLOB GL09_FILE ${SRC_DIR}/gl09.cpp shader.h shader.cpp bmp.hpp)
add_executable(gl09 ${GL09_FILE} ${GLAD_FILES})
target_link_libraries(gl09 PRIVATE ${GL_LIBS})
# gl10
file(GLOB GL10_FILE ${SRC_DIR}/gl10.cpp shader.h shader.cpp bmp.hpp)
add_executable(gl10 ${GL10_FILE} ${GLAD_FILES})
target_link_libraries(gl10 PRIVATE ${GL_LIBS})
# gl11
file(GLOB GL11_FILE ${SRC_DIR}/gl11.cpp shader.h shader.cpp bmp.hpp)
add_executable(gl11 ${GL11_FILE} ${GLAD_FILES})
target_link_libraries(gl11 PRIVATE ${GL_LIBS})
# gl12
file(GLOB GL12_FILE ${SRC_DIR}/gl12.cpp shader.h shader.cpp bmp.hpp)
add_executable(gl12 ${GL12_FILE} ${GLAD_FILES})
target_link_libraries(gl12 PRIVATE ${GL_LIBS})
# gl13
file(GLOB GL13_FILE ${SRC_DIR}/gl13.cpp shader.h shader.cpp bmp.hpp)
add_executable(gl13 ${GL13_FILE} ${GLAD_FILES})
target_link_libraries(gl13 PRIVATE ${GL_LIBS})
# gl14
file(GLOB GL14_FILE ${SRC_DIR}/gl14.cpp shader.h shader.cpp bmp.hpp)
add_executable(gl14 ${GL14_FILE} ${GLAD_FILES})
target_link_libraries(gl14 PRIVATE ${GL_LIBS})
# gl15
file(GLOB GL15_FILE ${SRC_DIR}/gl15.cpp shader.h shader.cpp bmp.hpp)
add_executable(gl15 ${GL15_FILE} ${GLAD_FILES})
target_link_libraries(gl15 PRIVATE ${GL_LIBS})
# gl16
file(GLOB GL16_FILE ${SRC_DIR}/gl16.cpp shader.h shader.cpp bmp.hpp)
add_executable(gl16 ${GL16_FILE} ${GLAD_FILES})
target_link_libraries(gl16 PRIVATE ${GL_LIBS})
# gl17
file(GLOB GL17_FILE ${SRC_DIR}/gl17.cpp shader.h shader.cpp bmp.hpp)
add_executable(gl17 ${GL17_FILE} ${GLAD_FILES})
target_link_libraries(gl17 PRIVATE ${GL_LIBS})
# gl18
file(GLOB GL18_FILE ${SRC_DIR}/gl18.cpp shader.h shader.cpp bmp.hpp)
add_executable(gl18 ${GL18_FILE} ${GLAD_FILES})
target_link_libraries(gl18 PRIVATE ${GL_LIBS})
# gl19
file(GLOB GL19_FILE ${SRC_DIR}/gl19.cpp shader.h shader.cpp bmp.hpp)
add_executable(gl19 ${GL19_FILE} ${GLAD_FILES})
target_link_libraries(gl19 PRIVATE ${GL_LIBS})
# gl20
file(GLOB GL20_FILE ${SRC_DIR}/gl20.cpp shader.h shader.cpp bmp.hpp)
add_executable(gl20 ${GL20_FILE} ${GLAD_FILES})
target_link_libraries(gl20 PRIVATE ${GL_LIBS})
