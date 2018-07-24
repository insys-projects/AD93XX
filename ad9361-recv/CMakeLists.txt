cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(ad9361-recv)				# Название проекта

file(GLOB SOURCE "*.cpp")

include_directories(${AD93XX_INCLUDE_DIRS} ${GIPCY_INCLUDE_DIRS})

IF(WIN32)
add_definitions(-D__IPC_WIN__ -D_UNICODE)
ELSE(WIN32)
add_definitions(-D__IPC_LINUX__)
ENDIF(WIN32)

add_executable(ad9361-recv ${SOURCE})# Создание динамической библиотеки

target_link_libraries(ad9361-recv 
						${AD93XX_LIBRARIES}
						${GIPCY_LIBRARIES})
	
install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_PATH})