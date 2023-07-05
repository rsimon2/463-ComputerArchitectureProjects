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

#include <cctype> // isprint()
#include <stack> // stack operations
#include <iomanip> // setw, setfill
#include <fstream> // ifstream
#include "memory.h"
#include "hex.h"

/**
 * Allocate and initialize vector elements
 *
 * Allocate siz bytes in the mem vector and initialize every byte/element to 0xa5.
 *
 * @param siz Number of bytes to allocate in the mem vector
 ************************************************************************************/
memory::memory(uint32_t siz)
{
	siz = (siz + 15) & 0xfffffff0; // round the length up, mod-16
	mem.resize(siz, 0xa5);
}

/**
 * Destructor
 * 
 * Clean up anything necessary.
 ************************************************************************************/
memory::~memory()
{}

/**
 * Check if address is in range.
 * 
 * Checks if a given address is in the mem vector.
 *
 * @param i Address given address to check against mem vector range.
 *
 * @return Return true if the given address i does not represent an element that is 
 *		present in mem vector. Otherwise return false
 *
 * @note If given address is not in range of valid addresses of simulated memory, 
 *		print warning message to stdout and return true
 ************************************************************************************/
bool memory::check_illegal(uint32_t i) const
{
	if (i > memory::get_size())
	{
		std::cout << "WARNING: Address out of range: " << hex::to_hex0x32(i) << std::endl;
		
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * Size of vector
 * 
 * @return Return the rounded up number of bytes in simulated memory
 ************************************************************************************/
uint32_t memory::get_size() const
{
	return mem.size();
}

/**
 * Return value of byte at given address
 *
 * Checks if an address is in range of mem vector and returns the value at that
 * 		address if it exists
 *
 * @param addr Given address from which to check and retrieve value
 *
 * @return If address exists, return the value at that address in simulated memory,
 * 		if it does not exist, return 0
 *
 * @note Only code that will ever read values from mem vector
 ************************************************************************************/
uint8_t memory::get8(uint32_t addr) const
{
	if (check_illegal(addr) == false)
	{
		return mem[addr];
	}
	else
	{
		return 0;
	}
}

/**
 * Return value of bytes at given address
 *
 * Calls get8() two times to return the bytes stored at the address given
 *
 * @param addr Given address from which to check and retrieve value
 *
 * @return If get8() is successful, returns the bytes from the given address in 
 *		little endian order
 *
 * @note Job of validating addrs of 2 bytes will be taken care of in get8(), not here
 ************************************************************************************/
uint16_t memory::get16(uint32_t addr) const
{
	uint8_t b1 = get8(addr);
	uint8_t b2 = get8(addr + 1);
	
	return ((uint16_t) b2 << 8) | b1;
}

/**
 * Return value of bytes at given address
 *
 * Calls get16() two times to return the bytes stored at the address given
 *
 * @param addr Given address from which to check and retrieve value
 *
 * @return If get16() is successful, returns the bytes from the given address in 
 *		little endian order
 *
 * @note Job of validating addrs of 2 bytes will be taken care of in get8(), not here
 ************************************************************************************/
uint32_t memory::get32(uint32_t addr) const
{
	uint16_t b1 = get16(addr);
	uint16_t b2 = get16(addr + 2);
	
	return ((uint32_t) b2 << 16) | b1;
}

/**
 * Return value of bytes at given address, sign extended
 *
 * Calls get8() and return as a sign extended 32-bit integer
 *
 * @param addr Given address from which to check and retrieve value
 *
 * @return Return value retrieved with get8() as sign extended integer
 ************************************************************************************/
int32_t memory::get8_sx(uint32_t addr) const
{
	return (int32_t)get8(addr);
}

/**
 * Return value of bytes at given address, sign extended
 *
 * Calls get16() and return as a sign extended 32-bit integer
 *
 * @param addr Given address from which to check and retrieve value
 *
 * @return Return value retrieved with get16() as sign extended integer
 ************************************************************************************/
int32_t memory::get16_sx(uint32_t addr) const
{
	return (int32_t)get16(addr);
}

/**
 * Return value of bytes at given address, sign extended
 *
 * Calls get32() and return as a sign extended 32-bit integer
 *
 * @param addr Given address from which to check and retrieve value
 *
 * @return Return value retrieved with get32() as sign extended integer
 ************************************************************************************/
int32_t memory::get32_sx(uint32_t addr) const
{
	return get32(addr);
}

/**
 * Set byte in simulated memory to given value
 *
 * After verifying the address is in range of the mem vector, sets the byte in the 
 *		vector to the given value
 *
 * @param addr Given address from which to check and to write to
 * @param val Given value to insert into vector at given address
 *
 * @note This and constructor are only code that will ever write vals into mem vector
 ************************************************************************************/
void memory::set8(uint32_t addr, uint8_t val)
{
	check_illegal(addr);
	if (check_illegal(addr) == false)
	{
		mem[addr] = val;
	}
}

/**
 * Set byte in simulated memory to given value
 *
 * Call set8() twice, store given val in little endian order into simulated memory
 * starting at address given in addr argument
 *
 * @param addr Given address from which to check and to write to
 * @param val Given value to insert into vector at given address
 ************************************************************************************/
void memory::set16(uint32_t addr, uint16_t val)
{ 
	uint16_t b1 = ((val & 0xFF00) >> 8);
	uint16_t b2 = (val & 0x00FF);
	
	set8((addr + 1), b1);
	set8(addr, b2);
}

/**
 * Set byte in simulated memory to given value
 *
 * Call set16() twice, store given val in little endian order into simulated memory
 * starting at address given in addr argument
 *
 * @param addr Given address from which to check and to write to
 * @param val Given value to insert into vector at given address
 ************************************************************************************/
void memory::set32(uint32_t addr, uint32_t val)
{
	uint32_t b1 = ((val & 0xFFFF0000) >> 16);
	uint32_t b2 = (val & 0x0000FFFF);
	
	set16((addr + 2), b1);
	set16(addr, b2);
}

/**
 * Dump contents of simulated memory
 *
 * Dump entire contents of simulated memory in hex w/ corresponding ASCII characters
 *		on the right
 *
 * @note isprint determines if a charcter is printable and prints either a character
 * 		or a '.'
 ************************************************************************************/
void memory::dump() const
{
	std::string asciiDump;
	for (uint32_t i = 0; i < get_size(); ++i)
	{
		if (i % 16 == 0)
		{
			std::cout << std::setw(8) << std::setfill('0') << hex::to_hex32(i) << ": ";
		}
		
		std::cout << hex::to_hex8(get8(i)) << " ";
		
		uint8_t ch = get8(i);
		ch = isprint(ch) ? ch : '.';
		asciiDump += ch;
		
		if (i % 16 == 7)
		{
			std::cout << " ";
		}
		if (i % 16 == 15)
		{
			std::cout << "*" << asciiDump << "*" << std::endl;
			asciiDump.clear();
		}
	}
}

/**
 * Open file named fname in binary mode
 *
 * @param fname The file to be be read into simulated memory
 *
 * @return If file can't be opened print error message to stderr and return false
 * 		otherwise close file and return true
 *
 * @note 
 ************************************************************************************/
bool memory::load_file(const std::string &fname)
{
	std::ifstream infile(fname, std::ios::in|std::ios::binary);
	if (infile.is_open())
	{
		uint8_t i;
		infile >> std::noskipws;
		for (uint32_t addr = 0; infile >> i; ++addr)
		{
			check_illegal(addr);
			if (check_illegal(addr) == true)
			{
				std::cerr << "Program too big." << std::endl;
				
				return false;
			}
			set8(addr, i);
		}
		return true;
	}
	else
	{
		std::cerr << "Can't open file '" << fname << "' for reading." << std::endl;
		
		return false;
	}
	
	return true;
}