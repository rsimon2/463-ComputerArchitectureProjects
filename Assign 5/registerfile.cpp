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

#include "registerfile.h"

void registerfile::reset()
{
	// Initialize register x0 to zero, and all other registers to 0xf0f0f0f0.
	regs.push_back(0x00000000);
	
	for (int i = 1; i < 32; i++)
	{
		regs.push_back(0xf0f0f0f0);
	}
}

void registerfile::set(uint32_t r, int32_t val)
{
	// Assign register r the given val. If r is zero then do nothing.
	if (r != 0)
	{
		regs[r] = val;
	}
}

int32_t registerfile::get(uint32_t r) const
{
	// Return the value of register r. If r is zero then return zero.
	if (r == 0)
	{
		return 0;
	}
	
	return regs[r];
}

void registerfile::dump(const std::string &hdr) const
{
	// Implement a dump of the registers. The hdr parameter is a string that must be printed at the begining
	// 		of the output lines.
	// Inherit the hex class and use its hex32() utility function to simplify printing the register values!
	for (uint32_t i = 0; i < regs.size(); ++i)
	{
		if (i % 8 == 0)
		{
			std::cout << hdr << rv32i_decode::render_reg(i) << ": ";
		}
		
		std::cout << hex::to_hex32(regs[i]) << " ";
		
		if (i % 8 == 3)
		{
			std::cout << " ";
		}
	}
}