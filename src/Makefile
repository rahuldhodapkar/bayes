CC = clang++

all : predict

predict : predict.cpp histogram.o
	$(CC) $^ -o predict

histogram.o : histogram.cpp
	$(CC) -c $^ -o histogram.o



