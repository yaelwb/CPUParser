#include "stdafx.h"
#include "Compiler.h"

Compiler::Compiler()
{
	m_opcodeMap["nop"] =     "000000";
	m_opcodeMap["xor"] =     "000001";
	m_opcodeMap["and"] =     "000010";
	m_opcodeMap["or"] =      "000011";
	m_opcodeMap["nxor"] =    "000100";
	m_opcodeMap["nor"] =     "000101";
	m_opcodeMap["nand"] =    "000110";
	m_opcodeMap["add"] =     "000111";
	m_opcodeMap["sub"] =     "001000";
	m_opcodeMap["mul"] =     "001001";
	m_opcodeMap["div"] =     "001010";
	m_opcodeMap["addfp"] =   "001011";
	m_opcodeMap["subfp"] =   "001100"; 
	m_opcodeMap["mulfp"] =   "001101";
	m_opcodeMap["divfp"] =   "001110";
	m_opcodeMap["cmpreg"] =  "001111";
	m_opcodeMap["not"] =     "010000";
	m_opcodeMap["abs"] =     "010001";
	m_opcodeMap["absfp"] =   "010010";
	m_opcodeMap["sllr"] =    "010011";
	m_opcodeMap["slar"] =    "010100";
	m_opcodeMap["srlr"] =    "010101";
	m_opcodeMap["srar"] =    "010110";
	m_opcodeMap["rotlr"] =   "010111";
	m_opcodeMap["rotrr"] =   "011000";
	m_opcodeMap["ldfm"] =    "011001";
	m_opcodeMap["stfm"] =    "011010";
	m_opcodeMap["xori"] =    "011011";
	m_opcodeMap["andi"] =    "011100";
	m_opcodeMap["ori"] =     "011101";
	m_opcodeMap["nxori"] =   "011110";
	m_opcodeMap["nori"] =    "011111";
	m_opcodeMap["nandi"] =   "100000";
	m_opcodeMap["addi"] =    "100001";
	m_opcodeMap["subi"] =    "100010";
	m_opcodeMap["muli"] =    "100011";
	m_opcodeMap["divi"] =    "100100";
	m_opcodeMap["addfpi"] =  "100101";
	m_opcodeMap["subfpi"] =  "100110";
	m_opcodeMap["mulfpi"] =  "100111";
	m_opcodeMap["divfpi"] =  "101000";
	m_opcodeMap["cmpregi"] = "101001";
	m_opcodeMap["jmp"] =     "101010";
	m_opcodeMap["bre"] =     "101011";
	m_opcodeMap["brue"] =    "101100";
	m_opcodeMap["brg"] =     "101101";
	m_opcodeMap["bls"] =     "101110";
	m_opcodeMap["btr"] =     "101111";
	m_opcodeMap["bfs"] =     "110000";
	m_opcodeMap["jmpr"] =    "110001";
	m_opcodeMap["slli"] =    "110010";
	m_opcodeMap["slai"] =    "110011";
	m_opcodeMap["srli"] =    "110100";
	m_opcodeMap["srai"] =    "110101";
	m_opcodeMap["rotli"] =   "110110";
	m_opcodeMap["rotri"] =   "110111";
	m_opcodeMap["ldfh"] =    "111000";
	m_opcodeMap["ldfl"] =    "111001";
	m_opcodeMap["ldf"] =     "111010";
	m_opcodeMap["goto"] =    "111111";
	m_opcodeMap["mov"] =     "111100";
	m_opcodeMap["clr"] =     "111101";
	
	m_regMap[0] =  "00000";
	m_regMap[1] =  "00001";
	m_regMap[2] =  "00010";
	m_regMap[3] =  "00011";
	m_regMap[4] =  "00100";
	m_regMap[5] =  "00101";
	m_regMap[6] =  "00110";
	m_regMap[7] =  "00111";
	m_regMap[8] =  "01000";
	m_regMap[9] =  "01001";
	m_regMap[10] = "01010";
	m_regMap[11] = "01011";
	m_regMap[12] = "01100";
	m_regMap[13] = "01101";
	m_regMap[14] = "01110";
	m_regMap[15] = "01111";
	m_regMap[16] = "10000";
	m_regMap[17] = "10001";
	m_regMap[18] = "10010";
	m_regMap[19] = "10011";
	m_regMap[20] = "10100";
	m_regMap[21] = "10101";
	m_regMap[22] = "10110";
	m_regMap[23] = "10111";
	m_regMap[24] = "11000";
	m_regMap[25] = "11001";
	m_regMap[26] = "11010";
	m_regMap[27] = "11011";
	m_regMap[28] = "11100";
	m_regMap[29] = "11101";
	m_regMap[30] = "11110";
	m_regMap[31] = "11111";

}


int Compiler::ProcessCmd(CmdInfo &cmdInfo)
{
	int ret;
    
	switch (cmdInfo.GetCommandType())
	{
	case CMD_3_REGS:
	case CMD_3_REGS_FP:
		ret = compile3Regs(cmdInfo);
		break;
	case CMD_2_REGS:
	case CMD_2_REGS_FP:
		ret = compile2Regs(cmdInfo);
		break;
	case CMD_2_REGS_IMM:
	case CMD_2_REGS_IMM_FP:
		ret = compile2RegsIMM(cmdInfo);
		break;
	case CMD_REG_IMM:
		ret = compileRegsIMM(cmdInfo);
		break;
	case CMD_SINGLE:
		ret = compileSINGLE(cmdInfo);
		break;
	case CMD_JUMP:
		ret = compileJump(cmdInfo);
		break;
	case CMD_GOTO:
		ret = compileGOTO(cmdInfo);
		break;
	case CMD_LDFM:
		ret = compileLDFM(cmdInfo);
		break;
	case CMD_STFM:
		ret = compileSTFM(cmdInfo);
		break;
	case CMD_LDFH:
		ret = compileLDFH(cmdInfo);
		break;
	case CMD_LDFL:
		ret = compileLDFL(cmdInfo);
		break;
	default:
		ret = UNKNOWN_CMD;
	}

	return NO_ERROR_FOUND;
}

Compiler::~Compiler()
{
}

int Compiler::compile3Regs(CmdInfo &cmdInfo)
{
	string binary_str;
	binary_str.clear();
	binary_str.append(m_opcodeMap[cmdInfo.GetCommand()]);
	binary_str.append(m_regMap[cmdInfo.m_r1]);
	binary_str.append(m_regMap[cmdInfo.m_r2]);
	binary_str.append(m_regMap[cmdInfo.m_r3]);
	appendTail(binary_str,11);
	cmdInfo.SetBinaryStr(binary_str);
	return NO_ERROR_FOUND;
}

int Compiler::compile2Regs(CmdInfo &cmdInfo)
{
	string binary_str;
	binary_str.clear();
	binary_str.append(m_opcodeMap[cmdInfo.GetCommand()]);
	binary_str.append(m_regMap[cmdInfo.m_r1]);
	binary_str.append(m_regMap[cmdInfo.m_r2]);
	appendTail(binary_str,16);
	cmdInfo.SetBinaryStr(binary_str);
	return NO_ERROR_FOUND;
}

int Compiler::compile2RegsIMM(CmdInfo &cmdInfo)
{
	string binary_str;
	string imm_str;
	size_t size_of_gap = 16, length = 0;
	binary_str.clear();
	binary_str.append(m_opcodeMap[cmdInfo.GetCommand()]);
	binary_str.append(m_regMap[cmdInfo.m_r1]);
	binary_str.append(m_regMap[cmdInfo.m_r2]);
	compileIMMintoStr(cmdInfo.m_imm, imm_str);
	length = imm_str.size();
	if(length <size_of_gap)
	{
      size_of_gap -= length;
	  appendTail(binary_str,size_of_gap);
	}
	binary_str.append(imm_str);
	cmdInfo.SetBinaryStr(binary_str);
	return NO_ERROR_FOUND;
}

int Compiler::compileIMM(CmdInfo &cmdInfo)
{
	int n = cmdInfo.m_imm;
	string str;
	size_t size_of_gap = 21, length = 0;
	string binary_str;
	compileIMMintoStr(cmdInfo.m_imm, str);
	length = str.size();
	if(length <size_of_gap)
	{
      size_of_gap -= length;
	  appendTail(binary_str,size_of_gap);
	}
	binary_str.append(str);
	cmdInfo.SetBinaryStr(binary_str);
	return NO_ERROR_FOUND;
}
int Compiler::compileIMMintoStr(int n, string& str)
{
	if (n >= 0)
	{
		char cstr[17];
		_itoa_s(n,cstr,17,2);
		str.clear();
		str.append(cstr);
		return NO_ERROR_FOUND;
	}
	char tmp[33];
	int i = 0;
	_itoa_s(n,tmp,33,2);

	str.clear();
	str.append(&tmp[16]);
	return NO_ERROR_FOUND;
}

int Compiler::compileRegsIMM(CmdInfo &cmdInfo)
{
	string binary_str;
	string imm_str;
	size_t size_of_gap = 21, length = 0;
	binary_str.clear();
	binary_str.append(m_opcodeMap[cmdInfo.GetCommand()]);
	binary_str.append(m_regMap[cmdInfo.m_r1]);
	compileIMMintoStr(cmdInfo.m_imm, imm_str);
	length = imm_str.size();
	if(length <size_of_gap)
	{
      size_of_gap -= length;
	  appendTail(binary_str,size_of_gap);
	}
	binary_str.append(imm_str);
	cmdInfo.SetBinaryStr(binary_str);
	return NO_ERROR_FOUND;
}

int Compiler::compileSINGLE(CmdInfo &cmdInfo)
{
	string binary_str;
	binary_str.clear();
	binary_str.append(m_opcodeMap[cmdInfo.GetCommand()]);
	appendTail(binary_str,26);
	cmdInfo.SetBinaryStr(binary_str);
	return NO_ERROR_FOUND;
}

int Compiler::compileGOTO(CmdInfo &cmdInfo)
{
	string binary_str;
	string imm_str;
	size_t size_of_gap = 26, length = 0;
	binary_str.clear();
	binary_str.append(m_opcodeMap[cmdInfo.GetCommand()]);
	compileIMMintoStr(cmdInfo.m_imm, imm_str);
	length = imm_str.size();
	if(length <size_of_gap)
	{
      size_of_gap -= length;
	  appendTail(binary_str,size_of_gap);
	}
	binary_str.append(imm_str);
	cmdInfo.SetBinaryStr(binary_str);
	return NO_ERROR_FOUND;
}

int Compiler::compileJump(CmdInfo &cmdInfo)
{
	string binary_str;
	string imm_str;
	size_t size_of_gap = 26, length = 0;
	binary_str.clear();
	binary_str.append(m_opcodeMap[cmdInfo.GetCommand()]);
	compileIMMintoStr(cmdInfo.m_imm, imm_str);
	length  = imm_str.size();
	if(length <size_of_gap)
	{
      size_of_gap -= length;
	  appendTail(binary_str,size_of_gap);
	}
	binary_str.append(imm_str);
	cmdInfo.SetBinaryStr(binary_str);
	return NO_ERROR_FOUND;
}

void Compiler::appendTail(string &str, size_t num)
{
	str.append(num,'0');
}

int Compiler::compileLDFM(CmdInfo &cmdInfo)
{
	return compileRegsIMM(cmdInfo);
}

int Compiler::compileSTFM(CmdInfo &cmdInfo)
{
	return compileRegsIMM(cmdInfo);
}

int Compiler::compileLDFH(CmdInfo &cmdInfo)
{
	string strHigh, strLow;
	string binary_str;
	processFP(cmdInfo.GetFloat(), strHigh, strLow);
	binary_str.clear();
	binary_str.append(m_opcodeMap[cmdInfo.GetCommand()]);
	binary_str.append(m_regMap[cmdInfo.m_r1]);
	appendTail(binary_str,5);
	binary_str.append(strHigh);
	cmdInfo.SetBinaryStr(binary_str);
	return NO_ERROR_FOUND;
}

int Compiler::compileLDFL(CmdInfo &cmdInfo)
{
	string strHigh, strLow;
	string binary_str;
	processFP(cmdInfo.GetFloat(), strHigh, strLow);
	binary_str.clear();
	binary_str.append(m_opcodeMap[cmdInfo.GetCommand()]);
	binary_str.append(m_regMap[cmdInfo.m_r1]);
	appendTail(binary_str,5);
	binary_str.append(strLow);
	cmdInfo.SetBinaryStr(binary_str);
	return NO_ERROR_FOUND;
}

void Compiler::printbin(unsigned long x, int n)
{
  if (--n) printbin(x>>1, n);
  putchar("01"[x&1]);
}

void Compiler::processFP(float input, string& strHigh, string& strLow)
{
	char str[33];
	long lu;
    size_t length = 0;
    lu = *(long*)&input;
    printbin(lu, 32);
	_itoa_s(lu,str,33,2);

	string result(str);
	length = result.size();
	strHigh.clear();
	strLow.clear();
	
	if(length <32)
	{
		string tmp_result(str);
		result.clear();
		result.append(32-length,'0');
		result.append(tmp_result);
	}
	
	strHigh.append(result.substr(0,16));
	strLow.append(result.substr(16,16));
}