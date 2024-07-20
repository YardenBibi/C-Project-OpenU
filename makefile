# Compilation macros
 CC = gcc
 CFLAGS = -ansi -Wall -pedantic -g # Flags
 GLOBAL_DEPS = globals.h # Dependencies for everything
 EXE_DEPS = assembler.o  common.o  handle_macr.o handle_errors.o # Deps for exe

 ## Executable
assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@

assembler.o:  ../SourceFiles $(GLOBAL_DEPS)
	$(CC) -c assembler.c $(CFLAGS) -o $@

common.o: ../SourceFiles Header $(GLOBAL_DEPS)
	$(CC) -c preproc.c $(CFLAGS) -o $@

handle_macr.o: ../SourceFiles Header $(GLOBAL_DEPS)
	$(CC) -c first_pass.c $(CFLAGS) -o $@

handle_errors.o: ../SourceFiles Header $(GLOBAL_DEPS)
	$(CC) -c second_pass.c $(CFLAGS) -o $@

clean:
	rm -rf *.o *.am *.ob *.ent *.ext