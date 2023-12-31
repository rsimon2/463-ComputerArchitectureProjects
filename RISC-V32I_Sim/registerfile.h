#ifndef REGISTERFILE_H
#define REGISTERFILE_H

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
#include <string>
#include <vector>
#include "hex.h"
#include "rv32i_decode.h"

class registerfile : public rv32i_decode
{
public:
	void reset();
	void set(uint32_t r, int32_t val);
	int32_t get(uint32_t r) const;
	void dump(const std::string &hdr) const;

private:
	std::vector<int32_t> regs;
};

#endif