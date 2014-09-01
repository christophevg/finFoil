set(CMAKE_AUTOUIC ON)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Sources
set(SRC
    main.cpp
    patheditor-tests/quadrantrestrictortest.cpp
    patheditor-tests/serializationtests.cpp
)

# Headers
set(HDR
    patheditor-tests/quadrantrestrictortest.h
    patheditor-tests/serializationtests.h
)


#
# The executable
#

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
add_executable(${TESTS_BINARY_NAME} WIN32 ${SRC} ${HDR} ${MOC_SRC} ${FORMS_HEADERS} ${RC_SRC} ${QM})


#
# Linking
#

#Qt modules
target_link_libraries(${TESTS_BINARY_NAME} Qt5::Core Qt5::Test)

# hrlib
target_link_libraries(${TESTS_BINARY_NAME} hrlib)
target_include_directories(${TESTS_BINARY_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src/hrlib")

# patheditor
target_link_libraries(${TESTS_BINARY_NAME} patheditor)
target_include_directories(${TESTS_BINARY_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src/patheditor")

# JenSON
target_link_libraries(${TESTS_BINARY_NAME} jenson)
target_include_directories(${TESTS_BINARY_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/submodules/jenson/src")