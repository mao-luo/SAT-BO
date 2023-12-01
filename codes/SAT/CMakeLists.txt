cmake_minimum_required(VERSION 3.22)
project(sat VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -fPIC")

include_directories()
set(SOURCES sat/main_example.cpp)

add_executable(sat_test 
    sat/main_example.cpp
    sat/walksat.cpp
    sat/ioBase.cpp
    sat/genSolutions.cpp
    sat/dpll.cpp
    sat/check.cpp
    sat/vitrualEnvCaucute.cpp
    sat/creatFlow.cpp
)
aux_source_directory(./src_optimize DIR_SRCS)
add_executable(sat_optimize ${DIR_SRCS})

aux_source_directory(./src_BOBSS1 DIR_SRCS11)
add_executable(BOBSS1 ${DIR_SRCS11})

aux_source_directory(./src_BOBSS2 DIR_SRCS222)
add_executable(BOBSS2 ${DIR_SRCS222})

aux_source_directory(./src_BOBSS3 DIR_SRCS33)
add_executable(BOBSS3 ${DIR_SRCS33})
aux_source_directory(./src_BOBSS0 DIR_SRCS00)
add_executable(BOBSS0 ${DIR_SRCS00})