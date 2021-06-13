cmake_minimum_required(VERSION 3.10)

list(APPEND Sources Main.cpp)

add_executable(automata-service ${Sources})
source_group("" FILES ${Sources})
target_link_libraries(automata-service automata-library)

install(TARGETS automata-service RUNTIME DESTINATION bin)
install(FILES automata.service DESTINATION systemd)
