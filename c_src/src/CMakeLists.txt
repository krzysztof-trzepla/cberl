##
# Author: Krzysztof Trzepla
# Copyright (C) 2017: Krzysztof Trzepla
# This software is released under the MIT license cited in 'LICENSE.md'
#

set(CBERL_SYSTEM_INCLUDE_DIRS
    ${ASIO_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES *.cc *.h)

add_library(cberl_obj OBJECT
    ${SOURCES})

target_include_directories(cberl_obj SYSTEM PRIVATE
    ${CBERL_SYSTEM_INCLUDE_DIRS})

set(CBERL_SYSTEM_INCLUDE_DIRS
    ${CBERL_SYSTEM_INCLUDE_DIRS}
    PARENT_SCOPE)

set(CBERL_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    PARENT_SCOPE)

set(CBERL_LIBRARIES
    ${CMAKE_THREAD_LIBS_INIT}
    ${COUCHBASE_LIBRARY}
    PARENT_SCOPE)

set(PROJECT_SOURCES
    $<TARGET_OBJECTS:cberl_obj>
    ${PROJECT_SOURCES}
    PARENT_SCOPE)
