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


# # Compilation macros
#  CC = gcc
#  HEADRES = HeaderFiles
#  SOURCES = SourceFiles
#  CFLAGS = -ansi -Wall -pedantic -g # Flags
#  GLOBAL_DEPS = HeaderFiles/globals.h # Dependencies for everything
#  EXE_DEPS = assembler.o  common.o  handle_macr.o handle_errors.o assembly_utils.o macr_node.o # Deps for exe

#  ## Executable
# assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
# 	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@

# assembler.o:  $(SOURCES)/assembler.c $(GLOBAL_DEPS)
# 	$(CC) -c $(SOURCES)/assembler.c $(CFLAGS) -o $@

# common.o: $(SOURCES)/common.c $(HEADRES)/common.h $(GLOBAL_DEPS)
# 	$(CC) -c $(SOURCES)/common.c $(CFLAGS) -o $@

# handle_macr.o: $(SOURCES)/handle_macr.c $(HEADRES)/handle_macr.h $(GLOBAL_DEPS)
# 	$(CC) -c $(SOURCES)/handle_macr.c $(CFLAGS) -o $@

# handle_errors.o: $(SOURCES)/handle_errors.c $(HEADRES)/handle_errors.h $(GLOBAL_DEPS)
# 	$(CC) -c $(SOURCES)/handle_errors.c $(CFLAGS) -o $@

# assembly_utils.o: $(SOURCES)/assembly_utils.c $(HEADRES)/assembly_utils.h $(GLOBAL_DEPS)
# 	$(CC) -c $(SOURCES)/assembly_utils.c $(CFLAGS) -o $@

# macr_node.o: $(SOURCES)/macr_node.c $(HEADRES)/macr_node.h $(GLOBAL_DEPS)
# 	$(CC) -c $(SOURCES)/macr_node.c $(CFLAGS) -o $@

# clean:
# 	rm -rf *.o *.am *.ob *.ent *.ext
