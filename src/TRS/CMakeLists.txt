set(TARGET_NAME TinyRenderSystem)

add_definitions(-DTRSLIB)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/TRS/glad)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ThirdParty/freetype-2.10.4/include)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/ThirdParty/freetype-2.10.4/lib64)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/TRS)
set(Target_INCLUDE_LIST
    ${INCLUDE_DIR}/TRSCharacterTexture.h
    ${INCLUDE_DIR}/TRSConst.h
    ${INCLUDE_DIR}/TRSMathConst.h
    ${INCLUDE_DIR}/TRSEnum.h
    ${INCLUDE_DIR}/TRSExport.h
    ${INCLUDE_DIR}/TRSShader.h
    ${INCLUDE_DIR}/TRSStateSet.h
    ${INCLUDE_DIR}/TRSTexture.h
    ${INCLUDE_DIR}/TRSVAO.h
    ${INCLUDE_DIR}/TRSViewer.h
    ${INCLUDE_DIR}/TRSVisitors.h
    ${INCLUDE_DIR}/TRSBox.h
    ${INCLUDE_DIR}/NodeVisitor.h
    ${INCLUDE_DIR}/TRSGeode.h
    ${INCLUDE_DIR}/TRSGroup.h
    ${INCLUDE_DIR}/TRSMesh.h
    ${INCLUDE_DIR}/TRSNode.h
    ${INCLUDE_DIR}/TRSTextNode.h
    ${INCLUDE_DIR}/TRSEventDefine.h
    ${INCLUDE_DIR}/TRSEventDispatcher.h
    ${INCLUDE_DIR}/TRSEventHandler.h
    ${INCLUDE_DIR}/TRSShortcutKeyHandler.h
    ${INCLUDE_DIR}/TRSGeometry.h
    ${INCLUDE_DIR}/TRSCube.h
    ${INCLUDE_DIR}/TRSCylinder.h
    ${INCLUDE_DIR}/TRSGrid.h
    ${INCLUDE_DIR}/TRSMathUtil.h
    ${INCLUDE_DIR}/TRSMatrix.h
    ${INCLUDE_DIR}/TRSVector.h
    ${INCLUDE_DIR}/CullVisitor.h
    ${INCLUDE_DIR}/ObjUtils.h
    ${INCLUDE_DIR}/stb_image.h
    ${INCLUDE_DIR}/TRSUtils.h
    ${INCLUDE_DIR}/TRSCamera.h
    ${INCLUDE_DIR}/TRSDefaultCameraHandler.h
    ${INCLUDE_DIR}/glad/glad.h
    ${INCLUDE_DIR}/glad/KHR/khrplatform.h
    )

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src/TRS)
set(Target_SRC_LIST
    ${SRC_DIR}/TRSCharacterTexture.cpp
    ${SRC_DIR}/TRSShader.cpp
    ${SRC_DIR}/TRSStateSet.cpp
    ${SRC_DIR}/TRSTexture.cpp
    ${SRC_DIR}/TRSVAO.cpp
    ${SRC_DIR}/TRSViewer.cpp
    ${SRC_DIR}/TRSVisitors.cpp
    ${SRC_DIR}/TRSBox.cpp
    ${SRC_DIR}/NodeVisitor.cpp
    ${SRC_DIR}/TRSGeode.cpp
    ${SRC_DIR}/TRSGroup.cpp
    ${SRC_DIR}/TRSMesh.cpp
    ${SRC_DIR}/TRSNode.cpp
    ${SRC_DIR}/TRSTextNode.cpp
    ${SRC_DIR}/TRSEventDispatcher.cpp
    ${SRC_DIR}/TRSEventHandler.cpp
    ${SRC_DIR}/TRSShortcutKeyHandler.cpp
    ${SRC_DIR}/TRSGeometry.cpp
    ${SRC_DIR}/TRSCube.cpp
    ${SRC_DIR}/TRSCylinder.cpp
    ${SRC_DIR}/TRSGrid.cpp
    ${SRC_DIR}/TRSMathUtil.cpp
    ${SRC_DIR}/TRSMatrix.cpp
    ${SRC_DIR}/CullVisitor.cpp
    ${SRC_DIR}/ObjUtils.cpp
    ${SRC_DIR}/TRSUtils.cpp
    ${SRC_DIR}/TRSCamera.cpp
    ${SRC_DIR}/TRSDefaultCameraHandler.cpp
    ${SRC_DIR}/glad.c
)

FILE(GLOB Target_SHADER_LIST ${PROJECT_SOURCE_DIR}/shaders/*.glsl)
#source_group(TREE ${INCLUDE_DIR} FILES ${Target_INCLUDE_LIST})
#source_group(TREE ${SRC_DIR} FILES ${Target_SRC_LIST})
source_group(include FILES ${Target_INCLUDE_LIST})
source_group(src FILES ${Target_SRC_LIST})
source_group("shader" FILES ${Target_SHADER_LIST})

add_library(${TARGET_NAME} SHARED ${Target_INCLUDE_LIST} ${Target_SRC_LIST} ${Target_SHADER_LIST} )
    
set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")	
set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib")
set_target_properties(${TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/lib")
set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
set_target_properties(${TARGET_NAME} PROPERTIES RELEASE_POSTFIX "${CMAKE_RELEASE_POSTFIX}")

SET(ADDITIONAL_LIBRARIES_DEBUG
freetyped.lib
)
SET(ADDITIONAL_LIBRARIES_RELEASE
freetype.lib
)
TARGET_LINK_LIBRARIES(${TARGET_NAME} debug "${ADDITIONAL_LIBRARIES_DEBUG}" optimized "${ADDITIONAL_LIBRARIES_RELEASE}")

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/resources 
                ${CMAKE_BINARY_DIR}/bin/resources)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory  
                ${CMAKE_SOURCE_DIR}/shaders 
                ${CMAKE_BINARY_DIR}/bin/shaders)

