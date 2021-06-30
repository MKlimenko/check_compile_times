conan install boost/1.76.0@ --build=never -if build/boost -s build_type=Debug -g CMakeDeps
cd build

for i in {0..9}
do
	mkdir build_$i
	cd build_$i
	cmake -G Ninja ../.. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=$PWD/../boost
	cmake --build .
	cd ..
done

cmake -G Ninja .. -DSKIP_EVALUATION=true -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=$PWD/boost
cmake --build .
./CheckCompileTimes
