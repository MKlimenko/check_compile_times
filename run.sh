mkdir build
cd build

for i in {0..10}
do
	mkdir build_$i
	cd build_$i
	cmake -G Ninja ../..
	cmake --build .
	cd ..
done

cmake -G Ninja ..
cmake --build .
./CheckCompileTimes
