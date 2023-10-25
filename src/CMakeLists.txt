set(TARGET_NAME TinyRenderSystem)

add_definitions(-DTRSLIB)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ThirdParty/assimp-4.1.0/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ThirdParty/freetype-2.10.4/include)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/ThirdParty/assimp-4.1.0/lib64)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/ThirdParty/freetype-2.10.4/lib64)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(Target_INCLUDE_LIST
    ${INCLUDE_DIR}/Core/TRSCharacterTexture.h
    ${INCLUDE_DIR}/Core/TRSConfig.h
    ${INCLUDE_DIR}/Core/TRSConst.h
    ${INCLUDE_DIR}/Core/TRSExport.h
    ${INCLUDE_DIR}/Core/TRSShader.h
    ${INCLUDE_DIR}/Core/TRSStateSet.h
    ${INCLUDE_DIR}/Core/TRSTexture.h
    ${INCLUDE_DIR}/Core/TRSVAO.h
    ${INCLUDE_DIR}/Core/TRSVAOBase.h
    ${INCLUDE_DIR}/Core/TRSViewer.h
    ${INCLUDE_DIR}/Core/TRSVisitors.h
    ${INCLUDE_DIR}/Core/TRSBox.h
    ${INCLUDE_DIR}/DataModel/NodeVisitor.h
    ${INCLUDE_DIR}/DataModel/TRSGeode.h
    ${INCLUDE_DIR}/DataModel/TRSGroup.h
    ${INCLUDE_DIR}/DataModel/TRSMesh.h
    ${INCLUDE_DIR}/DataModel/TRSNode.h
    ${INCLUDE_DIR}/DataModel/TRSTextNode.h
    ${INCLUDE_DIR}/Event/TRSEventDefine.h
    ${INCLUDE_DIR}/Event/TRSEventDispatcher.h
    ${INCLUDE_DIR}/Event/TRSEventHandler.h
    ${INCLUDE_DIR}/Event/TRSShortcutKeyHandler.h
    ${INCLUDE_DIR}/Geometry/TRSGeometry.h
    ${INCLUDE_DIR}/Geometry/TRSCube.h
    ${INCLUDE_DIR}/Geometry/TRSCylinder.h
    ${INCLUDE_DIR}/Geometry/TRSGrid.h
    ${INCLUDE_DIR}/Geometry/GeometryUtil.h
    ${INCLUDE_DIR}/IO/TRSAssimpLoader.h
    ${INCLUDE_DIR}/Math/BSpline.h
    ${INCLUDE_DIR}/Math/BSplineSurface.h
    ${INCLUDE_DIR}/Math/TRSMathUtil.h
    ${INCLUDE_DIR}/Math/TRSMatrix.h
    ${INCLUDE_DIR}/Math/TRSVector.h
    ${INCLUDE_DIR}/Util/CullVisitor.h
    ${INCLUDE_DIR}/Util/ObjUtils.h
    ${INCLUDE_DIR}/Util/stb_image.h
    ${INCLUDE_DIR}/Util/TRSResource.h
    ${INCLUDE_DIR}/Util/TRSUtils.h
    ${INCLUDE_DIR}/Camera/TRSCamera.h
    ${INCLUDE_DIR}/Camera/TRSDefaultCameraHandler.h
    ${INCLUDE_DIR}/glad/glad.h
    ${INCLUDE_DIR}/KHR/khrplatform.h
    )


set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(Target_SRC_LIST
    ${SRC_DIR}/Core/TRSCharacterTexture.cpp
    ${SRC_DIR}/Core/TRSShader.cpp
    ${SRC_DIR}/Core/TRSStateSet.cpp
    ${SRC_DIR}/Core/TRSTexture.cpp
    ${SRC_DIR}/Core/TRSVAO.cpp
    ${SRC_DIR}/Core/TRSViewer.cpp
    ${SRC_DIR}/Core/TRSVisitors.cpp
    ${SRC_DIR}/Core/TRSBox.cpp
    ${SRC_DIR}/DataModel/NodeVisitor.cpp
    ${SRC_DIR}/DataModel/TRSGeode.cpp
    ${SRC_DIR}/DataModel/TRSGroup.cpp
    ${SRC_DIR}/DataModel/TRSMesh.cpp
    ${SRC_DIR}/DataModel/TRSNode.cpp
    ${SRC_DIR}/DataModel/TRSTextNode.cpp
    ${SRC_DIR}/Event/TRSEventDispatcher.cpp
    ${SRC_DIR}/Event/TRSEventHandler.cpp
    ${SRC_DIR}/Event/TRSShortcutKeyHandler.cpp
    ${SRC_DIR}/Geometry/TRSGeometry.cpp
    ${SRC_DIR}/Geometry/TRSCube.cpp
    ${SRC_DIR}/Geometry/TRSCylinder.cpp
    ${SRC_DIR}/Geometry/TRSGrid.cpp
    ${SRC_DIR}/Geometry/GeometryUtil.cpp
    ${SRC_DIR}/IO/TRSAssimpLoader.cpp
    ${SRC_DIR}/Math/BSpline.cpp
    ${SRC_DIR}/Math/BSplineSurface.cpp
    ${SRC_DIR}/Math/TRSMathUtil.cpp
    ${SRC_DIR}/Math/TRSMatrix.cpp
    ${SRC_DIR}/Util/CullVisitor.cpp
    ${SRC_DIR}/Util/ObjUtils.cpp
    ${SRC_DIR}/Util/TRSResource.cpp
    ${SRC_DIR}/Util/TRSUtils.cpp
    ${SRC_DIR}/Camera/TRSCamera.cpp
    ${SRC_DIR}/Camera/TRSDefaultCameraHandler.cpp
    ${SRC_DIR}/glad/glad.c
)

FILE(GLOB Target_SHADER_LIST ${PROJECT_SOURCE_DIR}/shaders/*.glsl)
source_group(TREE ${INCLUDE_DIR} FILES ${Target_INCLUDE_LIST})
source_group(TREE ${SRC_DIR} FILES ${Target_SRC_LIST})
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
assimp-vc140-mt.lib
freetyped.lib
)
SET(ADDITIONAL_LIBRARIES_RELEASE
assimp-vc140-mt.lib
freetype.lib
)
TARGET_LINK_LIBRARIES(${TARGET_NAME} debug "${ADDITIONAL_LIBRARIES_DEBUG}" optimized "${ADDITIONAL_LIBRARIES_RELEASE}")

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy 
                ${CMAKE_SOURCE_DIR}/ThirdParty/assimp-4.1.0/bin64/assimp-vc140-mt.dll
                ${CMAKE_BINARY_DIR}/bin/assimp-vc140-mt.dll)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/resources 
                ${CMAKE_BINARY_DIR}/bin/resources)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory  
                ${CMAKE_SOURCE_DIR}/shaders 
                ${CMAKE_BINARY_DIR}/bin/shaders)

