cmake_minimum_required(VERSION 3.1.0)

project(mt-primitives)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}
    PROPERTIES CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
target_sources(${PROJECT_NAME} PRIVATE
    src/main.cpp
    src/spinlock/spinlock_tas.cpp
    src/spinlock/spinlock_ttas.cpp
    src/atomic_snapshot/atomic_snapshot_lockfree.cpp
    src/atomic_snapshot/atomic_snapshot_waitfree.cpp
)
target_link_libraries(${PROJECT_NAME}
    Threads::Threads
    atomic
)
