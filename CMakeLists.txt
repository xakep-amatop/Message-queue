cmake_minimum_required (VERSION 2.6)

set(PROJECT_SERVER mq_server)
set(PROJECT_CLIENT mq_client)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/common /usr/local/include /usr/local/lib )

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")

set(SOURCES_SHARED ${CMAKE_CURRENT_SOURCE_DIR}/common/message_info.pb.cc)
set(SOURCES_SERVER ${CMAKE_CURRENT_SOURCE_DIR}/src_server/server.cpp ${SOURCES_SHARED})
set(SOURCES_CLIENT ${CMAKE_CURRENT_SOURCE_DIR}/src_client/client.cpp ${SOURCES_SHARED})

set(LINK_LIBS -lprotobuf -pthread -lrt)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/common/message_info.pb.h ${CMAKE_CURRENT_SOURCE_DIR}/common/message_info.pb.cc
  COMMAND protoc --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/common --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/common/ message_info.proto
)

add_custom_target(regenerate
  COMMAND protoc --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}/common --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/common/ message_info.proto
)

add_executable(${PROJECT_SERVER} ${SOURCES_SERVER})
add_executable(${PROJECT_CLIENT} ${SOURCES_CLIENT})

add_dependencies(${PROJECT_SERVER} regenerate)
add_dependencies(${PROJECT_CLIENT} regenerate)

target_link_libraries(${PROJECT_SERVER} ${LINK_LIBS})
target_link_libraries(${PROJECT_CLIENT} ${LINK_LIBS})