# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++20

# Source files and object files
SRCS := lexer.cpp parser.cpp main.cpp
OBJS := $(SRCS:.cpp=.o)

# Output binary
TARGET := pl0

# Default target
all: build

# Build target
build: compile_commands.json $(TARGET)

# Link object files to create the binary
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJS) $(TARGET) compile_commands.json

# Run target
run: build
	./$(TARGET) test.pl0

# Generate compile_commands.json
compile_commands.json:
	bear --append -- make $(TARGET)

# Phony targets
.PHONY: all build clean run compile_commands.json

