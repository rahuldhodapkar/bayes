CC = clang++
CFLAGS = -std=c++11

all : predict

predict : predict.o histogram.o datagen.o run.o
	$(CC) $(CFLAGS) $^ -o predict

predict.o : predict.cpp
	$(CC) $(CFLAGS) -c $^ -o predict.o

histogram.o : histogram.cpp
	$(CC) $(CFLAGS) -c $^ -o histogram.o

datagen.o : datagen.cpp
	$(CC) $(CFLAGS) -c $^ -o datagen.o

run.o : run.cpp
	$(CC) $(CFLAGS) -c $^ -o run.o
