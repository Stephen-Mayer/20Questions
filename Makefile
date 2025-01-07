#
# Mark's Magic Makefile
#
# Instructions:
#
# 1. Add .cpp files (not .h files) to the line beginning with SRCS = main.cpp.
#    Files should be separated with spaces, not commas.  If all of the
#    .cpp files in the directory should be included in the build, then you
#    can avoid maintaining a specific list of source files by commenting
#    out the first SRCS line and uncommenting the second.
# 2. Type 'make depend' to build or update the dependency list.  Do this
#    whenever you add this Makefile or add a .cpp or .h file to a project.
# 3. Type 'make' to build the project.
# 4. Type 'make clean' to start over or prepare for submission.  This does
#    not remove the dependency information.
#
# N.B. If you're using files with .cc extensions, then you need to search
# and replace cpp with cc.
#

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
