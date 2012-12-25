#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

const int ILLEGAL_VALUE = -1;

//error codes
const int NO_ERROR_FOUND = 0;
const int UNKNOWN_CMD = 1;
const int UNKNOWN_LABEL = 2;
const int ILLEGAL_REG_NO = 3;
const int ILLEGAL_IMM_VAL = 4;
const int ILLEGAL_CMD_STX = 5;

//command types
const int CMD_3_REGS = 1;
const int CMD_2_REGS = 2;
const int CMD_2_REGS_IMM = 3;
const int CMD_IMM = 4;
const int CMD_REG_IMM = 5;
const int CMD_SINGLE = 6;
const int CMD_JUMP = 7;
const int CMD_GOTO = 8;
const int CMD_LDFM = 9;
const int CMD_STFM = 10;
const int CMD_LDFH = 11;
const int CMD_LDFL = 12;
const int CMD_LDF = 13;
const int CMD_3_REGS_FP = 14;
const int CMD_2_REGS_FP = 15;
const int CMD_2_REGS_IMM_FP = 16;

class CmdInfo {
public:
	CmdInfo(): m_r1(0), m_r2(0), m_r3(0), m_imm(0){}
	void SetCommand(string cmd){m_cmd = cmd;}
	string GetCommand(){return m_cmd;}
	void SetAssemblyStr(string assembly_str){m_assembly_str = assembly_str;}
	string GetAssemblyStr(){return m_assembly_str;}
	void SetBinaryStr(string binary_str){m_binary_str = binary_str;}
	string GetBinaryStr(){return m_binary_str;}
	int GetCommandType() {return m_command_type;}
	void SetCommandType(int command_type) {m_command_type = command_type;}
	int GetImm() {return m_imm;}
	void SetImm(int imm) {m_imm = imm;}
	int GetR1() {return m_r1;}
	void SetR1(int r1) {m_r1 = r1;}
	float GetFloat() {return m_f;}
	void SetFloat(float f) {m_f = f;}
	friend ostream& operator << (ostream &out, CmdInfo& cmdInfo)
	{ out << cmdInfo.m_binary_str; return out;}
	~CmdInfo(){}

public:
	string m_assembly_str;
	string m_binary_str;
	int m_r1, m_r2, m_r3, m_imm;
	float m_f;
private:
	int m_command_type;
	string m_cmd;
};