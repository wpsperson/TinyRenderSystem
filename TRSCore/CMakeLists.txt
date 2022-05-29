set(TARGET_NAME TRSCore)

add_definitions(-DTRSLIB)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/${TARGET_NAME})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ThirdParty/glfw-3.2.1/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ThirdParty/assimp-4.1.0/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ThirdParty/freetype-2.10.4/include)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/ThirdParty/glfw-3.2.1/lib)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/ThirdParty/assimp-4.1.0/lib64)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/ThirdParty/freetype-2.10.4/lib64)


FILE(GLOB Target_INCLUDE_LIST  "${PROJECT_SOURCE_DIR}/${TARGET_NAME}/*.h" )
FILE(GLOB Target_SRC_LIST ${PROJECT_SOURCE_DIR}/${TARGET_NAME}/*.cpp ${PROJECT_SOURCE_DIR}/${TARGET_NAME}/*.c)
FILE(GLOB Target_SHADER_LIST ${PROJECT_SOURCE_DIR}/${TARGET_NAME}/shaders/*.glsl)
source_group("include" FILES ${Target_INCLUDE_LIST} )
source_group("src" FILES ${Target_SRC_LIST})
source_group("shader" FILES ${Target_SHADER_LIST})
#add_executable(${TARGET_NAME}  ${Target_INCLUDE_LIST} ${Target_SRC_LIST} ${Target_SHADER_LIST})
add_library(${TARGET_NAME} SHARED ${Target_INCLUDE_LIST} ${Target_SRC_LIST} ${Target_SHADER_LIST} )
    
set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")	
set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib")
set_target_properties(${TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/lib")
set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
set_target_properties(${TARGET_NAME} PROPERTIES RELEASE_POSTFIX "${CMAKE_RELEASE_POSTFIX}")

SET(ADDITIONAL_LIBRARIES_DEBUG
glfw3d.lib
assimp-vc140-mt.lib
freetyped.lib
)
SET(ADDITIONAL_LIBRARIES_RELEASE
glfw3.lib
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
                ${CMAKE_SOURCE_DIR}/${TARGET_NAME}/shaders 
                ${CMAKE_BINARY_DIR}/bin/shaders)

