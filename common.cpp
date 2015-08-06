#include "stdafx.h"
#include "common.h"

std::string IntToString(int iValue)
{
  std::stringstream intString;
  intString << iValue;
  return intString.str();
}

std::string FloatToString(double f)
{
  if (f == 0) return "0.0";
  char szBuf[128] = {0};
  sprintf(szBuf, "%.20f", f);
  const char * pszDot = strrchr(szBuf, '.');
  if (pszDot != NULL) {
    size_t iLen = strlen(szBuf);
    char * pEnd = szBuf + iLen - 1;
    while (*pEnd == '0') {
      pEnd--;
    }
    if (pEnd == pszDot) {
      *pEnd = 0;
    } else {
      *(pEnd + 1) = 0;
    }
  }
  return szBuf;
}

#include "utf8/unchecked.h"
std::wstring ConvertToWString(const std::string & szStr)
{
  std::wstring utf16result;
  utf8::unchecked::utf8to16(szStr.begin(), szStr.end(), back_inserter(utf16result));
  return utf16result;
}
std::string ConvertToAString(const std::wstring & szStr)
{
  std::string utf8result;
  utf8::unchecked::utf16to8(szStr.begin(), szStr.end(), back_inserter(utf8result));
  return utf8result;
}

void * LoadResourceFromFile(const char *szFilename, unsigned long *pSize)
{
  unsigned char * pBuffer = NULL;
  FILE * fp = fopen(szFilename, "rb");
  if (fp)
  {
    fseek(fp,0,SEEK_END);
    *pSize = ftell(fp);
    fseek(fp,0,SEEK_SET);
    pBuffer = new unsigned char[*pSize+1];
    *pSize = (unsigned long)fread(pBuffer,sizeof(unsigned char), *pSize,fp);
    pBuffer[*pSize] = 0;
	fclose(fp);
  }
  return pBuffer;
}

bool LoadXMLFile(const char * szFileName, TiXmlDocument & doc)
{
  unsigned long size = 0;
  void * pData = LoadResourceFromFile(szFileName, &size);
  if (pData == NULL) return false;
  doc.Parse((char*)pData);
  return true;
}

bool SaveXMLFile(const char * szFileName, TiXmlDocument & doc)
{
  char szFullPath[_MAX_PATH];
  sprintf(szFullPath, "%s", szFileName);
  return doc.SaveFile(szFullPath);
}
