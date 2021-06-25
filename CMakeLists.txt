cmake_minimum_required(VERSION 3.10)
project(CheckCompileTimes LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	message( FATAL_ERROR "Only clang currently supports -ftime-trace" )
endif()

set(HEADERS_TO_PROCESS 
						boost/accumulators/accumulators.hpp
						boost/algorithm/algorithm.hpp
						boost/align.hpp
						boost/any.hpp
						boost/array.hpp
						boost/asio.hpp
						boost/assert.hpp
						boost/assign.hpp
						boost/atomic.hpp
						boost/beast.hpp
						boost/bimap.hpp
						boost/bind.hpp
						boost/call_traits.hpp
						boost/callable_traits.hpp
						boost/chrono.hpp
						boost/circular_buffer.hpp
						boost/compressed_pair.hpp
						boost/concept_check.hpp
						boost/config.hpp
						boost/container/flat_map.hpp
						boost/container/flat_set.hpp
						boost/container/slist.hpp
						boost/container/small_vector.hpp
						boost/container/stable_vector.hpp
						boost/container/static_vector.hpp
						boost/container_hash/hash.hpp
						
						
)

foreach(header ${HEADERS_TO_PROCESS})
	string(REPLACE "." "-" filename_preliminary ${header})
	string(REPLACE "/" "-" header_name ${filename_preliminary})
	set(filename "${CMAKE_CURRENT_LIST_DIR}/${header_name}_main.cpp")
	file(WRITE ${filename} "#include <${header}>\n  int main() { return 0; }\n")
		
	set(executable_name "Check${header_name}")
	add_executable(${executable_name} ${filename})
	target_compile_options(${executable_name} PUBLIC -ftime-trace)
	target_link_libraries(${executable_name} pthread)
endforeach()

add_executable(CheckCompileTimes main.cpp)
target_compile_definitions(CheckCompileTimes PUBLIC CACHE_DIR="${CMAKE_CACHEFILE_DIR}")
