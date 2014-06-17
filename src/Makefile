CC = clang++
CFLAGS = -g

all : predict

predict : predict.o histogram.o datagen.o bin_run.o
	$(CC) $(CFLAGS) $^ -o predict

predict.o : predict.cpp
	$(CC) $(CFLAGS) -c $^ -o predict.o

histogram.o : histogram.cpp
	$(CC) $(CFLAGS) -c $^ -o histogram.o

datagen.o : datagen.cpp
	$(CC) $(CFLAGS) -c $^ -o datagen.o

bin_run.o : bin_run.cpp
	$(CC) $(CFLAGS) -c $^ -o bin_run.o
