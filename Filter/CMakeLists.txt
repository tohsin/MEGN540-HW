cmake_minimum_required(VERSION 3.10)
set( PROJECT_ID "filter_hw")

project(${PROJECT_ID})

set(CMAKE_CXX_STANDARD 14)

file(GLOB SRC_C_FILES "solution/*.c")
file(GLOB SRC_CPP_FILES "solution/*.cpp")
file(GLOB SRC_CC_FILES "solution/*.cc")

file(GLOB MAIN_FILE "*main.c*")


# list(REMOVE_ITEM SRC_C_FILES ${MAIN_FILE})
# list(REMOVE_ITEM SRC_CPP_FILES ${MAIN_FILE})
# list(REMOVE_ITEM SRC_CC_FILES ${MAIN_FILE})
message(STATUS "Found C Files...")
foreach (_variableName ${SRC_C_FILES})
    message(STATUS "\t${_variableName}")
endforeach()
message(STATUS "")

message(STATUS "Found CPP Files...")
foreach (_variableName ${SRC_CPP_FILES})
    message(STATUS "\t${_variableName}")
endforeach()
message(STATUS "")

message(STATUS "Found CC Files...")
foreach (_variableName ${SRC_CC_FILES})
    message(STATUS "\t${_variableName}")
endforeach()
message(STATUS "")



add_library( _${PROJECT_ID} ${SRC_C_FILES} ${SRC_CC_FILES} ${SRC_CPP_FILES} )
target_link_libraries( _${PROJECT_ID}  yaml-cpp )


add_executable( ${PROJECT_ID} ${MAIN_FILE} )
target_link_libraries( ${PROJECT_ID}  _${PROJECT_ID} yaml-cpp )


enable_testing()


file(GLOB TEST_SRC_C_FILES "test/*.c")
file(GLOB TEST_SRC_CPP_FILES "test/**.cpp")
file(GLOB TEST_SRC_CC_FILES "*test/*.cc")

add_executable(
  ${PROJECT_ID}_test
  ${TEST_SRC_C_FILES} ${TEST_SRC_CC_FILES} ${TEST_SRC_CPP_FILES}
)
target_link_libraries(
        ${PROJECT_ID}_test
        _${PROJECT_ID}
        gtest
        gtest_main
        pthread
        yaml-cpp
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_ID}_test)



