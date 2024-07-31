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
    ${CMAKE_CURRENT_SOURCE_DIR}/SettingDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SettingDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UiSettingDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AppConst.h
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



get_target_property(Qt5_Core_Location Qt5::Core LOCATION)
get_filename_component(Qt5_DIR ${Qt5_Core_Location} DIRECTORY)
find_program(LUPDATE_EXECUTABLE lupdate PATH ${Qt5_DIR})
find_program(LRELEASE_EXECUTABLE lrelease PATH ${Qt5_DIR})
set(OUTPUT_DIR "${PROJECT_BINARY_DIR}/bin")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Qt5_DIR: ${Qt5_DIR}")
message(STATUS "OUTPUT_DIR: ${OUTPUT_DIR}")

set(TS_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/languages/app_en.ts"
    "${CMAKE_CURRENT_SOURCE_DIR}/languages/app_zh_CN.ts"
    "${CMAKE_CURRENT_SOURCE_DIR}/languages/qt_zh_CN.ts"
)
foreach(SRC_TS ${TS_FILES})
    get_filename_component(FILE_NAME ${SRC_TS} NAME_WE)
    set(OUTPUT_TS ${OUTPUT_DIR}/languages/${FILE_NAME}.ts)
    set(OUTPUT_QM ${OUTPUT_DIR}/languages/${FILE_NAME}.qm)
    
    if(${FILE_NAME} STREQUAL "qt_zh_CN")
        add_custom_command(OUTPUT ${OUTPUT_QM}
            COMMAND ${CMAKE_COMMAND} -E remove ${OUTPUT_QM}
            COMMAND ${LRELEASE_EXECUTABLE} ${SRC_TS} -qm ${OUTPUT_QM}
        )
    else()
        add_custom_command(OUTPUT ${OUTPUT_QM}
            COMMAND ${CMAKE_COMMAND} -E remove ${OUTPUT_TS} ${OUTPUT_QM}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SRC_TS} ${OUTPUT_TS}
            COMMAND ${LUPDATE_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR} -extensions cpp,h -locations none -no-obsolete -ts ${OUTPUT_TS}
            COMMAND ${LRELEASE_EXECUTABLE} ${OUTPUT_TS} -qm ${OUTPUT_QM}
        )
    endif()
    list(APPEND output_qm_files ${OUTPUT_QM})
endforeach()

add_custom_target(translate ALL DEPENDS ${output_qm_files})
set_target_properties(translate PROPERTIES FOLDER "TRSExamples")