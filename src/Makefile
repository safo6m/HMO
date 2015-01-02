CC = g++
CFLAGS = --std=gnu++11 -Wall -g
DEPS = GA.h Data.h Bitonic.h Constants.h
OBJ = hmo.cc GA.cc Data.cc Bitonic.cc
LIBS =

all: hmo

hmo: $(OBJ)
	$(CC) -o ../bin/$@ $^ $(CFLAGS) $(LIBS)

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

.PHONY: clean all

clean:
	rm -f *.o *~ ../bin/hmo
