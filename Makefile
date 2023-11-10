# makefile for compiling all C++ files

# Compiler Version
CC = g++

# Compiler Flags
CFLAGS = -Wall -g -std=c++11

# Target
TARGET = qna_tool

# Object Files
OBJ = qna_tool.o Node.o tester.o search.o dict.o

# Header Files
HEADER = qna_tool.h Node.h search.h dict.h

# cpp Files
CPP = qna_tool.cpp Node.cpp tester.cpp search.cpp dict.cpp

# Compile
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)
	rm -f $(OBJ)

# Object Files
qna_tool.o: qna_tool.cpp
	$(CC) $(CFLAGS) -c qna_tool.cpp
dict.o: dict.cpp
	$(CC) $(CFLAGS) -c dict.cpp
search.o: search.cpp
	$(CC) $(CFLAGS) -c search.cpp

Node.o: Node.cpp
	$(CC) $(CFLAGS) -c Node.cpp

# Tester
tester.o: tester.cpp
	$(CC) $(CFLAGS) -c tester.cpp

# Clean
clean:
	rm -f $(OBJ) $(TARGET) *~

# Run
run:
	./$(TARGET)

# debug
debug:
	gdb ./$(TARGET)