# source files.
SRC = quickSort.c radixSort.c bucketSort.c main.c 
#SRC = quickSort.c radixSort.c bucketSort.c mainNodes.c 

OBJ = $(SRC:.cpp=.o)

OUT = sort

# include directories
INCLUDES = -I. quickSort.h bucketSort.h radixSort.h
 
# C compiler flags 
#CCFLAGS = -O0 -Wall
CCFLAGS = -O3 -Wall -fno-omit-frame-pointer -g -I/share/apps/papi/5.4.1/include -L/share/apps/papi/5.4.1/lib
#CCFLAGS = -O3 -Wall -fno-omit-frame-pointer -g

# compiler
CCC = gcc -std=c99
#CCC = /opt/intel/Compiler/11.1/073/bin/ia32/icpc 
#CCC = g++-4.5

# libraries
LIBS = -lm -lpapi -fopenmp -funroll-all-loops -ftree-vectorize -msse4.1
#LIBS = -lm -fopenmp -funroll-all-loops -ftree-vectorize -msse4.1

.SUFFIXES: .cpp .c 


default: $(OUT)

.cpp.o:
	$(CCC) $(CCFLAGS) $(INCLUDES)  -c $< -o $@

.c.o:
	$(CCC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

$(OUT): $(OBJ)
	$(CCC) -o $(OUT) $(CCFLAGS) $(OBJ) $(LIBS) 

depend:  dep
#
#dep:
#	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	rm -f *.o .a *~ Makefile.bak $(OUT)
	rm -f slurm*
	rm -f perf*
