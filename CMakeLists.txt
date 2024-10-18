cmake_minimum_required(VERSION 3.29)
project(untitled C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wno-int-conversion)
add_executable(server server.c
        db/db.h
        admin/admintasks.h
        employee/emptasks.h
        customer/customertasks.h
        admin/adminoptions.h
        customer/customeroptions.h
        employee/empoptions.h
        transactions/transtasks.h
        loans/loanoptions.h
        loans/loantasks.h
        datetime_ot/datetimetasks.h
        datetime_ot/ottasks.h
)
add_executable(client client.c)
add_executable(adminCreator adminCreator.c)