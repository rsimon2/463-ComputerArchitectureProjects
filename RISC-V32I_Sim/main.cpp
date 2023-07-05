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

#include <iostream>
#include <sstream>
#include <unistd.h>
#include "hex.h"
#include "memory.h"
#include "rv32i_decode.h"
#include "rv32i_hart.h"
#include "registerfile.h"
#include "cpu_single_hart.h"

/**
 * Usage
 *
 * Delivers an error if a file cannot be opened.
 * 
 * @return stderr that a file cannot be opened.
 ****************************************************************************/

static void usage()
{
	std::cerr << "Usage : rv32i [-d ] [ -i] [-r] [- z] [-l exec - limit ] "
				<< "[-m hex - mem - size ] infile" << std::endl;
	std::cerr << "    -d show disassembly before program execution" << std::endl;
	std::cerr << "    -i show instruction printing during execution" << std::endl;
	std::cerr << "    -l maximum number of instructions to exec" << std::endl;
	std::cerr << "    -m specify memory size ( default = 0 x100 )" << std::endl;
	std::cerr << "    -r show register printing during execution" << std::endl;
	std::cerr << "    -z show a dump of the regs & memory after simulation" << std::endl;
	std::exit(1);
}

/**
 * Disassemble
 *
 * Loop that reads in instructions and prints the address, mnemonic and the
 * 		parameters of the instructions along with a 32-bit representation of
 *      the instructions
 * 
 * @return Mnemonic and parameters of a given instruction
 ****************************************************************************/

static void disassemble(const memory &mem)
{
	uint32_t insn = 0;

	while (insn < mem.get_size())
	{
		std::cout << hex::to_hex32(insn) << ": ";
		std::cout << hex::to_hex32(mem.get32(insn)) << " " 
			<< rv32i_decode::decode(insn, mem.get32(insn)) << std::endl;
		insn += 4;
	}
}

int main(int argc, char **argv)
{
	uint32_t memory_limit = 0x100; // default memory size = 256 bytes
	uint32_t execution_limit = 0;  // no limit (run forever)
	int opt;
	
	bool show_disassemble = false;
	bool show_instructions = false;
	bool show_registers = false;
	bool show_dump = false;
	
	/*
	rv32i_hart::set_show_disassemble(false);
	rv32i_hart::set_show_registers(false);
	rv32i_hart::set_show_instructions(false);
	rv32i_hart::set_show_dump(false);
	*/
	
	while ((opt = getopt(argc, argv, "m:")) != -1)
	{
		switch (opt)
		{
			case 'd':
			{
				show_disassemble = true;
			}
			break;
			
			case 'i':
			{
				show_instructions = true;
			}
			break;
			
			case 'l':
			{
				std::istringstream iss(optarg);
				iss >> std::hex >> execution_limit;
			}
			break;
			
			case 'm':
			{
				std::istringstream iss(optarg);
				iss >> std::hex >> memory_limit;
			}
			break;
			
			case 'r':
			{
				show_registers = true;
			}
			break;
			
			case 'z':
			{
				show_dump = true;
			}
			break;
			
		default: /* '?' */
			usage();
		}
	}

	if (optind >= argc)
		usage(); // missing filename

	memory mem(memory_limit);
	
	if (!mem.load_file(argv[optind]))
		usage();
	
	cpu_single_hart cpu(mem);

	if (show_disassemble == true)
	{
		disassemble(mem);
	}
	
	cpu.reset();
	
	cpu.set_show_instructions(show_instructions);
	
	cpu.set_show_registers(show_registers);
	
	if (show_dump == true)
	{
		cpu.dump();
		mem.dump();
	}
	
	return 0;
}