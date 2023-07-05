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

#include "rv32i_hart.h"

void rv32i_hart::reset()
{
	// Reset the rv32i object and the registerfile.
	// To reset a hart:
	// – Set the pc register to zero.
	// – Call regs.reset() to reset the register values.
	// – Set the insn_counter to zero.
	// – Set the the halt flag to false.
	// – Set the the halt_reason to "none".
	
	pc = 0;
	regs.reset();
	insn_counter = 0;
	halt = false;
	halt_reason = "none";
}

//void rv32i_hart::dump(const std::string &hdr="") const
void rv32i_hart::dump(const std::string &hdr) const
{
	/* Dump the entire state of the hart. Prefix each line printed by the given hdr string (the default being
		to not print any prefix.) It will dump the GP-regs (making use of the regs member variable by calling
		regs.dump(hdr)) and then add a dump of the PC register*/
		regs.dump(hdr);
		std::cout << " pc " << hex::to_hex32(pc) << std::endl;
}

//void rv32i_hart::tick(const std::string &hdr="")
void rv32i_hart::tick(const std::string &hdr)
{
/*The tick() method function is how to tell the simulator to execute and instruction. The hdr parameter
is required to be printed on the left of any and all output that is displayed as a result of calling this
method.
If the hart is halted then return immediately without doing anything. Otherwise, simulate the execution
of one single instruction:
– If show_registers is true then dump the state of the hart with the given hdr.
– If the pc register is not a multiple of 4 then set the halt flag to true, the halt_reason to
"PC alignment error", and return without further processing.
– Increment the insn_counter variable (not the pc register.)
– Fetch an instruction from the memory at the address in the pc register.
– If show_instructions is true then
∗ Print the hdr, the pc register (in hex), and the 32-bit fetched instruction (in hex).
∗ Call exec(insn, &std::cout) to execute the instruction and render the instruction and
simulation details.
– else
∗ Call exec(insn, nullptr) to execute the instruction without rendering anything.
Note that the reset() and tick() methods are the only way to change the state of the simulated
hart hardware. (Which is similar to but not to be confused with changing the state of the C++
rv32i_hart object! For example, the notion of calling set_show_instructions() can change the
state of the rv32i_hart object. But it does not change the state of the simulated hart hardware.)*/
	
	if (show_registers == true)
	{
		dump(hdr);
	}
	
	if (pc % 4 != 0)
	{
		halt = true;
		halt_reason = "PC alignment error";
	}
	
	insn_counter++;
	
	uint32_t insn = mem.get32(pc);
	
	if (show_instructions == true)
	{
		std::cout << hdr << " " << hex::to_hex32(pc) << ": " << hex::to_hex32(insn) << " ";
		exec(insn, &std::cout);
	}
	else
	{
		exec(insn, nullptr);
	}
}

void rv32i_hart::exec(uint32_t insn, std::ostream* pos)
{
	uint32_t funct3 = get_funct3(insn);
	uint32_t funct7 = get_funct7(insn);
	
		
	if (insn == insn_ecall)
	{
		return exec_ecall(insn, pos);
	}
	else if (insn == insn_ebreak)
	{
		return exec_ebreak(insn, pos);
	}
	else
	{
		switch(get_opcode(insn))
		{
			default: 							exec_illegal_insn(insn, pos); return;
			case opcode_lui: 					exec_lui(insn, pos); return;
			case opcode_auipc:					exec_auipc(insn, pos); return;
			case opcode_jal:					exec_jal(insn, pos); return;
			case opcode_jalr:					exec_jalr(insn, pos); return;
			case opcode_btype:
				switch(funct3)
				{
					default:					exec_illegal_insn(insn, pos); return;
					case funct3_beq:			exec_beq(insn, pos); return;
					case funct3_bne:			exec_bne(insn, pos); return;
					case funct3_blt:			exec_blt(insn, pos); return;
					case funct3_bge:			exec_bge(insn, pos); return;
					case funct3_bltu:			exec_bltu(insn, pos); return;
					case funct3_bgeu:			exec_bgeu(insn, pos); return;
				}
				assert(0 && "unrecognized funct3"); // impossible
			case opcode_load_imm:
				switch(funct3)
				{
					default:					exec_illegal_insn(insn, pos); return;
					case funct3_lb:				exec_lb(insn, pos); return;
					case funct3_lh:				exec_lh(insn, pos); return;
					case funct3_lw:				exec_lw(insn, pos); return;
					case funct3_lbu:			exec_lbu(insn, pos); return;
					case funct3_lhu:			exec_lhu(insn, pos); return;
				}
				assert(0 && "unrecognized funct3"); // impossible
			case opcode_stype:
				switch(funct3)
				{
					default:					exec_illegal_insn(insn, pos); return;
					case funct3_sb:				exec_sb(insn, pos); return;
					case funct3_sh:				exec_sh(insn, pos); return;
					case funct3_sw:				exec_sw(insn, pos); return;
				}
				assert(0 && "unrecognized funct3"); // impossible
			case opcode_alu_imm:
				switch(funct3)
				{
					default:					exec_illegal_insn(insn, pos); return;
					case funct3_add:			exec_addi(insn, pos); return;
					case funct3_slt:			exec_slti(insn, pos); return;
					case funct3_sltu:			exec_sltiu(insn, pos); return;
					case funct3_xor:			exec_xori(insn, pos); return;
					case funct3_or:				exec_ori(insn, pos); return;
					case funct3_and:			exec_andi(insn, pos); return;
					case funct3_sll:			exec_slli(insn, pos); return;
					case funct3_srx:
						switch(funct7)
						{
							default:			exec_illegal_insn(insn, pos); return;
							case funct7_sra:	exec_srai(insn, pos); return;
							case funct7_srl:	exec_srli(insn, pos); return;
						}
						assert(0 && "unrecognized funct7"); // impossible
				}
				assert(0 && "unrecognized funct3"); // impossible
			case opcode_rtype:
				switch(funct3)
				{
					default:					exec_illegal_insn(insn, pos); return;
					case funct3_add:
						switch(funct7)
						{
							default:			exec_illegal_insn(insn, pos); return;
							case funct7_add:	exec_add(insn, pos); return;
							case funct7_sub:	exec_sub(insn, pos); return;
						}
						assert(0 && "unrecognized funct7"); // impossible
					case funct3_slt:			exec_slt(insn, pos); return;
					case funct3_sltu:			exec_sltu(insn, pos); return;
					case funct3_xor:			exec_xor(insn, pos); return;
					case funct3_or:				exec_or(insn, pos); return;
					case funct3_and:			exec_and(insn, pos); return;
					case funct3_sll:			exec_sll(insn, pos); return;
					case funct3_srx:
						switch(funct7)
						{
							default:			exec_illegal_insn(insn, pos); return;
							case funct7_sra:	exec_sra(insn, pos); return;
							case funct7_srl:	exec_srl(insn, pos); return;
						}
						assert(0 && "unrecognized funct7"); // impossible
					assert(0 && "unrecognized funct3"); // impossible
				}
			case opcode_system:
				switch(funct3)
				{
					default:					exec_illegal_insn(insn, pos); return;
					case funct3_csrrw:			exec_csrrw(insn, pos); return;
					case funct3_csrrs:			exec_csrrs(insn, pos); return;
					case funct3_csrrc:			exec_csrrc(insn, pos); return;
					case funct3_csrrwi:			exec_csrrwi(insn, pos); return;
					case funct3_csrrsi:			exec_csrrsi(insn, pos); return;
					case funct3_csrrci:			exec_csrrci(insn, pos); return;
				}
				assert(0 && "unrecognized funct3"); // impossible
		}
	}
	assert(0 && "unrecognized opcode"); // It should be impossible to ever get here!
}

void rv32i_hart::exec_illegal_insn(uint32_t insn, std::ostream* pos)
{
	if (pos)
	{
		*pos << render_illegal_insn (insn);
	}
	
	halt = true ;
	halt_reason = "Illegal instruction";
}

void rv32i_hart::exec_ecall(uint32_t insn, std::ostream *pos)
{
	if (pos)
	{
		std::string s = render_ecall(insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// HALT";
	}
	
	halt = true;
	halt_reason = "ECALL instruction";
}

void rv32i_hart::exec_ebreak(uint32_t insn, std::ostream *pos)
{
	if (pos)
	{
		std::string s = render_ebreak(insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// HALT";
	}
	
	halt = true;
	halt_reason = "EBREAK instruction";
}

void rv32i_hart::exec_lui(uint32_t insn, std::ostream *pos)
{
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);
	
 	if (pos)
	{
		std::string s = render_lui(insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(imm_u);
	}
	
	pc += 4;
}

void rv32i_hart::exec_auipc(uint32_t insn, std::ostream *pos)
{
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);
	
	int32_t val = pc + imm_u;
	
 	if (pos)
	{
		std::string s = render_auipc(insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc) 
			<< hex::to_hex0x32(imm_u) << hex::to_hex0x32(val);
	}
	
	pc += 4;
}

void rv32i_hart::exec_jal(uint32_t insn, std::ostream *pos)
{
	uint32_t rd = get_rd(insn);
	int32_t pcrel_21 = get_imm_j(insn);
	
	uint32_t nextPC = pc + 4;
	int32_t val = pc + pcrel_21;
	
	if (pos)
	{
		std::string s = render_jal(pc, insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(nextPC) << "pc = "
			<< hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(pcrel_21) << " = " 
			<< hex::to_hex0x32(val);
	}
	
	pc += val;
}

void rv32i_hart::exec_jalr(uint32_t insn, std::ostream *pos)
{
	uint32_t rd = get_rd(insn);
	int32_t imm_i = get_imm_i(insn);
	uint32_t rs1 = get_rs1(insn);
	
	uint32_t nextPC = pc + 4;
	int32_t val = (regs.get(rs1) + imm_i) & 0xfffffffe;
	
	if (pos)
	{
		std::string s = render_jalr(insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << rv32i_decode::render_reg(rd) << " = " << hex::to_hex0x32(nextPC) << ", "
			<< "pc = (" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i)
			<< ") & 0xfffffffe";
	}
	
	pc += val;
}

void rv32i_hart::exec_beq(uint32_t insn, std::ostream *pos) // btype
{
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);
	int32_t pcrel_13 = get_imm_b(insn);
	
	int32_t val = (regs.get(rs1) == regs.get(rs2) ? pcrel_13 : 4);
	
	if (pos)
	{
		std::string s = render_btype(pc, insn, "beq");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << "pc += (" << hex::to_hex0x32(regs.get(rs1)) << " == "
			<< hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(pcrel_13) 
			<< " : 4) = " << hex::to_hex0x32(val);
	}
	
	pc += val;
}

void rv32i_hart::exec_bne(uint32_t insn, std::ostream *pos) // btype
{
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);
	int32_t pcrel_13 = get_imm_b(insn);
	
	int32_t val = (regs.get(rs1) != regs.get(rs2) ? pcrel_13 : 4);
	
	if (pos)
	{
		std::string s = render_btype(pc, insn, "bne");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << "pc += (" << hex::to_hex0x32(regs.get(rs1)) << " != "
			<< hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(pcrel_13) 
			<< " : 4) = " << hex::to_hex0x32(val);
	}
	
	pc += val;
}

void rv32i_hart::exec_blt(uint32_t insn, std::ostream *pos) // btype
{
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);
	int32_t pcrel_13 = get_imm_b(insn);
	
	int32_t val = (regs.get(rs1) < regs.get(rs2) ? pcrel_13 : 4);
	
	if (pos)
	{
		std::string s = render_btype(pc, insn, "blt");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << "pc += (" << hex::to_hex0x32(regs.get(rs1)) << " < "
			<< hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(pcrel_13) 
			<< " : 4) = " << hex::to_hex0x32(val);
	}
	
	pc += val;
}

void rv32i_hart::exec_bge(uint32_t insn, std::ostream *pos) // btype
{
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);
	int32_t pcrel_13 = get_imm_b(insn);
	
	int32_t val = (regs.get(rs1) >= regs.get(rs2) ? pcrel_13 : 4);
	
	if (pos)
	{
		std::string s = render_btype(pc, insn, "bge");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << "pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >= "
			<< hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(pcrel_13 - pc) 
			<< " : 4) = " << hex::to_hex0x32(val);
	}
	
	pc += val;
}

void rv32i_hart::exec_bltu(uint32_t insn, std::ostream *pos) // btype
{
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	int32_t pcrel_13 = get_imm_b(insn);
	
	int32_t val = (regs.get(rs1) < regs.get(rs2) ? pcrel_13 : 4);
	
	if (pos)
	{
		std::string s = render_btype(pc, insn, "bltu");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << "pc += (" << hex::to_hex0x32(regs.get(rs1)) << " <U "
			<< hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(pcrel_13) 
			<< " : 4) = " << hex::to_hex0x32(val);
	}
	
	pc += val;
}

void rv32i_hart::exec_bgeu(uint32_t insn, std::ostream *pos) // btype
{
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	int32_t pcrel_13 = get_imm_b(insn);
	
	int32_t val = (regs.get(rs1) >= regs.get(rs2) ? pcrel_13 : 4);
	
	if (pos)
	{
		std::string s = render_btype(pc, insn, "bge");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << "pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >=U "
			<< hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(pcrel_13 - pc) 
			<< " : 4) = " << hex::to_hex0x32(val);
	}
	
	pc += val;
}

void rv32i_hart::exec_lb(uint32_t insn, std::ostream *pos) // itype load
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);
	
	int32_t val = regs.get(rs1) + regs.get(imm_i);
	
	int32_t extVal = mem.get8(val);
	
	if (0x00000080 & extVal)
	{
		extVal |= 0xffffff00;
	}
	
	regs.set(rd, extVal);
	
 	if (pos)
	{
		std::string s = render_itype_load(insn, "lb");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = sx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " 
			<< hex::to_hex0x32(regs.get(imm_i)) << ")) = " << hex::to_hex0x32(extVal);
	}
	
	pc += 4;
}

void rv32i_hart::exec_lh(uint32_t insn, std::ostream *pos) // itype load
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);
	
	int32_t val = regs.get(rs1) + regs.get(imm_i);
	
	int32_t extVal = mem.get16(val);
	
	if (0x00008000 & extVal)
	{
		extVal |= 0xffff0000;
	}
	
	regs.set(rd, extVal);
	
	
 	if (pos)
	{
		std::string s = render_itype_load(insn, "lh");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = sx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " 
			<< hex::to_hex0x32(regs.get(imm_i)) << ")) = " << hex::to_hex0x32(extVal);
	}
	
	pc += 4;
}

void rv32i_hart::exec_lw(uint32_t insn, std::ostream *pos) // itype load
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);
	
	int32_t val = regs.get(rs1) + regs.get(imm_i);
	
	int32_t extVal = mem.get32(val);
	
	regs.set(rd, extVal);
	
	
 	if (pos)
	{
		std::string s = render_itype_load(insn, "lw");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = sx(m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " 
			<< hex::to_hex0x32(regs.get(imm_i)) << ")) = " << hex::to_hex0x32(extVal);
	}
	
	pc += 4;
}

void rv32i_hart::exec_lbu(uint32_t insn, std::ostream *pos) // itype load
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);
	
	int32_t val = regs.get(rs1) + regs.get(imm_i);
	
	int32_t extVal = mem.get8(val);

	regs.set(rd, extVal);
	
 	if (pos)
	{
		std::string s = render_itype_load(insn, "lbu");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = zx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " 
			<< hex::to_hex0x32(regs.get(imm_i)) << ")) = " << hex::to_hex0x32(extVal);
	}
	
	pc += 4;
}
void rv32i_hart::exec_lhu(uint32_t insn, std::ostream *pos) // itype load
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);
	
	int32_t val = regs.get(rs1) + regs.get(imm_i);
	
	int32_t extVal = mem.get16(val);

	regs.set(rd, extVal);
	
	
 	if (pos)
	{
		std::string s = render_itype_load(insn, "lhu");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = sx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " 
			<< hex::to_hex0x32(regs.get(imm_i)) << ")) = " << hex::to_hex0x32(extVal);
	}
	
	pc += 4;
}

void rv32i_hart::exec_sb(uint32_t insn, std::ostream *pos) // stype
{
	uint32_t rs1 = get_rs1(insn);
	int32_t imm_s = get_imm_s(insn);
	
	int32_t val = (regs.get(rs1) + regs.get(imm_s));
	int32_t extVal = mem.get8(val);
	
 	if (pos)
	{
		std::string s = render_stype(insn, "sb");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " 
			<< hex::to_hex0x32(regs.get(imm_s)) << ") = " << hex::to_hex0x32(extVal);
	}
	
	pc += 4;
}

void rv32i_hart::exec_sh(uint32_t insn, std::ostream *pos) // stype
{
	uint32_t rs1 = get_rs1(insn);
	int32_t imm_s = get_imm_s(insn);
	
	int32_t val = (regs.get(rs1) + regs.get(imm_s));
	int32_t extVal = mem.get16(val);
	
 	if (pos)
	{
		std::string s = render_stype(insn, "sh");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " 
			<< hex::to_hex0x32(regs.get(imm_s)) << ") = " << hex::to_hex0x32(extVal);
	}
	
	pc += 4;
}

void rv32i_hart::exec_sw(uint32_t insn, std::ostream *pos) // stype
{
	uint32_t rs1 = get_rs1(insn);
	int32_t imm_s = get_imm_s(insn);
	
	int32_t val = (regs.get(rs1) + regs.get(imm_s));
	int32_t extVal = mem.get32(val);
	
 	if (pos)
	{
		std::string s = render_stype(insn, "sw");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " 
			<< hex::to_hex0x32(regs.get(imm_s)) << ") = " << hex::to_hex0x32(extVal);
	}
	
	pc += 4;
}

void rv32i_hart::exec_addi(uint32_t insn, std::ostream *pos) // itype alu
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);
	
	int32_t val = regs.get(rs1) + regs.get(imm_i);
	
 	if (pos)
	{
		std::string s = render_itype_alu(insn, "addi", get_imm_i(insn));
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " + " 
			<< hex::to_hex0x32(regs.get(imm_i)) << " = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_slti(uint32_t insn, std::ostream *pos) // itype alu
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);
	
	int32_t val = (regs.get(rs1) < regs.get(imm_i)) ? 1 : 0;
	
 	if (pos)
	{
		std::string s = render_itype_alu(insn, "slti", get_imm_i(insn));
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " < " 
			<< hex::to_hex0x32(regs.get(imm_i)) << ") ? 1 : 0 = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_sltiu(uint32_t insn, std::ostream *pos) // itype alu
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);
	
	uint32_t val = (regs.get(rs1) < regs.get(imm_i)) ? 1 : 0;
	
 	if (pos)
	{
		std::string s = render_itype_alu(insn, "sltiu", get_imm_i(insn));
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " <U " 
			<< hex::to_hex0x32(regs.get(imm_i)) << ") ? 1 : 0 = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_xori(uint32_t insn, std::ostream *pos) // itype alu
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);
	
	int32_t val = regs.get(rs1) ^ regs.get(imm_i);
	
 	if (pos)
	{
		std::string s = render_itype_alu(insn, "xori", get_imm_i(insn));
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " ^ " 
			<< hex::to_hex0x32(regs.get(imm_i)) << " = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_ori(uint32_t insn, std::ostream *pos) // itype alu
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);
	
	int32_t val = regs.get(rs1) | regs.get(imm_i);
	
 	if (pos)
	{
		std::string s = render_itype_alu(insn, "ori", get_imm_i(insn));
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " | " 
			<< regs.get(imm_i) << " = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_andi(uint32_t insn, std::ostream *pos) // itype alu
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);
	
	int32_t val = regs.get(rs1) & regs.get(imm_i);
	
 	if (pos)
	{
		std::string s = render_itype_alu(insn, "andi", get_imm_i(insn));
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " & " 
			<< regs.get(imm_i) << " = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_slli(uint32_t insn, std::ostream *pos) // itype alu
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);
	uint32_t shamt = imm_i & 0x0000001f;
	
	int32_t val = regs.get(rs1) << shamt;
	
 	if (pos)
	{
		std::string s = render_itype_alu(insn, "slli", get_imm_i(insn)%XLEN);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " << " 
			<< shamt << " = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_srai(uint32_t insn, std::ostream *pos) // itype alu ******************************************************************
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);
	uint32_t shamt = imm_i & 0x0000001f;
	
	int32_t val = regs.get(rs1) >> shamt;
	
 	if (pos)
	{
		std::string s = render_itype_alu(insn, "srai", get_imm_i(insn)%XLEN);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " 
			<< shamt << " = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_srli(uint32_t insn, std::ostream *pos) // itype alu
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);
	uint32_t shamt = imm_i & 0x0000001f;
	
	int32_t val = regs.get(rs1) >> shamt;
	
 	if (pos)
	{
		std::string s = render_itype_alu(insn, "srli", get_imm_i(insn)%XLEN);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " 
			<< shamt << " = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_add(uint32_t insn, std::ostream *pos) // rtype
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	
	int32_t val = regs.get(rs1) + regs.get(rs2);
	
 	if (pos)
	{
		std::string s = render_rtype(insn, "add");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " + " 
			<< hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_sub(uint32_t insn, std::ostream *pos) // rtype
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	
	int32_t val = regs.get(rs1) - regs.get(rs2);
	
 	if (pos)
	{
		std::string s = render_rtype(insn, "sub");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " - " 
			<< hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_slt(uint32_t insn, std::ostream *pos) // rtype
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	
	int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;
	
 	if (pos)
	{
		std::string s = render_rtype(insn, "slt");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " < " 
			<< hex::to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_sltu(uint32_t insn, std::ostream *pos) // rtype
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	
	uint32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;
	
 	if (pos)
	{
		std::string s = render_rtype(insn, "sltu");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " <U " 
			<< hex::to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_xor(uint32_t insn, std::ostream *pos) // rtype
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	
	int32_t val = regs.get(rs1) ^ regs.get(rs2);
	
 	if (pos)
	{
		std::string s = render_rtype(insn, "xor");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " ^ " 
			<< hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_or(uint32_t insn, std::ostream *pos) // rtype
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	
	int32_t val = regs.get(rs1) | regs.get(rs2);
	
 	if (pos)
	{
		std::string s = render_rtype(insn, "or");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " | " 
			<< regs.get(rs2) << " = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_and(uint32_t insn, std::ostream *pos) // rtype
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	
	int32_t val = regs.get(rs1) & regs.get(rs2);
	
 	if (pos)
	{
		std::string s = render_rtype(insn, "and");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " & " 
			<< regs.get(rs2) << " = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_sll(uint32_t insn, std::ostream *pos) // rtype
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	
	int32_t val = regs.get(rs1) << regs.get(rs2);
	
 	if (pos)
	{
		std::string s = render_rtype(insn, "sll");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " << " 
			<< regs.get(rs2) << " = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_sra(uint32_t insn, std::ostream *pos) // rtype ******************************************************************************************
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	
	int32_t val = regs.get(rs1) >> regs.get(rs2);
	
 	if (pos)
	{
		std::string s = render_rtype(insn, "sra");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " 
			<< regs.get(rs2) << " = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_srl(uint32_t insn, std::ostream *pos) //rtype
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	
	int32_t val = regs.get(rs1) >> regs.get(rs2);
	
 	if (pos)
	{
		std::string s = render_rtype(insn, "srl");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " 
			<< regs.get(rs2) << " = " << hex::to_hex0x32(val);
	}
	
	regs.set(rd, val);
	pc += 4;
}

void rv32i_hart::exec_csrrw(uint32_t insn, std::ostream *pos) // system
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	
	if (pos)
	{
		std::string s = render_csrrx(insn, "csrrw");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1));
	}
	
	pc += 4;
}

void rv32i_hart::exec_csrrs(uint32_t insn, std::ostream *pos) // system
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	
	int32_t val = regs.get(rs1) & regs.get(rd);
	
	if (pos)
	{
		std::string s = render_csrrx(insn, "csrrs");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(val);
	}
	
	pc += 4;
}

void rv32i_hart::exec_csrrc(uint32_t insn, std::ostream *pos) // system
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	
	int32_t val = regs.get(rs1) & ~regs.get(rd);
	
	if (pos)
	{
		std::string s = render_csrrx(insn, "csrrc");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(val);
	}
	
	pc += 4;
}

void rv32i_hart::exec_csrrwi(uint32_t insn, std::ostream *pos) // system
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	
	if (pos)
	{
		std::string s = render_csrrx(insn, "csrrwi");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1));
	}
	
	pc += 4;
}

void rv32i_hart::exec_csrrsi(uint32_t insn, std::ostream *pos) // system
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	
	uint32_t val = regs.get(rs1) & regs.get(rd);
	
	if (pos)
	{
		std::string s = render_csrrx(insn, "csrrsi");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(val);
	}
	
	pc += 4;
}

void rv32i_hart::exec_csrrci(uint32_t insn, std::ostream *pos) // system
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	
	uint32_t val = regs.get(rs1) & ~regs.get(rd);
	
	if (pos)
	{
		std::string s = render_csrrx(insn, "csrrci");
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(val);
	}
	
	pc += 4;
}