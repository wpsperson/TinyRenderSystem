cmake_minimum_required(VERSION 3.0)
set(ProjectName TinyRenderSystem)
PROJECT(${ProjectName})

SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
set_property( GLOBAL PROPERTY USE_FOLDERS ON)

ADD_SUBDIRECTORY(src)

option(BuildTRSExamples "是否构建 TRSExamples?" OFF)
if (BuildTRSExamples)
	ADD_SUBDIRECTORY(TRSExamples)
endif(BuildTRSExamples)

option(BuildPlainOpenGLExamples "是否构建 PlainOpenGLExamples" ON)
if (BuildPlainOpenGLExamples)
	ADD_SUBDIRECTORY(PlainOpenGLExamples)
endif(BuildPlainOpenGLExamples)


