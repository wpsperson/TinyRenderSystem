set(TARGET_NAME RenderBasic)

INCLUDE_DIRECTORIES(${PlainExamPath})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ThirdParty/glm-0.9.6.3)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/ThirdParty/glfw-3.2.1/include)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/ThirdParty/glfw-3.2.1/lib)

set(Target_HEADER_SRC_LIST
${PlainExamPath}/glad.c
${PlainExamPath}/stb_image.h
${PlainExamPath}/BasicUtils.h
${PlainExamPath}/BasicUtils.cpp
${PlainExamPath}/${TARGET_NAME}/RenderTriangle.h
${PlainExamPath}/${TARGET_NAME}/RenderTriangle.cpp
${PlainExamPath}/${TARGET_NAME}/RenderColorTriangle.h
${PlainExamPath}/${TARGET_NAME}/RenderColorTriangle.cpp
${PlainExamPath}/${TARGET_NAME}/RenderTexture.h
${PlainExamPath}/${TARGET_NAME}/RenderTexture.cpp
${PlainExamPath}/${TARGET_NAME}/StencilTest.h
${PlainExamPath}/${TARGET_NAME}/StencilTest.cpp
${PlainExamPath}/${TARGET_NAME}/TessellationOnShader.h
${PlainExamPath}/${TARGET_NAME}/TessellationOnShader.cpp
${PlainExamPath}/${TARGET_NAME}/main.cpp
)

FILE(GLOB Target_SHADER_LIST ${PlainExamPath}/${TARGET_NAME}/shaders/*.glsl)
source_group("shader" FILES ${Target_SHADER_LIST})
add_executable(${TARGET_NAME}  ${Target_HEADER_SRC_LIST} ${Target_SHADER_LIST})

set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")	
set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
set_target_properties(${TARGET_NAME} PROPERTIES RELEASE_POSTFIX "${CMAKE_RELEASE_POSTFIX}")
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "PlainOpenGLExamples")

SET(ADDITIONAL_LIBRARIES_DEBUG
glfw3d.lib
)
SET(ADDITIONAL_LIBRARIES_RELEASE
glfw3.lib
)
TARGET_LINK_LIBRARIES(${TARGET_NAME} debug "${ADDITIONAL_LIBRARIES_DEBUG}" optimized "${ADDITIONAL_LIBRARIES_RELEASE}")

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory  
                ${PlainExamPath}/${TARGET_NAME}/shaders 
                ${CMAKE_BINARY_DIR}/bin/shaders)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/resources/textures/awesomeface.png
                ${CMAKE_BINARY_DIR}/bin/resources/textures/awesomeface.png)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/resources/textures/container.jpg
                ${CMAKE_BINARY_DIR}/bin/resources/textures/container.jpg)
