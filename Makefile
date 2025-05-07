# Makefile for the CardDeck and its corresponding tests
CXX=		g++
CXXFLAGS=	-g -Wall -std=gnu++11
LDFLAGS=	
SHELL=		bash
PROGRAMS=	playGame
SOURCES=	playGame.cpp cardDeck.cpp gameGraphics.cpp gameState.cpp CardEvaluation/handEvaluator.cpp ai.cpp
OBJECTS=	$(SOURCES:.cpp=.o)

all:		$(PROGRAMS)

%.o:		%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $^

playGame:	$(OBJECTS) 
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS) -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f $(PROGRAMS) $(OBJECTS)
