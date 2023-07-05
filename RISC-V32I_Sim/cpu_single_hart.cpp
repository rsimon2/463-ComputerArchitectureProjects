//***************************************************************************
//*                                                                         *
//*  Rick Simon                                                             *
//*  Z1944495                                                               *
//*  CSCI 463 PE1                                                           *
//*                                                                         *
//*  I certify that this is my own work and where appropriate an extension  *
//*  of the starter code provided for the assignment.                       *
//*                                                                         *
//***************************************************************************

#include "cpu_single_hart.h"

void run(uint64_t exec_limit)
{
	/*
		Since code that executes on this simulator has no (practical) way to determine how much memory the
		machine has, set register x2 to the memory size (get it with mem.get_size()) before executing any
		instructions in your run() method. Note that the number of bytes in the memory is also the address
		of the first byte past the end of the simulated memory.2
		If the exec_limit parameter is zero, call tick() in a loop until the is_halted() returns true.
		If the exec_limit parameter is not zero then enter a loop that will call tick() until is_halted()
		returns true or exec_limit number of instructions have been executed.
		If the hart becomes halted then print a message indicating why by using get_halt_reason() to get
		the reason message.
		Regardless of why the execution has terminated, print the number of instructions that have been
		executed by using get_insn_counter().
		For example running the simulator with an execution limit of 2, dumps enabled by the -ir commandline options, and simulating the allinsns5.bin example program will result in the output shown in
		Figure 5
	*/
	regs.set(2, mem.get_size())
	
	while (true)
	{
		if (exec_limit && insn_counter >= exec_limit)
		{
			break;
		}
		
		if (is_halted == true)
		{
			break;
		}
		
		tick();
	}
	
	std::cout << rv32i_hart::get_insn_counter(insn_counter) << std::endl;
}