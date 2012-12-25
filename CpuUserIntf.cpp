#include "stdafx.h"
#include "CpuUserIntf.h"
#include <iomanip>

CpuUserIntf::CpuUserIntf():m_inputLineNum(0), m_outputLineNum(0)
{
	SetSrcFileName(L"assembly.txt");
	SetDestFileName(L"C:\\project\\binary.mis");
}

int CpuUserIntf::Execute()
{
	int ret = NO_ERROR_FOUND;
	ret = ParseFile();
	if(ret == NO_ERROR_FOUND)
	  ret = Compile();
	if(ret == NO_ERROR_FOUND)
		MessageBox(NULL, L"Process successful!", L"Process result", MB_OK);
	return ret;
}

int CpuUserIntf::ParseFile()
{
	ifstream fin(m_srcFileName);

	char buffer[255];
	int ret = NO_ERROR_FOUND;

	while (fin.getline(buffer,255) && ret == NO_ERROR_FOUND)
	{
      m_inputLineNum++;
	  ret = m_parser.ProcessLine(buffer);
	  if (ret != NO_ERROR_FOUND)
		PrintError(ret);
	}
	if(ret == NO_ERROR_FOUND)
	{
	  m_parser.PostProcessLabels();
	}
	fin.close();
	return ret;
}

void CpuUserIntf::PrintError(int ret)
{
	wchar_t msg[MAX_PATH];
	wsprintf(msg, L"Line %d: ", m_inputLineNum);

	switch(ret)
	{
	case UNKNOWN_CMD:
		wcscat_s(msg, MAX_PATH, L"Unknown command.");
		break;
	case UNKNOWN_LABEL:
		wcscat_s(msg, MAX_PATH, L"Unknown label.");
		break;
	case ILLEGAL_REG_NO:
		wcscat_s(msg, MAX_PATH, L"Illegal register nubmer.");
		break;
	case ILLEGAL_IMM_VAL:
		wcscat_s(msg, MAX_PATH, L"Illegal immediate value.");
		break;
	case ILLEGAL_CMD_STX:
		wcscat_s(msg, MAX_PATH, L"Illegal command syntax.");
		break;
	}
	MessageBox(NULL, msg, L"Process result", MB_OK);
}

void CpuUserIntf::PrintBinary(string &str, ofstream &fout)
{
	if(m_outputLineNum <256)
		fout << "0";
	if(m_outputLineNum <16)
		fout << "0";
	fout << hex << m_outputLineNum << ": ";
	fout << str << ";\n";

	m_outputLineNum++;
}

void CpuUserIntf::PrintHeader(ofstream &fout)
{
	fout << "DEPTH = 1024;\n";
	fout << "WIDTH = 32;\n";
	fout << "ADDRESS_RADIX = HEX;\n";
	fout << "DATA_RADIX = BIN;\n";
	fout << "CONTENT\n";
	fout << "BEGIN\n";
}

int CpuUserIntf::Compile()
{
	ofstream fout(m_destFileName);
	int ret = NO_ERROR_FOUND;
	fout.clear();
    PrintHeader(fout);

	vector<CmdInfo> cmdInfoVec = m_parser.GetCommandsVec();
	vector<CmdInfo>::iterator it = cmdInfoVec.begin();
	for (; it != cmdInfoVec.end() && m_outputLineNum < 1023; it++)
	{
		CmdInfo &cmdInfo = *it;
		ret = m_compiler.ProcessCmd(cmdInfo);
		if (ret != NO_ERROR_FOUND)
			PrintError(ret);
		else
			PrintBinary(cmdInfo.GetBinaryStr(), fout);
	}

	//fill with nops to create a 1024 cmd lines chunk
	string empty;
	empty.clear();
	empty.append(32,'0');
	while(m_outputLineNum <1023)
		PrintBinary(empty, fout);

    fout << "END;";
	fout.close();
	return ret;
}

void CpuUserIntf::SetSrcFileName(wchar_t* filename)
{
	wcscpy_s(m_srcFileName,MAX_PATH, filename);
}

void CpuUserIntf::SetDestFileName(wchar_t* filename)
{
	wcscpy_s(m_destFileName,MAX_PATH, filename);
}