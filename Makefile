CC = g++
CFLAGS = --std=c++17 -Wall -Werror -pedantic -g
LIB = -lboost_regex -lboost_date_time -lboost_unit_test_framework

DEPS =

OBJECTS = main.o

PROGRAM = ps7

.PHONY: all clean lint

all: $(PROGRAM)

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

clean:
	rm *.o $(PROGRAM)

lint:
	cpplint *.cpp *.hpp
