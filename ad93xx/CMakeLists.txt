cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

#IF(WIN32)
#	SET(AD93XX_LIB "ad93xx")
#ELSE(WIN64)		
#	SET(AD93XX_LIB "ad93xx64")
#ENDIF(WIN64)

project(ad93xx)				# Название проекта

#list(APPEND SOURCE_LIB
#	AD93xx_Device.cpp
#	AD9361_Device_Impl.cpp
#)

file(GLOB SOURCE_LIB "*.cpp")

include_directories(${IIO_INCLUDE_DIRS} ${AD9361_INCLUDE_DIRS})

add_library(ad93xx SHARED ${SOURCE_LIB})# Создание динамической библиотеки

target_link_libraries(ad93xx
	${IIO_LIBRARIES}
	${AD9361_LIBRARIES})
	
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND INC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/AD93xx_Device.h
	${CMAKE_CURRENT_SOURCE_DIR}/AD93xx_Device_Params.h
)

install(FILES ${INC_FILES} DESTINATION include)
