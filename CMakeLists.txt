cmake_minimum_required(VERSION 2.8.12)
project(PlaneCutting)

set(PLANECUTTING_VERSION 1.0)

set(HEADER_FILES
        src/config.h

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

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} ${SCENE_FILES})

target_link_libraries(${PROJECT_NAME} SofaComponentGeneral SofaBaseTopology)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_PLANECUTTING")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PLANECUTTING_VERSION})

sofa_create_package(${PROJECT_NAME} ${PLANECUTTING_VERSION} ${PROJECT_NAME} ${PROJECT_NAME})
