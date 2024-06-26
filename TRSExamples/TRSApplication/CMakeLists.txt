set(TARGET_NAME TRSApplication)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 CONFIG REQUIRED COMPONENTS Widgets)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLWidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ImportStep.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ImportStep.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )


add_executable(${TARGET_NAME}  ${SOURCE_FILES})
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
TARGET_LINK_LIBRARIES(${TARGET_NAME} Qt5::Widgets ${CMAKE_DL_LIBS})



