cmake_minimum_required(VERSION 3.16.3)
project(RISC_V_Simulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(src head_file)

add_executable(RISC_V_Simulator src/memory.cpp head_file/memory.h src/tool.cpp head_file/tool.h main.cpp src/algorithm_logic_unit.cpp "head_file/algorithm_logic_unit.h" src/load_store_buffer.cpp head_file/load_store_buffer.h src/clock_control.cpp head_file/clock_control.h "src/reservation_staion.cpp" "head_file/reservation_staion.h" src/reorder_buffer.cpp src/register_file.cpp head_file/register_file.h src/program_counter.cpp head_file/program_counter.h head_file/reorder_buffer.h src/decoder.cpp head_file/decoder.h src/predictor.cpp head_file/predictor.h)
