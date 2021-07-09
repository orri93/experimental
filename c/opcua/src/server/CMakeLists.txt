cmake_minimum_required(VERSION 3.8.2)

find_package(open62541 REQUIRED)

set(opc_ua_ex_server_target opcuaexserver)

list(APPEND opc_ua_ex_server_source
  server.c)

list(APPEND opc_ua_ex_server_libraries
  open62541::open62541)

add_executable(${opc_ua_ex_server_target}
  ${opc_ua_ex_server_source})

target_link_libraries(${opc_ua_ex_server_target}
  ${opc_ua_ex_server_libraries})

if(DEPLOY_PDB_FILES)
  install(FILES "$<TARGET_PDB_FILE:${opc_ua_ex_server_target}>"
    DESTINATION pdb
    COMPONENT pdb)
endif()

install(TARGETS ${opc_ua_ex_server_target}
  RUNTIME DESTINATION bin)
