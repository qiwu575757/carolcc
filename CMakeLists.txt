cmake_minimum_required(VERSION 3.16)

PROJECT(compiler)
SET(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
# add_definitions(-Wno-sign-compare -Wno-unused-function,)
FIND_PACKAGE(BISON REQUIRED)
SET(BISONOUTPUT ${CMAKE_SOURCE_DIR}/src/frontend/parser.cpp)

IF(BISON_FOUND)
    ADD_CUSTOM_COMMAND(
      OUTPUT ${BISONOUTPUT}
      COMMAND ${BISON_EXECUTABLE}
              --defines=${CMAKE_SOURCE_DIR}/src/frontend/parser.h
              --output=${BISONOUTPUT}
              ${CMAKE_SOURCE_DIR}/src/frontend/parser.yy
      COMMENT "Generating parser.cpp"
    )
ENDIF()

FIND_PACKAGE(FLEX REQUIRED)
SET(FLEXOUTPUT ${CMAKE_SOURCE_DIR}/src/frontend/token.cpp)
IF(FLEX_FOUND)
    ADD_CUSTOM_COMMAND(
      OUTPUT ${FLEXOUTPUT}
      COMMAND ${FLEX_EXECUTABLE}
              --outfile=${FLEXOUTPUT}
              ${CMAKE_SOURCE_DIR}/src/frontend/token.l
      COMMENT "Generating token.cpp"
    )
ENDIF()

FILE(GLOB_RECURSE source_files "src/*.cpp")

MESSAGE("source is ${source_files}")
SET(INCLUDE_DIRECTORIES ./src )
INCLUDE_DIRECTORIES(src/)

ADD_EXECUTABLE(${PROJECT_NAME} ${source_files} ${BISONOUTPUT} ${FLEXOUTPUT})
