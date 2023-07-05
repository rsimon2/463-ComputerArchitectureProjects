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
#include <iomanip>
#include "hex.h"

/**
 * Convert integer to 8 bit hex value
 *
 * This function returns a std::string with exactly 2 hex digits representing the 
 * 		8 bits of the i argument
 *
 * @param i Integer to be converted to hex
 *
 * @return Return std::string with exactly 2 hex digits representing 8 bits of i
 ************************************************************************************/
std::string hex::to_hex8(uint8_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
	return os.str();
}

/**
 * Convert integer to 16 bit hex value
 *
 * This function returns a std::string with exactly 4 hex digits representing the 
 * 		16 bits of the i argument
 *
 * @param i Integer to be converted to hex
 *
 * @return Return std::string with exactly 4 hex digits representing 16 bits of i
 ************************************************************************************/
std::string hex::to_hex32(uint32_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(8) << static_cast<uint32_t>(i);
	return os.str();
}

/**
 * Convert integer to 32 bit hex value
 *
 * This function returns a std::string with exactly 8 hex digits representing the 
 * 		32 bits of the i argument
 *
 * @param i Integer to be converted to hex
 *
 * @return Return std::string with exactly 8 hex digits representing 32 bits of i
 ************************************************************************************/
std::string hex::to_hex0x32(uint32_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(8) << static_cast<uint32_t>(i);
	return std::string("0x")+to_hex32(i);
}

/**
 * Convert integer to 20 bit hex value
 *
 * This function returns a std::string with exactly 5 hex digits representing the 
 * 		20 bits of the i argument
 *
 * @param i Integer to be converted to hex
 *
 * @return Return std::string with exactly 5 hex digits representing 20 bits of i
 ************************************************************************************/

std::string hex::to_hex0x20(uint32_t i)
{	
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(5) << static_cast<uint32_t>(i);
	return std::string("0x")+os.str();
}

/**
 * Convert integer to 12 bit hex value
 *
 * This function returns a std::string with exactly 3 hex digits representing the 
 * 		12 bits of the i argument
 *
 * @param i Integer to be converted to hex
 *
 * @return Return std::string with exactly 3 hex digits representing 12 bits of i
 ************************************************************************************/

std::string hex::to_hex0x12(uint32_t i)
{
	// This function must return a std::string beginning with 0x, followed by the 3 hex digits
	// representing the 12 least significant bits of the i argument. See to_hex0x20().
	
	// This new method will be used when formatting the csrrX() instructions.
	
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(3) << static_cast<uint16_t>(i);
	return std::string("0x")+os.str();
}