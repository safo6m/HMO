CC = g++
CFLAGS = --std=gnu++11 -Wsign-compare -Wall -g
DEPS = GA.h Data.h Bitonic.h FirstFit.h Constants.h
OBJ = hmo.cc GA.cc Data.cc Bitonic.cc FirstFit.cc
OBJ_G = greedy.cc 
LIBS =

all: hmo greedy_hmo

greedy_hmo: $(OBJ_G)
	$(CC) -o ../bin/$@ $^ $(CFLAGS) $(LIBS)

hmo: $(OBJ)
	$(CC) -o ../bin/$@ $^ $(CFLAGS) $(LIBS)

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

.PHONY: clean all

clean:
	rm -f *.o *~ ../bin/hmo
