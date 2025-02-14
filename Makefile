//To make type 

CC = g++
CFLAGS = -pg -Wall -std=c++11
SRCS = main.cpp Animal.cpp AnimalDatabase.cpp QuestionDatabase.cpp Question.cpp Matrix.cpp NeuralNetwork.cpp Attribute.cpp GameInterface.cpp
#SRCS = ${wildcard *.cpp}
OBJS = ${SRCS:.cpp=.o}
INCLS = ${SRCS:.cpp=.h} main.h Animal.h AnimalDatabase.h QuestionDatabase.h Question.h Matrix.h NeuralNetwork.h Attribute.h GameInterface.h
#INCLS = ${wildcard *.h}

20Questions: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o 20Questions

$(OBJS):
	$(CC) $(CFLAGS) -c $*.cpp

depend: Makefile.dep
	$(CC) -MM $(SRCS) > Makefile.dep

Makefile.dep:
	touch Makefile.dep

.PHONY: submit clean

submit:
	rm -f submit.zip
	zip submit.zip $(SRCS) $(INCLS) HONOR Makefile Makefile.dep
	
clean:
	rm -f $(OBJS) a.out core

include Makefile.dep
