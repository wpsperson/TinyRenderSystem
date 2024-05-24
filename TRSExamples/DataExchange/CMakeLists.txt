set(TARGET_NAME DataExchange)

# list(APPEND CMAKE_PREFIX_PATH "D:/3rd-party/occ-7.8.0")
find_package(OpenCASCADE REQUIRED)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${OpenCASCADE_INCLUDE_DIR})
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)

message(STATUS "current : ${CMAKE_CURRENT_SOURCE_DIR}")

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/StepConverter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/StepConverter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DataExchange.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DataExchange.cpp
    )

add_library(${TARGET_NAME} SHARED ${SOURCE_FILES})
add_dependencies(${TARGET_NAME} TinyRenderSystem)
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "TRSExamples")

set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")	
set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
set_target_properties(${TARGET_NAME} PROPERTIES RELEASE_POSTFIX "${CMAKE_RELEASE_POSTFIX}")

target_link_libraries(${TARGET_NAME} debug "TinyRenderSystemd.lib" optimized "TinyRenderSystem.lib")

set (RELEVANT_LIBRARIES TKernel;TKMath;TKXSBase;TKBRep;TKG2d;TKG3d;TKTopAlgo;TKGeomBase;TKShHealing;TKGeomAlgo;TKPrim;TKLCAF;TKXCAF;TKDESTEP;TKBinXCAF;TKBO;TKMesh)
foreach(LIB ${RELEVANT_LIBRARIES})
    target_link_libraries(${TARGET_NAME} debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
    target_link_libraries(${TARGET_NAME} optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB})
endforeach()
