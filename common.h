#ifndef COMMON_H_INCLUDE
#define COMMON_H_INCLUDE

std::string IntToString(int iValue);
std::string FloatToString(double f);

std::wstring ConvertToWString(const std::string & szStr);
std::string ConvertToAString(const std::wstring & szStr);

void * LoadResourceFromFile(const char *szFilename, unsigned long *pSize);

bool LoadXMLFile(const char * szFileName, TiXmlDocument & doc);
bool SaveXMLFile(const char * szFileName, TiXmlDocument & doc);

#endif//COMMON_H_INCLUDE
