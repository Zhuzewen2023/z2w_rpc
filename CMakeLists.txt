cmake_minimum_required(VERSION 3.10)

project(z2w_rpc VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_SOURCE_DIR}
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SERVER_SOURCES
    rpc_server.c
    crc32.c
    rpc.c
    cJSON.c
    rpc_method.c
    rpc_generator.c
)

set(CLIENT_SOURCES
    rpc_client.c
    crc32.c
    rpc.c
    cJSON.c
    rpc_method.c
    rpc_generator.c
)

set(REGISTER_JSON_TEST_SOURCES
    register_json_test.c
    rpc.c
    cJSON.c
    crc32.c
    rpc_method.c
    rpc_generator.c
)

set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_library(ntyco STATIC IMPORTED)
add_library(rpc_method SHARED rpc_client.c cJSON.c crc32.c rpc.c rpc_method.c)

target_include_directories(rpc_method PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/NtyCo/core)

set_target_properties(ntyco PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libntyco.a
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/NtyCo/core"    
)

add_executable(rpc_server ${SERVER_SOURCES})
add_executable(rpc_client ${CLIENT_SOURCES})
add_executable(register_json_test ${REGISTER_JSON_TEST_SOURCES})

target_link_libraries(rpc_server ntyco pthread dl rpc_method "-rdynamic")
# target_link_options(rpc_server PRIVATE "-rdynamic")
target_link_libraries(rpc_client ntyco pthread dl rpc_method "-rdynamic")
# target_link_options(rpc_client PRIVATE "-rdynamic")
target_link_libraries(register_json_test ntyco pthread dl rpc_method "-rdynamic")
# target_link_options(register_json_test PRIVATE "-rdynamic")



