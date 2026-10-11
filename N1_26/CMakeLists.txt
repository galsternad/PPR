cmake_minimum_required(VERSION 3.23)

project(N1_26)

add_executable(main)

set_property(TARGET main PROPERTY CXX_STANDARD 11)

target_sources(main
    PRIVATE
        "src/main.cpp"
)