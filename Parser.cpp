#include "stdafx.h"
#include "Parser.h"

Parser::Parser(): m_cmd_count(0)
{
	m_cmdMap["mov"] = CMD_2_REGS;
	m_cmdMap["xor"] = CMD_3_REGS;
	m_cmdMap["and"] = CMD_3_REGS;
	m_cmdMap["or"] = CMD_3_REGS; 
	m_cmdMap["nxor"] = CMD_3_REGS; 
	m_cmdMap["nor"] = CMD_3_REGS;  
	m_cmdMap["nand"] = CMD_3_REGS; 
	m_cmdMap["add"] = CMD_3_REGS; 
	m_cmdMap["sub"] = CMD_3_REGS;
	m_cmdMap["mul"] = CMD_3_REGS; 
	m_cmdMap["div"] = CMD_3_REGS; 
	m_cmdMap["addfp"] = CMD_3_REGS_FP; 
	m_cmdMap["subfp"] = CMD_3_REGS_FP; 
	m_cmdMap["mulfp"] = CMD_3_REGS_FP; 
	m_cmdMap["divfp"] = CMD_3_REGS_FP; 
	m_cmdMap["cmpreg"] = CMD_3_REGS;
	m_cmdMap["not"] = CMD_2_REGS;
	m_cmdMap["abs"] = CMD_2_REGS;
	m_cmdMap["absfp"] = CMD_2_REGS_FP;
	m_cmdMap["sllr"] = CMD_2_REGS;
	m_cmdMap["slar"] = CMD_2_REGS; 
	m_cmdMap["srlr"] = CMD_2_REGS;
	m_cmdMap["srar"] = CMD_2_REGS;
	m_cmdMap["rotlr"] = CMD_2_REGS;
	m_cmdMap["rotrr"] = CMD_2_REGS;
	m_cmdMap["ldfm"] = CMD_LDFM;
	m_cmdMap["stfm"] = CMD_STFM;
	m_cmdMap["xori"] = CMD_2_REGS_IMM;
	m_cmdMap["andi"] = CMD_2_REGS_IMM;
	m_cmdMap["ori"] = CMD_2_REGS_IMM;
	m_cmdMap["nxori"] = CMD_2_REGS_IMM;
	m_cmdMap["nori"] = CMD_2_REGS_IMM;
	m_cmdMap["nandi"] = CMD_2_REGS_IMM;
	m_cmdMap["addi"] = CMD_2_REGS_IMM;
	m_cmdMap["subi"] = CMD_2_REGS_IMM;
	m_cmdMap["muli"] = CMD_2_REGS_IMM;
	m_cmdMap["divi"] = CMD_2_REGS_IMM;
	m_cmdMap["addfpi"] = CMD_2_REGS_IMM_FP;
	m_cmdMap["subfpi"] = CMD_2_REGS_IMM_FP;
	m_cmdMap["mulfpi"] = CMD_2_REGS_IMM_FP;
	m_cmdMap["divfpi"] = CMD_2_REGS_IMM_FP;
	m_cmdMap["cmpregi"] = CMD_2_REGS_IMM;
	m_cmdMap["jmp"] = CMD_JUMP; 
	m_cmdMap["bre"] = CMD_JUMP;
	m_cmdMap["brue"] = CMD_JUMP;
	m_cmdMap["brg"] = CMD_JUMP;
	m_cmdMap["bls"] = CMD_JUMP;
	m_cmdMap["btr"] = CMD_JUMP;
	m_cmdMap["bfs"] = CMD_JUMP;
	m_cmdMap["jmpr"] = CMD_REG_IMM; 
	m_cmdMap["slli"] = CMD_REG_IMM;
	m_cmdMap["slai"] = CMD_REG_IMM;
	m_cmdMap["srli"] = CMD_REG_IMM;
	m_cmdMap["srai"] = CMD_REG_IMM;
	m_cmdMap["rotli"] = CMD_REG_IMM;
	m_cmdMap["rotri"] = CMD_REG_IMM;
	m_cmdMap["ldfh"] = CMD_LDFH;
	m_cmdMap["ldfl"] = CMD_LDFL;
	m_cmdMap["ldf"] = CMD_LDF;
	m_cmdMap["goto"] = CMD_GOTO;
	m_cmdMap["nop"] = CMD_SINGLE;
	m_cmdMap["clr"] = CMD_SINGLE;	
}

int Parser::PostProcessLabels()
{
	int cmd_idx = 0, imm_val = 0;
	map<int, string>::iterator it = m_unresolvedLabels.begin();
	for (; it != m_unresolvedLabels.end(); it++)
	{
		cmd_idx = (*it).first;
		string lbl((*it).second);
		CmdInfo &cmdInfo = m_commands[cmd_idx];
		map<string, int>::iterator it1 = m_labelsMap.find(lbl);
		if (it1 != m_labelsMap.end())
			cmdInfo.SetImm(it1->second);
	}
	return NO_ERROR_FOUND;
}

int Parser::ProcessLine(string assembly_cmd)
{
	string cmd, label;
	int ret = NO_ERROR_FOUND;
	CmdInfo cmdInfo;
	
	//eliminate whitespaces
	size_t start_pos = 0, end_pos = 0;
	string whitespaces (" \t\f\v\n\r");

	eliminatePreceedingWhitespaces(assembly_cmd);
	eliminateSucceedingWhitespaces(assembly_cmd);

	//parse labels
	while ((end_pos = assembly_cmd.find_first_of(":")) != string::npos)
	{
		label = assembly_cmd.substr(0,end_pos);
		assembly_cmd.erase(0,end_pos +1);

		eliminatePreceedingWhitespaces(label);
		eliminateSucceedingWhitespaces(label);

	    if(!label.empty() && label.find_first_not_of(whitespaces) != string::npos)
		  m_labelsMap[label] = m_cmd_count;
	}
	
    eliminatePreceedingWhitespaces(assembly_cmd);
	if(assembly_cmd.empty())
		return NO_ERROR_FOUND;

	//isolate command
	end_pos = assembly_cmd.find_first_of(whitespaces);
	cmd = assembly_cmd.substr(0,end_pos);	
    assembly_cmd.erase(0,end_pos);

	eliminatePreceedingWhitespaces(assembly_cmd);

	map<string,int>::iterator it = m_cmdMap.find(cmd);
	if (it == m_cmdMap.end())
		return UNKNOWN_CMD;
	int command_type = it->second;
	cmdInfo.SetAssemblyStr(assembly_cmd);
	cmdInfo.SetCommand(cmd);
	cmdInfo.SetCommandType(command_type);

	switch (command_type)
	{
	case CMD_3_REGS:
		ret = parse3Regs(cmdInfo);
		break;
	case CMD_2_REGS:
		ret = parse2Regs(cmdInfo);
		break;
	case CMD_2_REGS_IMM:
		ret = parse2RegsIMM(cmdInfo);
		break;
	case CMD_3_REGS_FP:
		ret = parse3Regs_FP(cmdInfo);
		break;
	case CMD_2_REGS_FP:
		ret = parse2Regs_FP(cmdInfo);
		break;
	case CMD_2_REGS_IMM_FP:
		ret = parse2RegsIMM_FP(cmdInfo);
		break;
	case CMD_IMM:
		ret = parseIMM(cmdInfo);
		break;
	case CMD_REG_IMM:
		ret = parseRegsIMM(cmdInfo);
		break;
	case CMD_SINGLE:
		break;
	case CMD_JUMP:
		ret = parseJump(cmdInfo);
		break;
	case CMD_GOTO:
		ret = parseGOTO(cmdInfo);
		break;
	case CMD_LDFM:
		ret = parseLDFM(cmdInfo);
		break;
	case CMD_STFM:
		ret = parseSTFM(cmdInfo);
		break;
	case CMD_LDF:
		ret = parseLDF(cmdInfo);
		cmdInfo.SetCommandType(CMD_LDFH);
		cmdInfo.SetCommand("ldfh");
		break;

	default:
		ret = UNKNOWN_CMD;
	}
	if (ret != NO_ERROR_FOUND)
		return ret;
	if (parseRemainingChars(cmdInfo.GetAssemblyStr()) != NO_ERROR_FOUND)
		return ILLEGAL_CMD_STX;
	m_commands.push_back(cmdInfo);
	m_cmd_count++;
	if(command_type == CMD_LDF)
	{
		CmdInfo cmdInfoLow;
		cmdInfoLow.SetCommandType(CMD_LDFL);
		cmdInfoLow.SetCommand("ldfl");
		cmdInfoLow.SetFloat(cmdInfo.GetFloat());
		cmdInfoLow.SetR1(cmdInfo.GetR1());
		m_commands.push_back(cmdInfoLow);
		m_cmd_count++;
	}
	return NO_ERROR_FOUND;
}

int Parser::parse3Regs(CmdInfo &cmdInfo)
{
	int err = NO_ERROR_FOUND;
	if ((err = parseReg(cmdInfo.m_assembly_str, cmdInfo.m_r1)) == NO_ERROR_FOUND &&
	    (err = parseReg(cmdInfo.m_assembly_str, cmdInfo.m_r2)) == NO_ERROR_FOUND &&
        (err = parseReg(cmdInfo.m_assembly_str, cmdInfo.m_r3)) == NO_ERROR_FOUND)
	  return NO_ERROR_FOUND;
    return err;
}

int Parser::parse2Regs(CmdInfo &cmdInfo)
{
	int err = NO_ERROR_FOUND;
	if ((err = parseReg(cmdInfo.m_assembly_str, cmdInfo.m_r1)) == NO_ERROR_FOUND &&
	    (err = parseReg(cmdInfo.m_assembly_str, cmdInfo.m_r2)) == NO_ERROR_FOUND)
	  return NO_ERROR_FOUND;
    return err;
}

int Parser::parse2RegsIMM(CmdInfo &cmdInfo)
{
    int err = NO_ERROR_FOUND;
	if ((err = parseReg(cmdInfo.m_assembly_str, cmdInfo.m_r1)) == NO_ERROR_FOUND &&
	    (err = parseReg(cmdInfo.m_assembly_str, cmdInfo.m_r2)) == NO_ERROR_FOUND &&
		(err = parseIMM(cmdInfo)) == NO_ERROR_FOUND)
	  return NO_ERROR_FOUND;
    return err;
}

int Parser::parse3Regs_FP(CmdInfo &cmdInfo)
{
	int err = NO_ERROR_FOUND;
	if ((err = parseFReg(cmdInfo.m_assembly_str, cmdInfo.m_r1)) == NO_ERROR_FOUND &&
	    (err = parseFReg(cmdInfo.m_assembly_str, cmdInfo.m_r2)) == NO_ERROR_FOUND &&
        (err = parseFReg(cmdInfo.m_assembly_str, cmdInfo.m_r3)) == NO_ERROR_FOUND)
	  return NO_ERROR_FOUND;
    return err;
}

int Parser::parse2Regs_FP(CmdInfo &cmdInfo)
{
	int err = NO_ERROR_FOUND;
	if ((err = parseFReg(cmdInfo.m_assembly_str, cmdInfo.m_r1)) == NO_ERROR_FOUND &&
	    (err = parseFReg(cmdInfo.m_assembly_str, cmdInfo.m_r2)) == NO_ERROR_FOUND)
	  return NO_ERROR_FOUND;
    return err;
}

int Parser::parse2RegsIMM_FP(CmdInfo &cmdInfo)
{
    int err = NO_ERROR_FOUND;
	if ((err = parseFReg(cmdInfo.m_assembly_str, cmdInfo.m_r1)) == NO_ERROR_FOUND &&
	    (err = parseFReg(cmdInfo.m_assembly_str, cmdInfo.m_r2)) == NO_ERROR_FOUND &&
		(err = parseIMM(cmdInfo)) == NO_ERROR_FOUND)
	  return NO_ERROR_FOUND;
    return err;
}

int Parser::parseIMM(CmdInfo &cmdInfo)
{
	int n = 0;
	eliminatePreceedingWhitespaces(cmdInfo.m_assembly_str);
	eliminateSucceedingWhitespaces(cmdInfo.m_assembly_str);
	n = atoi(cmdInfo.m_assembly_str.c_str());
	if(n > 32767 || n < -32767)
		return ILLEGAL_IMM_VAL;
	cmdInfo.SetImm(n);
	string numeric ("0123456789-");
	size_t end_pos = cmdInfo.m_assembly_str.find_first_not_of(numeric);
	cmdInfo.m_assembly_str.erase(0,end_pos);
	return NO_ERROR_FOUND;
}

int Parser::parseF(CmdInfo &cmdInfo)
{
	float f = 0;
	eliminatePreceedingWhitespaces(cmdInfo.m_assembly_str);
	eliminateSucceedingWhitespaces(cmdInfo.m_assembly_str);
	sscanf(cmdInfo.m_assembly_str.c_str(),"%f",&f);
	cmdInfo.SetFloat(f);
	string numeric ("0123456789.-");
	size_t end_pos = cmdInfo.m_assembly_str.find_first_not_of(numeric);
	cmdInfo.m_assembly_str.erase(0,end_pos);
	return NO_ERROR_FOUND;
}

int Parser::parseRegsIMM(CmdInfo &cmdInfo)
{
	int err = NO_ERROR_FOUND;
	if ((err = parseReg(cmdInfo.m_assembly_str, cmdInfo.m_r1)) == NO_ERROR_FOUND &&
	    (err = parseIMM(cmdInfo)) == NO_ERROR_FOUND)
	  return NO_ERROR_FOUND;
    return err;
}

int Parser::parseJump(CmdInfo &cmdInfo)
{
	eliminatePreceedingWhitespaces(cmdInfo.m_assembly_str);
	eliminateSucceedingWhitespaces(cmdInfo.m_assembly_str);
	cmdInfo.SetImm(atoi(cmdInfo.m_assembly_str.c_str()));
	string numeric ("0123456789");
	size_t end_pos = cmdInfo.m_assembly_str.find_first_not_of(numeric);
	cmdInfo.m_assembly_str.erase(0,end_pos);
	return NO_ERROR_FOUND;
}

int Parser::parseGOTO(CmdInfo &cmdInfo)
{
	eliminatePreceedingWhitespaces(cmdInfo.m_assembly_str);
	eliminateSucceedingWhitespaces(cmdInfo.m_assembly_str);
	map<string, int>::iterator it = m_labelsMap.find(cmdInfo.GetAssemblyStr());
	if (it != m_labelsMap.end())
	{
		cmdInfo.SetImm(it->second);
		cmdInfo.m_assembly_str.clear();
		return NO_ERROR_FOUND;
	}
		
	m_unresolvedLabels[m_cmd_count] = cmdInfo.GetAssemblyStr();
	cmdInfo.m_assembly_str.clear();
	return NO_ERROR_FOUND;
}

int Parser::parseReg(string &assembly_str, int &r)
{
	eliminatePreceedingWhitespaces(assembly_str);

	if(assembly_str[0] != 'R')
		return ILLEGAL_CMD_STX;
	assembly_str.erase(0,1);
	int n = ILLEGAL_VALUE;
	n = atoi(assembly_str.c_str());
	if(n > 31 || n < 0)
		return ILLEGAL_REG_NO;
	r = n;
	int pos = ((n>9)? 2:1);
	assembly_str.erase(0,pos);
	return NO_ERROR_FOUND;
}

int Parser::parseFReg(string &assembly_str, int &r)
{
	eliminatePreceedingWhitespaces(assembly_str);

	if(assembly_str[0] != 'F' || assembly_str[1] != 'R')
		return ILLEGAL_CMD_STX;
	assembly_str.erase(0,2);
	int n = ILLEGAL_VALUE;
	n = atoi(assembly_str.c_str());
	if(n > 7 || n < 0)
		return ILLEGAL_REG_NO;
	r = n;
	assembly_str.erase(0,1);
	return NO_ERROR_FOUND;
}

void Parser::eliminatePreceedingWhitespaces(string &assembly_str)
{
	size_t start_pos = 0;
	string whitespaces (" \t\f\v\n\r");
    start_pos = assembly_str.find_first_not_of(whitespaces);
	if (start_pos!=string::npos)
      assembly_str.erase(0,start_pos);
}

void Parser::eliminateSucceedingWhitespaces(string &assembly_str)
{
	string whitespaces (" \t\f\v\n\r");
	size_t end_pos = assembly_str.find_last_not_of(whitespaces);
    if (end_pos!=string::npos)
      assembly_str.erase(end_pos+1);
}

int Parser::parseRemainingChars(string &assembly_str)
{
	string whitespaces (" \t\f\v\n\r");
	size_t start_pos = assembly_str.find_first_not_of(whitespaces);
    if (start_pos!=string::npos)
      return ILLEGAL_CMD_STX;
	return NO_ERROR_FOUND;
}

int Parser::parseLDFM(CmdInfo &cmdInfo)
{
	int err = NO_ERROR_FOUND;
	if ((err = parseIMM(cmdInfo)) == NO_ERROR_FOUND &&
		(err = parseFReg(cmdInfo.m_assembly_str, cmdInfo.m_r1)) == NO_ERROR_FOUND)
	  return NO_ERROR_FOUND;
    return err;
}

int Parser::parseSTFM(CmdInfo &cmdInfo)
{
	int err = NO_ERROR_FOUND;
	if ((err = parseFReg(cmdInfo.m_assembly_str, cmdInfo.m_r1)) == NO_ERROR_FOUND &&
	    (err = parseIMM(cmdInfo)) == NO_ERROR_FOUND)
	  return NO_ERROR_FOUND;
    return err;
}

int Parser::parseLDF(CmdInfo &cmdInfo)
{
	int err = NO_ERROR_FOUND;
	if ((err = parseFReg(cmdInfo.m_assembly_str, cmdInfo.m_r1)) == NO_ERROR_FOUND &&
	    (err = parseF(cmdInfo)) == NO_ERROR_FOUND)
	  return NO_ERROR_FOUND;
    return err;
}

Parser::~Parser()
{
}