cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(ad9361-recv${POSTFIX_X64})				# Название проекта

file(GLOB SOURCE "*.cpp")

IF(WIN32)
add_definitions(-D__IPC_WIN__ -D_UNICODE)
ELSE(WIN32)
add_definitions(-D__IPC_LINUX__)
ENDIF(WIN32)

include_directories(${AD93XX_INCLUDE_DIRS} ${GIPCY_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCE})# Создание динамической библиотеки

target_link_libraries(${PROJECT_NAME} 
			ad93xx
			${GIPCY_LIBRARIES})
	
IF(WIN32)
install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_PATH})
ENDIF(WIN32)