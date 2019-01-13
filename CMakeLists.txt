cmake_minimum_required(VERSION 3.13)
project(CarbonCopySignatureMatching)

set(CMAKE_CXX_STANDARD 17)

add_executable(ccsm-train ccsm-train.cpp Neuron.cpp Neuron.h Net.cpp Net.h TrainingData.cpp TrainingData.h)
add_executable(ccsm-track ccsm-track.cpp TrackPad.cpp TrackPad.h)
add_executable(ccsm-view ccsm-view.cpp TrackPad.cpp TrackPad.h)
add_executable(ccsm-test ccsm-test.cpp TrackPad.cpp TrackPad.h Net.cpp Net.h Neuron.cpp Neuron.h TrainingData.cpp TrainingData.h)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-build-debug" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(ccsm-train ${SFML_LIBRARIES})
    target_link_libraries(ccsm-track ${SFML_LIBRARIES})
    target_link_libraries(ccsm-view ${SFML_LIBRARIES})
    target_link_libraries(ccsm-test ${SFML_LIBRARIES})
endif()
