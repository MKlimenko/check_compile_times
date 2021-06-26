cmake_minimum_required(VERSION 3.10)
project(CheckCompileTimes LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#	message( FATAL_ERROR "Only clang currently supports -ftime-trace" )
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
						boost/contract.hpp
						boost/convert.hpp
						boost/coroutine/all.hpp
						boost/coroutine2/all.hpp
						boost/crc.hpp
						boost/date_time.hpp
						boost/dll.hpp
						boost/dynamic_bitset.hpp
						boost/exception/all.hpp
						boost/fiber/all.hpp
						boost/filesystem.hpp
						boost/flyweight.hpp
						boost/foreach.hpp
						boost/format.hpp
						boost/function.hpp
						boost/function_types/function_type.hpp
						boost/functional.hpp
						boost/fusion/sequence.hpp
						boost/geometry.hpp
						boost/gil.hpp
						boost/graph/graph_traits.hpp
                        boost/hana.hpp
                        boost/heap/pairing_heap.hpp
                        boost/histogram.hpp
                        boost/hof.hpp
                        boost/icl/interval.hpp
						boost/integer.hpp
						boost/intrusive_ptr.hpp
						boost/iterator.hpp
						boost/lambda/lambda.hpp
						boost/lexical_cast.hpp
						boost/local_function.hpp
						boost/locale.hpp
						boost/lockfree/queue.hpp
						boost/log/core/core.hpp
						boost/metaparse.hpp
						boost/multi_array.hpp
						boost/multi_index_container.hpp
						boost/multiprecision/integer.hpp
						boost/numeric/interval.hpp
						boost/operators.hpp
						boost/optional.hpp
						boost/outcome.hpp
						boost/parameter.hpp
						boost/phoenix.hpp
						boost/poly_collection/any_collection.hpp
						boost/polygon/polygon.hpp
						boost/pool/pool.hpp
						boost/preprocessor.hpp
						boost/process.hpp
						boost/program_options.hpp
						boost/property_map/property_map.hpp
                        boost/property_tree/ptree.hpp
                        boost/proto/proto.hpp
						boost/qvm/all.hpp
						boost/random.hpp
						boost/range.hpp
						boost/ratio.hpp
						boost/rational.hpp
						boost/ref.hpp
						boost/regex.hpp
						boost/safe_numerics/safe_integer.hpp
						boost/scope_exit.hpp
						boost/serialization/serialization.hpp
						boost/signals2.hpp
						boost/smart_ptr.hpp
						boost/sort/sort.hpp
						boost/spirit/include/classic.hpp
						boost/stacktrace.hpp
						boost/statechart/state.hpp
						boost/static_assert.hpp
						boost/swap.hpp
						boost/system/config.hpp
						boost/thread.hpp
						boost/throw_exception.hpp
						boost/timer.hpp
						boost/tokenizer.hpp
						boost/tti/tti.hpp
						boost/tuple/tuple.hpp
						boost/type_erasure/any.hpp
						boost/type_index.hpp
						boost/type_traits.hpp
						boost/typeof/typeof.hpp
						boost/units/unit.hpp
						boost/unordered_map.hpp
						boost/unordered_set.hpp
						boost/utility.hpp
						boost/uuid/uuid.hpp
						boost/variant.hpp
						boost/vmd/vmd.hpp
						boost/wave.hpp
						boost/xpressive/xpressive.hpp
						boost/yap/yap.hpp
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
