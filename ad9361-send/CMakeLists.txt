cmake_minimum_required(VERSION 2.8) # �������� ������ CMake.
									# ���� ������ ������������ ���������
									# ������ ��������, ��������� ��������� �����.

project(ad9361-send${POSTFIX_X64})				# �������� �������

file(GLOB SOURCE "*.cpp")

include_directories(${AD93XX_INCLUDE_DIRS} ${GIPCY_INCLUDE_DIRS})

IF(WIN32)
add_definitions(-D__IPC_WIN__ -D_UNICODE)
ELSE(WIN32)
add_definitions(-D__IPC_LINUX__)
ENDIF(WIN32)

add_executable(${PROJECT_NAME} ${SOURCE})# �������� ������������ ����������

target_link_libraries(${PROJECT_NAME} 
						ad93xx
						${GIPCY_LIBRARIES})
	
	
IF(WIN32)
install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_PATH})
ENDIF(WIN32)