CXX = g++
CXXFLAGS = -g -std=c++0x -Wall
LEX_FILE = tvm.lex
OBJ = parse.o tvm.o lex.yy.o

main: $(OBJ)
	$(CXX) -o $@ main.cc $(OBJ) $(CXXFLAGS)
	
all: lex main test 
	
lex: 
	#flex $(LEX_FILE) -o lex.cc
	flex $(LEX_FILE) 

test: parse.test lex.test 

lex.test: lex.yy.o 
	$(CXX) -o $@ lex.test.cc lex.yy.o $(CXXFLAGS)

parse.test: $(OBJ) 
	$(CXX) -o $@ parse.test.cc $(OBJ) $(CXXFLAGS)

lex.yy.o: lex.h
tvm.o: lex.yy.o tvm.h
parse.o: tvm.o lex.yy.o parse.h lex.h

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.exe
