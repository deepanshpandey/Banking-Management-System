cmake_minimum_required(VERSION 3.29)
project(untitled C)

set(CMAKE_C_STANDARD 11)

add_executable(server server.c
        db.h
        admin/admintasks.h
        employee/emptasks.h
        customer/customertasks.h
)
add_executable(client client.c)
add_executable(adminCreator adminCreator.c
        readAdmin.c
)