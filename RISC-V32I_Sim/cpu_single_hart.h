#ifndef SINGLEHART_H
#define SINGLEHART_H

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
#include "rv32i_hart.h"
#include "registerfile.h"
#include "memory.h"

class cpu_single_hart : public rv32i_hart
{
public:
	cpu_single_hart(memory &mem) : rv32i_hart(mem) {}
	void run(uint64_t exec_limit);
};

#endif