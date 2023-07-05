#
# PROGRAM: Simulated RISC-V
# AUTHOR: Rick Simon
#

CXXFLAGS = -g -ansi -pedantic -Wall -Werror -std=c++14

rv32i: main.o hex.o memory.o rv32i_decode.o registerfile.o rv32i_hart.o cpu_single_hart.o
	g++ $(CXXFLAGS) -o rv32i main.o hex.o memory.o rv32i_decode.o registerfile.o rv32i_hart.o cpu_single_hart.o

rv32i.o: main.cpp hex.h hex.cpp memory.h memory.cpp rv32i_decode.h rv32i_decode.cpp registerfile.h registerfile.cpp rv32i_hart.h rv32i_hart.cpp cpu_single_hart.h cpu_single_hart.cpp
	g++ $(CXXFLAGS) -c main.cpp hex.cpp memory.cpp rv32i_decode.cpp registerfile.cpp rv32i_hart.cpp cpu_single_hart.cpp

clean:
	rm -f *.o rv32i
