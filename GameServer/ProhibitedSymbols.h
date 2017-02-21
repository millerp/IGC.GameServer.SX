#ifndef PROHIBITEDSYMBOLS_H
#define PROHIBITEDSYMBOLS_H
#define TYPE_CHAT 0
#define TYPE_NAME 1
#define TYPE_GUILDNAME 2
struct _PROSYM_STRUCT{
	char symbol;
	BYTE system[3];
};
class CProhibitedSymbols
{
public:
	void LoadSymbolFile(LPSTR szFile);
	bool Validate(char* string, int len, BYTE Type);
	std::map<char,_PROSYM_STRUCT> m_ProhibitedSymbols;
}; extern CProhibitedSymbols g_prohibitedSymbols;

#endif