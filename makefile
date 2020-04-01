ProgramName=findPrimes
CC=clang

UNAME := $(shell uname)
# OS X specific configuration
SharedFlags=-std=c11
ifeq ($(UNAME), Darwin)
	SharedFlags+=-Xpreprocessor -fopenmp -lomp -I"$(shell brew --prefix libomp)/include" -L"$(shell brew --prefix libomp)/lib"
else
	SharedFlags+=-fopenmp
endif
DebugFlags=$(SharedFlags) -Wall -g3 -pedantic -D DEBUG
ReleseFlags=$(SharedFlags) -Ofast -g0 -Wno-everything

Objects=

release: CompileFlags=$(ReleseFlags)
release: $(Objects)
	$(CC) $(CompileFlags) -o $(ProgramName) $(Objects) main.c

rebuild: clean release

debug: CompileFlags=$(DebugFlags)
debug: $(Objects)
	$(CC) $(CompileFlags) -o $(ProgramName) $(Objects) main.c

clean:
	rm -rf *.dSYM
	rm -f *.o

%.o : %.c %.h
	$(MPICC) $(CompileFlags) -c $< -o $@