cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(AD93XX CXX C)				# Название проекта

find_library(IIO_LIBRARIES iio)
find_path(IIO_INCLUDE_DIRS iio.h)
find_library(AD9361_LIBRARIES ad9361)
find_path(AD9361_INCLUDE_DIRS ad9361.h)
find_library(GIPCY_LIBRARIES gipcy)
find_path(GIPCY_INCLUDE_DIRS gipcy.h)

if(X64)
set(POSTFIX_X64 64)
endif(X64)

set(AD93XX_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/ad93xx)

set(INSTALL_PATH ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(ad93xx)
add_subdirectory(ad9361-recv)
add_subdirectory(ad9361-send)