cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(ad9361-send)				# Название проекта

file(GLOB SOURCE "*.cpp")

SET(AD93XX_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../ad93xx)
SET(AD93XX_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/../ad93xx/ad93xx${CMAKE_LINK_LIBRARY_SUFFIX})

include_directories(${AD93XX_INCLUDE_DIRS} ${GIPCY_INCLUDE_DIRS}
					${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_definitions(-D__IPC_WIN__ -D_UNICODE)

add_executable(ad9361-send ${SOURCE})# Создание динамической библиотеки

target_link_libraries(ad9361-send 
						${AD93XX_LIBRARIES}
						${GIPCY_LIBRARIES})
	
	
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../bin)