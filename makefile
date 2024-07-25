# Compilation macros
 CC = gcc
 HEADRES = HeaderFiles
 SOURCES = SourceFiles
 CFLAGS = -ansi -Wall -pedantic -g # Flags
 GLOBAL_DEPS = HeaderFiles/globals.h # Dependencies for everything
 EXE_DEPS = assembler.o  common.o  handle_macr.o handle_errors.o # Deps for exe

 ## Executable
assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
    $(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@

assembler.o:  $(SOURCES)/assembler.c $(GLOBAL_DEPS)
    $(CC) -c $(SOURCES)/assembler.c $(CFLAGS) -o $@

common.o: $(SOURCES)/common.c $(HEADRES)/common.h $(GLOBAL_DEPS)
    $(CC) -c $(SOURCES)/common.c $(CFLAGS) -o $@

handle_macr.o: $(SOURCES)/handle_macr.c $(HEADRES)/handle_macr.h $(GLOBAL_DEPS)
    $(CC) -c $(SOURCES)/handle_macr.c $(CFLAGS) -o $@

handle_errors.o: $(SOURCES)/handle_errors.c $(HEADRES)/handle_errors.h $(GLOBAL_DEPS)
    $(CC) -c $(SOURCES)/handle_errors.c $(CFLAGS) -o $@

clean:
    rm -rf *.o *.am *.ob *.ent *.ext
