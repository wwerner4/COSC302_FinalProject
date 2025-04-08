# Makefile for the CardDeck and its corresponding tests
CXX=		g++
CXXFLAGS=	-g -Wall -std=gnu++11
LDFLAGS=	
SHELL=		bash
PROGRAMS=	cardDeckTest
SOURCES=	cardDeckTest.cpp cardDeck.cpp
OBJECTS=	$(SOURCES:.cpp=.o)

all:		$(PROGRAMS)

%.o:		%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $^

cardDeckTest:	$(OBJECTS) 
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS) -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f $(PROGRAMS) $(OBJECTS)