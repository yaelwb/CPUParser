#pragma once
#include <map>
#include <vector>
#include "CmdInfo.h"
using namespace std;

class Compiler {
public:
	Compiler();
	int ProcessCmd(CmdInfo &cmdInfo);
	~Compiler();

protected:
	int compile3Regs(CmdInfo &cmdInfo);
	int compile2Regs(CmdInfo &cmdInfo);
	int compile2RegsIMM(CmdInfo &cmdInfo);
	int compileIMM(CmdInfo &cmdInfo);
	int compileIMMintoStr(int n, string& imm_str);
	int compileRegsIMM(CmdInfo &cmdInfo);
	int compileSINGLE(CmdInfo &cmdInfo);
	int compileGOTO(CmdInfo &cmdInfo);
	int compileJump(CmdInfo &cmdInfo);
	int compileLDFM(CmdInfo &cmdInfo);
	int compileSTFM(CmdInfo &cmdInfo);
	int compileLDFH(CmdInfo &cmdInfo);
	int compileLDFL(CmdInfo &cmdInfo);
	void appendTail(string &str, size_t num);
	void printbin(unsigned long x, int n);
	void processFP(float input, string& strHigh, string& strLow);

private:
	map<string, string> m_opcodeMap;
	map<int, string> m_regMap;
};