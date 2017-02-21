#ifndef FORMULA_PARSE_H
#define FORMULA_PARSE_H

enum FORMULA_DATA_TYPE
{
	FORMULA_TYPE_EXP = 0,
	FORMULA_TYPE_EXCELLENT = 1,
	FORMULA_TYPE_UNK = 2
};

#pragma once
class CFormulaParse
{
public:
	CFormulaParse();
	~CFormulaParse();

	void LoadFile(char *szFile);
	std::string * GetFormula(int iFormulaType, int iFormulaID);

	double UseFormula(int iFormulaType, int iFormulaID, char * ArgNames, int iArgCount, ...);

private:

	bool m_bFileLoad;
	std::map<int, std::string> m_mapExpFormulaData;
	std::map<int, std::string> m_mapExcOptFormulaData;
};

extern CFormulaParse g_FormulaParse;

#endif