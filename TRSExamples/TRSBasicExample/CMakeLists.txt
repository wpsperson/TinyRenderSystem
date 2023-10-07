set(TARGET_NAME TRSBasicExample)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)


set(Target_INCLUDE_LIST
    ${TRSExamPath}/${TARGET_NAME}/E01TextureBox.h
    ${TRSExamPath}/${TARGET_NAME}/E02BasicMaterial.h
    ${TRSExamPath}/${TARGET_NAME}/E03BasicLight.h
    ${TRSExamPath}/${TARGET_NAME}/E04ElementArray.h
    ${TRSExamPath}/${TARGET_NAME}/E05LoadModelByAssimp.h
    ${TRSExamPath}/${TARGET_NAME}/E06MeshPerformance.h
    ${TRSExamPath}/${TARGET_NAME}/E07TessellationOnShader.h
    ${TRSExamPath}/${TARGET_NAME}/E08PNTriangles.h
    ${TRSExamPath}/${TARGET_NAME}/E09FreeType.h
    ${TRSExamPath}/${TARGET_NAME}/E10NurbsFaceToMesh.h
    )
set(Target_SRC_LIST
    ${TRSExamPath}/${TARGET_NAME}/E01TextureBox.cpp
    ${TRSExamPath}/${TARGET_NAME}/E02BasicMaterial.cpp
    ${TRSExamPath}/${TARGET_NAME}/E03BasicLight.cpp
    ${TRSExamPath}/${TARGET_NAME}/E04ElementArray.cpp
    ${TRSExamPath}/${TARGET_NAME}/E05LoadModelByAssimp.cpp
    ${TRSExamPath}/${TARGET_NAME}/E06MeshPerformance.cpp
    ${TRSExamPath}/${TARGET_NAME}/E07TessellationOnShader.cpp
    ${TRSExamPath}/${TARGET_NAME}/E08PNTriangles.cpp
    ${TRSExamPath}/${TARGET_NAME}/E09FreeType.cpp
    ${TRSExamPath}/${TARGET_NAME}/E10NurbsFaceToMesh.cpp
    ${TRSExamPath}/${TARGET_NAME}/main.cpp
)
source_group("include" FILES ${Target_INCLUDE_LIST} )
source_group("src" FILES ${Target_SRC_LIST})
FILE(GLOB Target_SHADER_LIST ${TRSExamPath}/${TARGET_NAME}/shaders/*.glsl)
source_group("shader" FILES ${Target_SHADER_LIST})
add_executable(${TARGET_NAME}  ${Target_INCLUDE_LIST} ${Target_SRC_LIST} ${Target_SHADER_LIST})
add_dependencies(${TARGET_NAME} TinyRenderSystem)
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "TRSExamples")


set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")	
set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
set_target_properties(${TARGET_NAME} PROPERTIES RELEASE_POSTFIX "${CMAKE_RELEASE_POSTFIX}")

SET(ADDITIONAL_LIBRARIES_DEBUG
TinyRenderSystemd.lib
)
SET(ADDITIONAL_LIBRARIES_RELEASE
TinyRenderSystem.lib
)
TARGET_LINK_LIBRARIES(${TARGET_NAME} debug "${ADDITIONAL_LIBRARIES_DEBUG}" optimized "${ADDITIONAL_LIBRARIES_RELEASE}")


add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory  
                ${TRSExamPath}/${TARGET_NAME}/shaders 
                ${CMAKE_BINARY_DIR}/bin/shaders)


