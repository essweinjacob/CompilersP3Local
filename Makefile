CC = g++
localSupport.c: CXXFlags += -w
CFLAGS = -Wall -g -std=c++11

MASTER_SRC = main.cpp
SRC =	token.h parser.h printTree.h scanner.h localSupport.h
AUX = node.h
MASTER_OBJ = $(MASTER_SRC:.cpp=.o)
OBJ = $(SRC:.h=.o)

MASTER = statSem

OUTPUT = $(MASTER)
all: $(OUTPUT)


%.o: %.cpp $(SRC) $(AUX)
	$(CC) $(CFLAGS) -c $< -o $@

$(MASTER): $(MASTER_OBJ) $(OBJ)
	$(CC) $(CFLAGS) $(MASTER_OBJ) $(OBJ) -o $(MASTER)


.PHONY: clean
clean:
	/bin/rm -f $(OUTPUT) *.o *.dat *.inorder *.preorder *.postorder out
