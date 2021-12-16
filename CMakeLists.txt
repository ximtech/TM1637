cmake_minimum_required(VERSION 3.20)
project(TM1637 C)

set(CMAKE_C_STANDARD 99)

include(cmake/CPM.cmake)

CPMAddPackage(
        NAME DWTDelay
        GITHUB_REPOSITORY ximtech/DWTDelay
        GIT_TAG origin/main)

set(TM1637_DIRECTORY
        ${DWT_DELAY_DIRECTORY}
        ${TM1637_SOURCE_DIR}
        ${TM1637_SOURCE_DIR}/Fonts
        ${TM1637_SOURCE_DIR}/include
        CACHE STRING "HD44780 LCD I2C directories include to the main project" FORCE)

set(TM1637_SOURCES
        ${DWT_DELAY_SOURCES}
        ${TM1637_SOURCE_DIR}/${PROJECT_NAME}.c
        ${TM1637_SOURCE_DIR}/include/${PROJECT_NAME}.h
        CACHE STRING "HD44780 LCD I2C source files include to the main project" FORCE)