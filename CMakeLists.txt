cmake_minimum_required(VERSION 3.16)
project(QAlgorithms LANGUAGES C)

add_library(qalg_lib STATIC
    gates.c
    grover.c
    measure.c
    qft.c
    state.c
)

target_include_directories(qalg_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(qalg_lib PUBLIC c_std_99)
target_link_libraries(qalg_lib PRIVATE m)


add_executable(grover main.c)
target_link_libraries(grover PRIVATE qalg_lib)

add_executable(test tests.c)
target_link_libraries(test PRIVATE qalg_lib)

add_custom_target(run_grover
    COMMAND grover
    DEPENDS grover
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Запуск grover..."
)
add_custom_target(run_test
    COMMAND test
    DEPENDS test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Запуск тестов..."
)