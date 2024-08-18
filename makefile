# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -ansi -pedantic -g

# Directories
SRCDIR = SourceFiles
OBJDIR = obj

# Source and Object Files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Executable
EXECUTABLE = assembler

# Default Target
all: $(EXECUTABLE)

# Rule to create the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $@

# Rule to create object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(EXECUTABLE)
