set(TARGET_NAME TRSApplication)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)


set(Target_INCLUDE_LIST
    ${TRSExamPath}/${TARGET_NAME}/E01TextureBox.h
    )
set(Target_SRC_LIST
    ${TRSExamPath}/${TARGET_NAME}/E01TextureBox.cpp
    ${TRSExamPath}/${TARGET_NAME}/main.cpp
)
source_group("include" FILES ${Target_INCLUDE_LIST} )
source_group("src" FILES ${Target_SRC_LIST})
source_group("shader" FILES ${Target_SHADER_LIST})
add_executable(${TARGET_NAME}  ${Target_INCLUDE_LIST} ${Target_SRC_LIST} ${Target_SHADER_LIST})
add_dependencies(${TARGET_NAME} TinyRenderSystem)
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "TRSApplication")


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


