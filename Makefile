CC = g++
CFLAGS = -std=c++11 -Wall

all: run

run: cache.o cache_l2.o main.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cc
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o junk

