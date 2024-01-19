cmake_minimum_required(VERSION 3.20)


project(hashy)


file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_library(hashy SHARED ${sources})
add_library(hashy_static STATIC ${sources})

add_executable(hashy_e ${sources})

target_compile_options(hashy PUBLIC -fPIC)
target_compile_options(hashy_static PUBLIC -fPIC)
target_compile_options(hashy_e PUBLIC -fPIC)

set(HASHY_LIBS m pthread)
target_link_libraries(hashy PUBLIC ${HASHY_LIBS})
target_link_libraries(hashy_static PUBLIC ${HASHY_LIBS})
target_link_libraries(hashy_e PUBLIC ${HASHY_LIBS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/include)


target_include_directories(
  hashy_e
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(
  hashy
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_include_directories(
  hashy_static
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/include)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/bench)
