# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(main)

target_include_directories(__idf_lvgl PUBLIC ${CMAKE_CURRENT_LIST_DIR}/FT800-FT813)
target_compile_definitions(__idf_lvgl PUBLIC "-DEVE_RVT50H")
target_compile_options(__idf_lvgl PRIVATE -Wno-error=uninitialized -Wno-error=format -Wno-error=maybe-uninitialized)
