#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "Compiler.h"
#include "Parser.h"
using namespace std;


class CpuUserIntf {
public:
	CpuUserIntf();
	int Execute();
	~CpuUserIntf(){}
	void SetSrcFileName(wchar_t* filename);
	void SetDestFileName(wchar_t* filename);

protected:
	int ParseFile();
	int Compile();
	void PrintError(int ret);
	void PrintBinary(string &str, ofstream &fout);
	void PrintHeader(ofstream &fout);


private:
	int m_inputLineNum;
	int m_outputLineNum;
	wchar_t m_srcFileName[MAX_PATH];
	wchar_t m_destFileName[MAX_PATH];
	Parser m_parser;
	Compiler m_compiler;
};