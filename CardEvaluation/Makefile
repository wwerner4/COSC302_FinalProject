# Makefile for Poker Hand Evaluator 
CC = g++
CFLAGS = -Wall -std=c++11

all: test_poker

# Compile the test program
test_poker: handEvaluatorTest.o handEvaluator.o
	$(CC) $(CFLAGS) -o test_poker handEvaluatorTest.o handEvaluator.o

# Compile the test file
handEvaluatorTest.o: handEvaluatorTest.cpp handEvaluator.h
	$(CC) $(CFLAGS) -c handEvaluatorTest.cpp

# Compile the hand evaluator
handEvaluator.o: handEvaluator.cpp handEvaluator.h
	$(CC) $(CFLAGS) -c handEvaluator.cpp

# Run the tests
run: test_poker
	./test_poker

# Clean up object files and executables
clean:
	rm -f *.o test_poker