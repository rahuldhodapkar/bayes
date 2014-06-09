CC = clang++

all : predict

predict : predict.o histogram.o datagen.o
	$(CC) $^ -o predict

predict.o : predict.cpp
	$(CC) -c $^ -o predict.o

histogram.o : histogram.cpp
	$(CC) -c $^ -o histogram.o

datagen.o : datagen.cpp
	$(CC) -c $^ -o datagen.o
