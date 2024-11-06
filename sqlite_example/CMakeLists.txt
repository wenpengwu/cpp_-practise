cmake_minimum_required(VERSION 3.16)

project(sqlite_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)





add_executable(sqlite_example main.cpp)
target_include_directories(sqlite_example PRIVATE  "/usr/local/Cellar/sqlite/3.46.1/include")

target_link_directories(sqlite_example PRIVATE  "/usr/local/Cellar/sqlite/3.46.1/lib")

target_link_libraries(sqlite_example sqlite3)

include(GNUInstallDirs)
install(TARGETS sqlite_example
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
