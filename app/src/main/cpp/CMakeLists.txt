cmake_minimum_required(VERSION 3.22.1)
project(vulkancheck LANGUAGES CXX)

add_library(vulkancheck SHARED vulkan_check.cpp)

find_library(log-lib log)
find_library(android-lib android)
find_library(vulkan-lib vulkan)

target_compile_features(vulkancheck PRIVATE cxx_std_17)

target_link_libraries(
        vulkancheck
        ${log-lib}
        ${android-lib}
        ${vulkan-lib})
