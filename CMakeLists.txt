cmake_minimum_required(VERSION 3.13)
project(CarbonCopySignatureMatching)

set(CMAKE_CXX_STANDARD 17)

add_executable(CarbonCopySignatureMatching ccsm-train.cpp Neuron.cpp Neuron.h Net.cpp Net.h TrainingData.cpp TrainingData.h)
