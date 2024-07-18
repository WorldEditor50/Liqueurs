cmake_minimum_required(VERSION 3.5)

project(gelato LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(Lib3rdParty "/home/galois/MySpace/3rdParty")
# opencv
set(OpenCV_DIR ${Lib3rdParty}/opencv410/lib/cmake/opencv4)
find_package(OpenCV REQUIRED)
if (OpenCV_FOUND)
    message("found opencv:${OpenCV_LIBRARIES}")
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()
# eigen
set(Eigen3_DIR ${Lib3rdParty}/eigen340/share/eigen3/cmake)
find_package(Eigen3 3.4 REQUIRED NO_MODULE)
if (EIGEN3_FOUND)
    message(STATUS "found eigen3:${EIGEN3_INCLUDE_DIRS}")
    include_directories(${EIGEN3_INCLUDE_DIRS})
endif()
# sophus
set(Sophus_DIR ${Lib3rdParty}/sophus/share/sophus/cmake)
find_package(Sophus REQUIRED)
if (Sophus_FOUND)
    message(STATUS "include sophus:${Lib3rdParty}/sophus/include")
    include_directories(${Lib3rdParty}/sophus/include)
endif()
# glog
set(glog_DIR ${Lib3rdParty}/glog/lib/cmake/glog)
find_package(glog REQUIRED)
if (glog_FOUND)
    message(STATUS "include glog:${GLOG_INCLUDE_DIRS}")
    include_directories(${GLOG_INCLUDE_DIRS})
endif()
# ceres
set(Ceres_DIR ${Lib3rdParty}/ceres/lib/cmake/Ceres)
find_package(Ceres REQUIRED)
if (Ceres_FOUND)
    message(STATUS "include ceres:${CERES_INCLUDE_DIRS}")
    include_directories(${CERES_INCLUDE_DIRS})
endif()
# test opengl
add_subdirectory(OpenglExamples)
# test opencv
add_executable(test_opencv test/test_opencv.cpp)
target_link_libraries(test_opencv PRIVATE ${OpenCV_LIBRARIES})
# test eigen3
add_executable(test_eigen3 test/test_eigen3.cpp)
# test sophus
add_executable(test_sophus test/test_sophus.cpp)
# test ceres
add_executable(test_ceres test/test_ceres.cpp)
target_link_libraries(test_ceres PRIVATE
    ${CERES_LIBRARIES}
    ${GLOG_LIBRARIES})
