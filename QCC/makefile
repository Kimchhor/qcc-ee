FLAG=`../build/bin/llvm-config --cxxflags --ldflags --system-libs --libs all`
LIB=`--cxxflags --system-libs --libs all`
GTEST=/usr/lib/libgtest.a /usr/lib/libgtest_main.a

ARGS=./res/machine_description.json ./tst/cnot.ll

all:
	g++ -g ./src/test.cpp -o ./src/test -fexceptions  ${FLAG} -std=c++17 

execute : all
	./src/test ${ARGS}

playground: 
	g++ -g ./src/playground.cpp -o ./src/playground ${FLAG}


# .PHONY: main
# main:
# 	clear;
# 	clang++ -Wall -std=c++11 main.cpp -o main -B --always-make
# 	./main

# 	# opt -O3 -S bin/main.ll -o bin/main_03.ll

clean:
	rm -rf bin/main_03.ll
	rm -rf bin/main.ll
	rm -rf bin/main.bc

run:
	clear;
	llvm-as cnot.ll -o cnot.bc

back:
	llvm-dis cnot.bc -o back.ll
.PHONY: test

test:
	g++ -g ./test/gtest.cpp -o ./test/gtest  ${GTEST} -pthread -std=c++17 
	clear
	cd ./test; ./gtest


.PHONY: opt
opt:
	opt -S main.ll