cmake_minimum_required(VERSION 3.10)
project(CheckCompileTimes LANGUAGES CXX)

find_package(Boost)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	message( FATAL_ERROR "Only clang currently supports -ftime-trace" )
endif()

set(HEADERS_TO_PROCESS 
						boost/accumulators/accumulators.hpp
						boost/algorithm/algorithm.hpp
						boost/align/align.hpp
						boost/any.hpp
						boost/array.hpp
						boost/asio.hpp
)

foreach(header ${HEADERS_TO_PROCESS})
	string(REPLACE "." "_" filename_preliminary ${header})
	string(REPLACE "/" "_" header_name ${filename_preliminary})
	set(filename "${CMAKE_CURRENT_LIST_DIR}/${header_name}_main.cpp")
	file(WRITE ${filename} "#include <${header}>\n  int main() { return 0; }\n")
		
	set(executable_name "Check${header_name}")
	add_executable(${executable_name} ${filename})
	target_compile_options(${executable_name} PUBLIC -ftime-trace)
	target_link_libraries(${executable_name} pthread)
endforeach()

add_executable(CheckCompileTimes main.cpp)
