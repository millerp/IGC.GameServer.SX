#include "StdAfx.h"
#include "ProhibitedSymbols.h"
#include "TLog.h"

CProhibitedSymbols g_prohibitedSymbols;

void CProhibitedSymbols::LoadSymbolFile(LPSTR szFile)
{
	this->m_ProhibitedSymbols.clear();
	_PROSYM_STRUCT data;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFile);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("%s file loading error - %s", szFile, res.description());
		return;
	}

	pugi::xml_node main_section = file.child("ProhibitedSymbols");

	for (pugi::xml_node restrictsymbol = main_section.child("Restrict"); restrictsymbol; restrictsymbol = restrictsymbol.next_sibling())
	{
		memcpy(&data.symbol, restrictsymbol.attribute("Symbol").as_string(), 1);

		data.system[0] = restrictsymbol.attribute("Chat").as_int();
		data.system[1] = restrictsymbol.attribute("CharName").as_int();
		data.system[2] = restrictsymbol.attribute("GuildName").as_int();

		this->m_ProhibitedSymbols.insert(std::pair<char, _PROSYM_STRUCT>(data.symbol, data));
	}

	g_Log.Add("Loaded %d prohibited symbols", this->m_ProhibitedSymbols.size());
	return;
}

bool CProhibitedSymbols::Validate(char* string, int len, BYTE Type)
{
	for(int i=0;i<len;i++)
	{	
		std::map<char, _PROSYM_STRUCT>::iterator it =  this->m_ProhibitedSymbols.find(string[i]);
		if(it != this->m_ProhibitedSymbols.end() && it->second.system[Type] == 1)
		{
			return false;
		}
	}
	return true;
}