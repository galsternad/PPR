cmake_minimum_required(VERSION 3.23)

project(N1_26)

add_executable(main)

set_property(TARGET main PROPERTY CXX_STANDARD 11)

target_compile_options(main PRIVATE
    -g
    -O0
    -fsanitize=undefined
    -fno-omit-frame-pointer
)

target_link_options(main PRIVATE
    -fsanitize=undefined
)

target_sources(main
    PRIVATE
        "src/main.cpp"
        "src/calculator.hpp"
        "src/calculator.cpp"
        "src/metrics.hpp"
        "src/metrics.cpp"
        "src/parser.hpp"
        "src/parser.cpp"
        "src/random_generator.hpp"
        "src/random_generator.cpp"
        "src/sequence.hpp"
        "src/sequence.cpp"
        "src/solver.hpp"
        "src/solver.cpp"
)