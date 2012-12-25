#pragma once
#include <map>
#include <vector>
#include "CmdInfo.h"
using namespace std;


class Parser {
public:
	Parser();
	int PostProcessLabels();
	int ProcessLine(string assembly_cmd);
	vector<CmdInfo>& GetCommandsVec() {return m_commands;}
	~Parser();

protected:
	int parse3Regs(CmdInfo &cmdInfo);
	int parse2Regs(CmdInfo &cmdInfo);
	int parse2RegsIMM(CmdInfo &cmdInfo);
	int parse3Regs_FP(CmdInfo &cmdInfo);
	int parse2Regs_FP(CmdInfo &cmdInfo);
	int parse2RegsIMM_FP(CmdInfo &cmdInfo);
	int parseIMM(CmdInfo &cmdInfo);
	int parseRegsIMM(CmdInfo &cmdInfo);
	int parseGOTO(CmdInfo &cmdInfo);
	int parseJump(CmdInfo &cmdInfo);
    int parseLDFM(CmdInfo &cmdInfo);
	int parseSTFM(CmdInfo &cmdInfo);
	int parseLDFHL(CmdInfo &cmdInfo);
	int parseLDF(CmdInfo &cmdInfo);
	int parseF(CmdInfo &cmdInfo);
	int parseReg(string &assembly_str, int &r);
	int parseFReg(string &assembly_str, int &r);
	int parseHalfFP(CmdInfo &cmdInfo);
	void eliminatePreceedingWhitespaces(string &assembly_str);
	void eliminateSucceedingWhitespaces(string &assembly_str);
	int parseRemainingChars(string &assembly_str);

private:
	int m_cmd_count;
	vector<CmdInfo> m_commands;
	map<string, int> m_cmdMap;
	map<string, int> m_labelsMap;
	map<int, string> m_unresolvedLabels;
};