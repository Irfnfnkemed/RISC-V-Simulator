cmake_minimum_required(VERSION 3.16.3)
project(RISC_V_Simulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(src include)

add_executable(code src/memory.cpp include/memory.h src/tool.cpp include/tool.h src/algorithm_logic_unit.cpp "include/algorithm_logic_unit.h" src/load_store_buffer.cpp include/load_store_buffer.h src/clock_control.cpp include/clock_control.h "src/reservation_staion.cpp" "include/reservation_staion.h" src/reorder_buffer.cpp src/register_file.cpp include/register_file.h src/program_counter.cpp include/program_counter.h include/reorder_buffer.h src/decoder.cpp include/decoder.h src/predictor.cpp include/predictor.h main.cpp)
