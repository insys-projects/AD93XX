cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.
		
project(ad93xx${POSTFIX_X64})				# Название проекта

set(LIBAD93XX_VERSION_MAJOR 0)
set(LIBAD93XX_VERSION_MINOR 1)
set(VERSION ${LIBAD93XX_VERSION_MAJOR}.${LIBAD93XX_VERSION_MINOR})

file(GLOB SOURCE_LIB "*.cpp")

include_directories(${IIO_INCLUDE_DIRS} ${AD9361_INCLUDE_DIRS})

add_library(${PROJECT_NAME} SHARED ${SOURCE_LIB})# Создание динамической библиотеки

target_link_libraries(${PROJECT_NAME}
	${IIO_LIBRARIES}
	${AD9361_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES
	VERSION ${VERSION}
	SOVERSION ${LIBAD93XX_VERSION_MAJOR}
	FRAMEWORK TRUE
#<----->PUBLIC_HEADER ${LIBAD93XX_HEADERS}
	C_STANDARD 99
	C_STANDARD_REQUIRED ON
	C_EXTENSIONS OFF
)	

list(APPEND INC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/AD93xx_Device.h
	${CMAKE_CURRENT_SOURCE_DIR}/AD93xx_Device_Params.h
)

install(FILES ${INC_FILES} DESTINATION include)

set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib"
	CACHE PATH "Installation directory for libraries")
mark_as_advanced(INSTALL_LIB_DIR)

install(TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
	RUNTIME DESTINATION bin
	FRAMEWORK DESTINATION lib)

if(WIN32)
install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_PATH})
endif(WIN32)