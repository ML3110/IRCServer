project(ircServer)
include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(ircServer ${SOURCES})
