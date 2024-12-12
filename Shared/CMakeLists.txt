add_library(Shared INTERFACE)

target_include_directories(Shared INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(Shared INTERFACE cxx_std_20)