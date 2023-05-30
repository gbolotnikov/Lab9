cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bulk VERSION ${PROJECT_VESRION})

set(INTERFACES
    Include/Interfaces/Manager.hpp
    Include/Interfaces/Writer.hpp
)

set(HEADERS ${INTERFACES}
    Include/CommandManager.hpp
    Include/ConsoleReader.hpp
    Include/ConsoleWriter.hpp
    Include/FileWriter.hpp
    Include/ThreadPool.hpp
)    

set(SOURCES ${INTERFACES}
    CommandManager.cpp
    ConsoleReader.cpp
    FileWriter.cpp
)    


add_executable(bulk main.cpp
    CommandManager.cpp
    FileWriter.cpp
    ConsoleReader.cpp
)

add_library(async SHARED Lib/async.cpp Lib/async.h ${SOURCES} ${HEADERS})
add_executable(client client.cpp)
target_link_libraries(client async)

set_target_properties(bulk async client PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(bulk PRIVATE "${CMAKE_BINARY_DIR}" Include)
target_include_directories(async PRIVATE "${CMAKE_BINARY_DIR}" Include)
target_include_directories(client PRIVATE "${CMAKE_BINARY_DIR}" Lib)

install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)