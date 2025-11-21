# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

project(wrdice_library VERSION 0.0.1 LANGUAGES C)

add_library(wrdice SHARED
  "wrdice.c"
  "battle.c"
  "stats.c"
  "dice_op.c"
)

set_target_properties(wrdice PROPERTIES
  PUBLIC_HEADER wrdice.h
  OUTPUT_NAME "wrdice"
)

target_compile_definitions(wrdice PUBLIC DART_SHARED_LIB)

if (ANDROID)
  # Support Android 15 16k page size
  target_link_options(wrdice PRIVATE "-Wl,-z,max-page-size=16384")
endif()
