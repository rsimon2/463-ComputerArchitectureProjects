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

#include "rv32i_decode.h"
#include "memory.h"
#include "hex.h"

/**
 * Decode instructions
 *
 * Prints instructions by reading the opcode, funct3, and funct 7
 *
 * @param addr The 32-bit address of the instruction being read in
 * @param insn The 32-bit instruction being read in
 ****************************************************************************/

std::string rv32i_decode::decode(uint32_t addr, uint32_t insn)
{
	uint32_t funct3 = get_funct3(insn);
	uint32_t funct7 = get_funct7(insn);
	
		
	if (insn == insn_ecall)
	{
		return render_ecall(insn);
	}
	else if (insn == insn_ebreak)
	{
		return render_ebreak(insn);
	}
	else
	{
		switch(get_opcode(insn))
		{
			default: 							return render_illegal_insn(insn);
			case opcode_lui: 					return render_lui(insn);
			case opcode_auipc:					return render_auipc(insn);
			case opcode_jal:					return render_jal(addr, insn);
			case opcode_jalr:					return render_jalr(insn);
			case opcode_btype:
				switch(funct3)
				{
					default:					return render_illegal_insn(insn);
					case funct3_beq:			return render_btype(addr, insn, "beq");
					case funct3_bne:			return render_btype(addr, insn, "bne");
					case funct3_blt:			return render_btype(addr, insn, "blt");
					case funct3_bge:			return render_btype(addr, insn, "bge");
					case funct3_bltu:			return render_btype(addr, insn, "bltu");
					case funct3_bgeu:			return render_btype(addr, insn, "bgeu");
				}
				assert(0 && "unrecognized funct3"); // impossible
			case opcode_load_imm:
				switch(funct3)
				{
					default:					return render_illegal_insn(insn);
					case funct3_lb:				return render_itype_load(insn, "lb");
					case funct3_lh:				return render_itype_load(insn, "lh");
					case funct3_lw:				return render_itype_load(insn, "lw");
					case funct3_lbu:			return render_itype_load(insn, "lbu");
					case funct3_lhu:			return render_itype_load(insn, "lhu");
				}
				assert(0 && "unrecognized funct3"); // impossible
			case opcode_stype:
				switch(funct3)
				{
					default:					return render_illegal_insn(insn);
					case funct3_sb:				return render_stype(insn, "sb");
					case funct3_sh:				return render_stype(insn, "sh");
					case funct3_sw:				return render_stype(insn, "sw");
				}
				assert(0 && "unrecognized funct3"); // impossible
			case opcode_alu_imm:
				switch(funct3)
				{
					default:					return render_illegal_insn(insn);
					case funct3_add:			return render_itype_alu(insn, "addi", get_imm_i(insn));
					case funct3_slt:			return render_itype_alu(insn, "slti", get_imm_i(insn));
					case funct3_sltu:			return render_itype_alu(insn, "sltiu", get_imm_i(insn));
					case funct3_xor:			return render_itype_alu(insn, "xori", get_imm_i(insn));
					case funct3_or:				return render_itype_alu(insn, "ori", get_imm_i(insn));
					case funct3_and:			return render_itype_alu(insn, "andi", get_imm_i(insn));
					case funct3_sll:			return render_itype_alu(insn, "slli", get_imm_i(insn)%XLEN);
					case funct3_srx:
						switch(funct7)
						{
							default:			return render_illegal_insn(insn);
							case funct7_sra:	return render_itype_alu(insn, "srai", get_imm_i(insn)%XLEN);
							case funct7_srl:	return render_itype_alu(insn, "srli", get_imm_i(insn)%XLEN);
						}
						assert(0 && "unrecognized funct7"); // impossible
				}
				assert(0 && "unrecognized funct3"); // impossible
			case opcode_rtype:
				switch(funct3)
				{
					default:					return render_illegal_insn(insn);
					case funct3_add:
						switch(funct7)
						{
							default:			return render_illegal_insn(insn);
							case funct7_add:	return render_rtype(insn, "add");
							case funct7_sub:	return render_rtype(insn, "sub");
						}
						assert(0 && "unrecognized funct7"); // impossible
					case funct3_slt:			return render_rtype(insn, "slt");
					case funct3_sltu:			return render_rtype(insn, "sltu");
					case funct3_xor:			return render_rtype(insn, "xor");
					case funct3_or:				return render_rtype(insn, "or");
					case funct3_and:			return render_rtype(insn, "and");
					case funct3_sll:			return render_rtype(insn, "sll");
					case funct3_srx:
						switch(funct7)
						{
							default:			return render_illegal_insn(insn);
							case funct7_sra:	return render_rtype(insn, "sra");
							case funct7_srl:	return render_rtype(insn, "srl");
						}
						assert(0 && "unrecognized funct7"); // impossible
					assert(0 && "unrecognized funct3"); // impossible
				}
			case opcode_system:
				switch(funct3)
				{
					default:					return render_illegal_insn(insn);
					case funct3_csrrw:			return render_csrrx(insn, "csrrw");
					case funct3_csrrs:			return render_csrrx(insn, "csrrs");
					case funct3_csrrc:			return render_csrrx(insn, "csrrc");
					case funct3_csrrwi:			return render_csrrxi(insn, "csrrwi");
					case funct3_csrrsi:			return render_csrrxi(insn, "csrrsi");
					case funct3_csrrci:			return render_csrrxi(insn, "csrrci");
				}
				assert(0 && "unrecognized funct3"); // impossible
		}
	}
	assert(0 && "unrecognized opcode"); // It should be impossible to ever get here!
	
	/*switch(insn)
	{
		default:					return render_illegal_insn(insn);
		case insn_ecall:			return render_ecall(insn);
		case insn_ebreak:			return render_ebreak(insn);
	}*/
}

/**
 * Invalid instruction message
 *
 * Prints an error if an instruction is not recognized
 *
 * @param insn The 32-bit instruction being read in
 ****************************************************************************/
 
std::string rv32i_decode::render_illegal_insn(uint32_t insn)
{
	return "ERROR: UNIMPLEMENTED INSTRUCTION";
}

/**
 * lui
 *
 * Prints the mnemonic and parameters of the lui instruction
 *
 * @param insn The 32-bit instruction being read in
 ****************************************************************************/

std::string rv32i_decode::rv32i_decode::render_lui(uint32_t insn)
{
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);
	
	std::ostringstream os;
	os << render_mnemonic("lui") << render_reg(rd) << ","
		<< to_hex0x20((imm_u >> 12) & 0x0fffff);
	
	return os.str();
}

/**
 * auipc
 *
 * Prints the mnemonic and parameters of the auipc instruction
 *
 * @param insn The 32-bit instruction being read in
 ****************************************************************************/
 
std::string rv32i_decode::rv32i_decode::render_auipc(uint32_t insn)
{
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);
	
	std::ostringstream os;
	os << render_mnemonic("auipc") << render_reg(rd) << ","
		<< to_hex0x20((imm_u >> 12) & 0x0fffff);
	
	return os.str();
}

/**
 * jal
 *
 * Prints the mnemonic and parameters of the jal instruction
 *
 * @param addr The 32-bit address of the instruction being read in
 * @param insn The 32-bit instruction being read in
 ****************************************************************************/

std::string rv32i_decode::rv32i_decode::render_jal(uint32_t addr, uint32_t insn)
{
	uint32_t rd = get_rd(insn);
	uint32_t pcrel_21 = get_imm_j(insn) + addr;
	
	std::ostringstream os;
	os << render_mnemonic("jal") << render_reg(rd) << ","
		<< to_hex0x32(pcrel_21);
	
	return os.str();
}

/**
 * jalr
 *
 * Prints the mnemonic and parameters of the jalr instruction
 *
 * @param insn The 32-bit instruction being read in
 ****************************************************************************/

std::string rv32i_decode::rv32i_decode::render_jalr(uint32_t insn)
{
	uint32_t rd = get_rd(insn);
	int32_t imm_i = get_imm_i(insn);
	uint32_t rs1 = get_rs1(insn);
	
	std::ostringstream os;
	os << render_mnemonic("jalr") << render_reg(rd) << ","
		<< imm_i << "(" << render_reg(rs1) << ")";
	
	return os.str();
}

/**
 * B-type
 *
 * Prints the mnemonic and parameters of a B-type instruction
 *
 * @param addr The 32-bit address of the instruction being read in
 * @param insn The 32-bit instruction being read in
 * @param mnemonic The mnemonic as determined by the decode() function
 ****************************************************************************/

std::string rv32i_decode::rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic)
{
	uint32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);
	uint32_t pcrel_13 = get_imm_b(insn) + addr;
	
	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rs1) << ","
		<< render_reg(rs2) << "," << to_hex0x32(pcrel_13);
	
	return os.str();
}

/**
 * I-type Load
 *
 * Prints the mnemonic and parameters of a Load I-type instruction
 *
 * @param insn The 32-bit instruction being read in
 * @param mnemonic The mnemonic as determined by the decode() function
 ****************************************************************************/

std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic)
{
	uint32_t rd = get_rd(insn);
	int32_t imm_i = get_imm_i(insn);
	uint32_t rs1 = get_rs1(insn);
	
	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << ","
		<< imm_i << "(" << render_reg(rs1) << ")";
	
	return os.str();
}

/**
 * S-type
 *
 * Prints the mnemonic and parameters of a B-type instruction
 *
 * @param insn The 32-bit instruction being read in
 * @param mnemonic The mnemonic as determined by the decode() function
 ****************************************************************************/

std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic)
{
	int32_t imm_s = get_imm_s(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	
	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rs2) << ","
		<< imm_s << "(" << render_reg(rs1) << ")";
	
	return os.str();
}

/**
 * I-type ALU
 *
 * Prints the mnemonic and parameters of an ALU I-type instruction
 *
 * @param insn The 32-bit instruction being read in
 * @param mnemonic The mnemonic as determined by the decode() function
 * @param imm_i The encoded imm_i of the given instruction. This is encoded 
 *              by the get_imm_i() function
 ****************************************************************************/

std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i)
{
	imm_i = get_imm_i(insn);
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	
	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << ","
		<< render_reg(rs1) << "," << imm_i;
	
	return os.str();
}

/**
 * R-type
 *
 * Prints the mnemonic and parameters of a R-type instruction
 *
 * @param insn The 32-bit instruction being read in
 * @param mnemonic The mnemonic as determined by the decode() function
 ****************************************************************************/

std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic)
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	
	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << ","
		<< render_reg(rs1) << "," << render_reg(rs2);
		
	return os.str();
}

/**
 * ecall
 *
 * Prints the mnemonic of the ecall instruction
 *
 * @param insn The 32-bit instruction being read in
 ****************************************************************************/

std::string rv32i_decode::render_ecall(uint32_t insn)
{
	return "ecall";
}

/**
 * ebreak
 *
 * Prints the mnemonic of the ebreak instruction
 *
 * @param insn The 32-bit instruction being read in
 ****************************************************************************/
 
std::string rv32i_decode::render_ebreak(uint32_t insn)
{
	return "ebreak";
}

/**
 * System-type csrrx
 *
 * Prints the mnemonic and parameters of a csrrx system-type instruction
 *
 * @param insn The 32-bit instruction being read in
 * @param mnemonic The mnemonic as determined by the decode() function
 ****************************************************************************/

std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic)
{
	int32_t imm_i = get_imm_i(insn);
	imm_i = imm_i & 0x00000fff;
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	
	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << ","
		<< hex::to_hex0x12(imm_i) << "," << render_reg(rs1);
	
	return os.str();
}

/**
 * System-type csrrxi
 *
 * Prints the mnemonic and parameters of a csrrxi system-type instruction
 *
 * @param insn The 32-bit instruction being read in
 * @param mnemonic The mnemonic as determined by the decode() function
 ****************************************************************************/

std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic)
{
	int32_t imm_i = get_imm_i(insn);
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	
	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << ","
		<< hex::to_hex0x12(imm_i) << "," << rs1;
	
	return os.str();
}

/**
 * Register
 *
 * Prints the register parameters for the given instruction as "x#"
 *
 * @param r The register number given for the parameters of the instruction
 ****************************************************************************/

std::string rv32i_decode::render_reg(int r)
{	
	std::ostringstream os;
	os << "x" << r;
	
	return os.str();
}

/**
 * Mnemonic
 *
 * Prints the mnemonic of the given instruction
 *
 * @param m The mnemonic of the given instruction
 ****************************************************************************/

std::string rv32i_decode::render_mnemonic(const std::string &m)
{
	std::ostringstream os;
	os << std::left << std::setw(mnemonic_width) << m;
	
	return os.str();
}

/**
 * Opcode
 *
 * Extracts bits 0-6 (opcode) of the given instruction
 *
 * @param insn The 32-bit instruction being read in
 ****************************************************************************/

uint32_t rv32i_decode::get_opcode(uint32_t insn)
{
	return (insn & 0x0000007f);
}

/**
 * Destination Register
 *
 * Extracts bits 7-11 (rd) of the given instruction
 *
 * @param insn The 32-bit instruction being read in
 ****************************************************************************/

uint32_t rv32i_decode::get_rd(uint32_t insn)
{
	return ((insn & 0x00000f80) >> (7-0));
}

/**
 * Register 1
 *
 * Extracts bits 15-19 (rs1) of the given instruction
 *
 * @param insn The 32-bit instruction being read in
 ****************************************************************************/

uint32_t rv32i_decode::get_rs1(uint32_t insn)
{
	return ((insn & 0x000f8000) >> (15-0));
}

/**
 * Register 2
 *
 * Extracts bits 20-24 (rs2) of the given instruction
 *
 * @param insn The 32-bit instruction being read in
 ****************************************************************************/

uint32_t rv32i_decode::get_rs2(uint32_t insn)
{
	return ((insn & 0x01f00000) >> (20-0));
}

/**
 * Funct3
 *
 * Extracts bits 12-14 (funct3) of the given instruction
 *
 * @param insn The 32-bit instruction being read in
 ****************************************************************************/

uint32_t rv32i_decode::get_funct3(uint32_t insn)
{
	return ((insn & 0x00007000) >> (12-0));
}

/**
 * Funct7
 *
 * Extracts bits 25-31 (funct7) of the given instruction
 *
 * @param insn The 32-bit instruction being read in
 ****************************************************************************/

uint32_t rv32i_decode::get_funct7(uint32_t insn)
{
	return ((insn & 0xfe000000) >> (25-0));
}

/**
 * Imm_I
 *
 * Encodes given instruction into imm_i format
 *
 * @param insn The 32-bit instruction being read in
 *
 * @note Extract and return the imm_i field from the given instruction as a 
 *       32-bit signed integer as shown in RVALP.
 ****************************************************************************/

int32_t rv32i_decode::get_imm_i(uint32_t insn)
{
	int32_t imm_i = (insn & 0xfff00000) >> (20-0);
	
	if (insn & 0x80000000)
	{
		imm_i |= 0xfffff000; // sign-extend the left
	}
	
	return imm_i;
}

/**
 * Imm_U
 *
 * Encodes given instruction into imm_u format
 *
 * @param insn The 32-bit instruction being read in
 *
 * @note Extract and return the imm_u field from the given instruction as a 
 *       32-bit signed integer as shown in RVALP.
 ****************************************************************************/

int32_t rv32i_decode::get_imm_u(uint32_t insn)
{	
	int32_t imm_u = (insn & 0xfffff000);
	
	return imm_u;
}

/**
 * Imm_B
 *
 * Encodes given instruction into imm_b format
 *
 * @param insn The 32-bit instruction being read in
 *
 * @note Extract and return the imm_b field from the given instruction as a 
 *       32-bit signed integer as shown in RVALP.
 ****************************************************************************/

int32_t rv32i_decode::get_imm_b(uint32_t insn)
{
	// Extract and return the imm_b field from the given instruction as a 32-bit signed integer as shown in RVALP
	
	int32_t imm_b = (insn & 0x80000000) >> (31-12);
	imm_b |= (insn & 0x00000080) << (11-7);
	imm_b |= (insn & 0x7e000000) >> (25-5);
	imm_b |= (insn & 0x00000f00) >> (8-1);
	imm_b |= (insn & 0xfffffffe);
	
	if (insn & 0x80000000)
	{
		imm_b |= 0xffffe000; // sign-extend the left
	}
	
	return imm_b;
}

/**
 * Imm_S
 *
 * Encodes given instruction into imm_s format
 *
 * @param insn The 32-bit instruction being read in
 *
 * @note Extract and return the imm_s field from the given instruction as a 
 *       32-bit signed integer as shown in RVALP.
 ****************************************************************************/

int32_t rv32i_decode::get_imm_s(uint32_t insn)
{
	// Extract and return the imm_s field from the given instruction. A suitable implementation is:
	
	int32_t imm_s = (insn & 0xfe000000) >> (25-5);
	imm_s |= (insn & 0x00000f80) >> (7-0);
	
	if (insn & 0x80000000)
	{
		imm_s |= 0xfffff000; // sign-extend the left
	}

	return imm_s;
}

/**
 * Imm_J
 *
 * Encodes given instruction into imm_s format
 *
 * @param insn The 32-bit instruction being read in
 *
 * @note Extract and return the imm_j field from the given instruction as a 
 *       32-bit signed integer as shown in RVALP.
 ****************************************************************************/

int32_t rv32i_decode::get_imm_j(uint32_t insn)
{	
	int32_t imm_j = (insn & 0x80000000) >> (31-20);
	imm_j |= (insn & 0x000ff000);
	imm_j |= (insn & 0x00100000) >> (20-11);
	imm_j |= (insn & 0x7fe00000) >> (21-1);
	imm_j |= (insn & 0xfffffffe);
	
	if (insn & 0x80000000)
	{
		imm_j |= 0xffe00000;
	}
	
	return imm_j;
}