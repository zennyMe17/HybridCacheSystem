# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS = -pthread

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Find all .cpp files in the source directory
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
# Replace the .cpp extension with .o and put them in the obj directory
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Name of the final executable
TARGET = $(BINDIR)/hybrid_cache

# Default target: build the executable
all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Linking complete. Executable is at $(TARGET)"

# Rule to compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled $< into $@"

# Clean up build files
clean:
	@echo "Cleaning up build artifacts..."
	@rm -rf $(OBJDIR) $(BINDIR)

# Phony targets don't represent actual files
.PHONY: all clean
