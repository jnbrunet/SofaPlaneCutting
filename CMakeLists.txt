cmake_minimum_required(VERSION 2.8.12)
project(PlaneCutting)

set(HEADER_FILES
        src/TetrahedronSetTopologyCuttingAlgorithms.h
        src/TetrahedronSetTopologyCuttingAlgorithms.inl
        src/PlaneCutting.h
        )

set(SOURCE_FILES
        src/initPlaneCutting.cpp
        src/TetrahedronSetTopologyCuttingAlgorithms.cpp
        src/PlaneCutting.cpp
        )

file(GLOB SCENE_FILES
        "scenes/*.scn"
        "scenes/*/*.scn"
        )

find_package(SofaGeneral REQUIRED)
find_package(SofaBase REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} ${SCENE_FILES})
target_link_libraries(${PROJECT_NAME} SofaComponentGeneral SofaBaseTopology)
