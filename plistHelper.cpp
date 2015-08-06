#include "stdafx.h"
#include "plistHelper.h"
#include "common.h"

#include <time.h>
unsigned long long getTimeFromString(const char * cszTimeStr)
{
  struct tm tms;
  sscanf(cszTimeStr, "%4d-%2d-%2dT%2d:%2d:%2dZ", &tms.tm_year, &tms.tm_mon, &tms.tm_mday, &tms.tm_hour, &tms.tm_min, &tms.tm_sec);
  tms.tm_year-=1900;
  tms.tm_mon-=1;
  return (unsigned long long)mktime(&tms);
}

void getTimeString(unsigned long long t, char * cszTimeStr)
{
  time_t _time = (time_t)t;
  struct tm * tms = localtime(&_time);
  sprintf(cszTimeStr, "%4d-%02d-%02dT%02d:%02d:%02dZ", tms->tm_year+1900, tms->tm_mon+1, tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);
}

const char * CCObject::ClassOfCCObject     = "CCObject";
const char * CCObject::ClassOfCCString     = "CCString";
const char * CCObject::ClassOfCCInteger    = "CCInteger";
const char * CCObject::ClassOfCCBool       = "CCBool";
const char * CCObject::ClassOfCCReal       = "CCReal";
const char * CCObject::ClassOfCCDate       = "CCDate";
const char * CCObject::ClassOfCCArray      = "CCArray";
const char * CCObject::ClassOfCCDictionary = "CCDictionary";

void addObject(CCObject * pParentObj, const std::string & key, CCObject* obj)
{
  if (pParentObj->isOfClass("CCArray"))
  {
    CCArray * pArray = (CCArray*)pParentObj;
    pArray->push_back(obj);
  }
  else if (pParentObj->isOfClass("CCDictionary"))
  {
    CCDictionary * pDict = (CCDictionary *)pParentObj;
    pDict->insert(std::pair<std::string, CCObject *>(key, obj));
  }
}

void Recurse(TiXmlNode * pParentNode, CCObject * pParentObj)
{
  std::string keyValue;
  for (TiXmlNode * pNode = pParentNode->FirstChild(); pNode; pNode = pNode->NextSiblingElement())
  {
    TiXmlElement * pNodeNE = pNode->ToElement();
    if (pNodeNE == NULL) continue;
    const char * szValue = pNodeNE->Value();
    const char * szText = pNodeNE->GetText();
    if (_stricmp(szValue, "key") == 0)
    {
      keyValue = szText;
    }
    else if (_stricmp(szValue, "dict") == 0)
    {
      CCDictionary * pNewDict = new CCDictionary;
      addObject(pParentObj, keyValue, pNewDict);
        
      Recurse(pNode, pNewDict);
    }
    else if (_stricmp(szValue, "array") == 0)
    {
      CCArray * pNewArray = new CCArray();
      addObject(pParentObj, keyValue, pNewArray);
        
      Recurse(pNode, pNewArray);
    }
    else if (_stricmp(szValue, "string") == 0)
    {
      const char * szIn = szText == NULL ? "" : szText;
      std::wstring ret = ConvertToWString(szIn);
      CCString * pString = new CCString(ret.c_str());
      addObject(pParentObj, keyValue, pString);
    }
    else if (_stricmp(szValue, "false") == 0 || _stricmp(szValue, "true") == 0)
    {
      CCBool * pBool = new CCBool(_stricmp(szValue, "true") == 0);
      addObject(pParentObj, keyValue, pBool);
    }
    else if (_stricmp(szValue, "integer") == 0)
    {
      CCInteger * pInt = new CCInteger(atoi(szText));
      addObject(pParentObj, keyValue, pInt);
    }
    else if (_stricmp(szValue, "real") == 0)
    {
      CCReal * pReal = new CCReal(atof(szText));
      addObject(pParentObj, keyValue, pReal);
    }
    else if (_stricmp(szValue, "date") == 0)
    {
      CCDate * pDate = new CCDate(getTimeFromString(szText));
      addObject(pParentObj, keyValue, pDate);
    }
    else
    {
      assert(!"unknown value");
    }
  }
}

void RecurseDestroy(CCObject * pObj)
{
  if (pObj == NULL) return;
  if (pObj->isOfClass(CCObject::ClassOfCCDictionary))
  {
    CCDictionary * pDict = (CCDictionary *)pObj;
    CCDictionary::const_iterator itDict = pDict->begin();
    while(itDict != pDict->end())
    {
      RecurseDestroy(itDict->second);
      itDict++;
    }
    pDict->erase(pDict->begin(), pDict->end());
    V_SAFE_DELETE(pDict);
  }
  else if (pObj->isOfClass(CCObject::ClassOfCCArray))
  {
    CCArray * pArray = (CCArray *)pObj;
    int iCount = (int)pArray->size();
    for (int i = 0; i < iCount; i++)
    {
      RecurseDestroy((*pArray)[i]);
    }
    V_SAFE_DELETE(pArray);
  }
  else if (pObj->isOfClass(CCObject::ClassOfCCString))
  {
    CCString * pString = (CCString*)pObj;
    V_SAFE_DELETE(pString);
  }
  else
  {
    V_SAFE_DELETE(pObj);
  }
}

CCObject * LoadPlist(const char * szPlistFile)
{
  TiXmlDocument doc;
  if (!LoadXMLFile(szPlistFile, doc))
  {
    return NULL;
  }

  TiXmlElement * pRoot = doc.RootElement();
  if (!pRoot)
    return NULL;

  CCDictionary * pRootDict = new CCDictionary;
  Recurse(XMLHelper::SubNode(pRoot,"dict",false), pRootDict);
  
  return pRootDict;
}

void RecurseSave(TiXmlNode * pParentNode, CCObject * pParentObj)
{
  if (pParentObj->isOfClass(CCObject::ClassOfCCDictionary))
  {
    TiXmlElement * pDictElement = new TiXmlElement("dict");
    pParentNode->LinkEndChild(pDictElement);

    CCDictionary * pDict = (CCDictionary *)pParentObj;
    for (CCDictionary::iterator it = pDict->begin(); it != pDict->end(); it++)
    {
      TiXmlElement * pKeyElement = new TiXmlElement("key");
      {
        TiXmlText * pKeyText = new TiXmlText(it->first.c_str());
        pKeyElement->LinkEndChild(pKeyText);
      }
      pDictElement->LinkEndChild(pKeyElement);
      CCObject * pObject = it->second;
      RecurseSave(pDictElement, pObject);
    }
  }
  else if (pParentObj->isOfClass(CCObject::ClassOfCCArray))
  {
    TiXmlElement * pArrayElement = new TiXmlElement("array");
    pParentNode->LinkEndChild(pArrayElement);

    CCArray * pValue = (CCArray *)pParentObj;
    for (CCArray::iterator it = pValue->begin(); it != pValue->end(); it++)
    {
      CCObject * pObject = *it;
      RecurseSave(pArrayElement, pObject);
    }
  }
  else if (pParentObj->isOfClass(CCObject::ClassOfCCString))
  {
    CCString * pValue = (CCString *)pParentObj;
    TiXmlElement * pValueElement = new TiXmlElement("string");
    {
      TiXmlText * pValueText = new TiXmlText(ConvertToAString(pValue->c_str()).c_str());
      pValueElement->LinkEndChild(pValueText);
    }
    pParentNode->LinkEndChild(pValueElement);
  }
  else if (pParentObj->isOfClass(CCObject::ClassOfCCBool))
  {
    CCBool * pValue = (CCBool *)pParentObj;
    TiXmlElement * pValueElement = new TiXmlElement(*pValue ? "true" : "false");
    pParentNode->LinkEndChild(pValueElement);
  }
  else if (pParentObj->isOfClass(CCObject::ClassOfCCInteger))
  {
    CCInteger * pValue = (CCInteger *)pParentObj;
    TiXmlElement * pValueElement = new TiXmlElement("integer");
    {
      char szBuf[64] = {0};
      sprintf(szBuf, "%d", (int)(*pValue));
      TiXmlText * pValueText = new TiXmlText(szBuf);
      pValueElement->LinkEndChild(pValueText);
    }
    pParentNode->LinkEndChild(pValueElement);
  }
  else if (pParentObj->isOfClass(CCObject::ClassOfCCReal))
  {
    CCReal * pValue = (CCReal *)pParentObj;
    TiXmlElement * pValueElement = new TiXmlElement("real");
    {
      TiXmlText * pValueText = new TiXmlText(FloatToString((double)(*pValue)));
      pValueElement->LinkEndChild(pValueText);
    }
    pParentNode->LinkEndChild(pValueElement);
  }
  else if (pParentObj->isOfClass(CCObject::ClassOfCCDate))
  {
    CCDate * pValue = (CCDate *)pParentObj;
    TiXmlElement * pValueElement = new TiXmlElement("date");
    {
      char szBuf[64] = {0};
      getTimeString(*pValue, szBuf);
      TiXmlText * pValueText = new TiXmlText(szBuf);
      pValueElement->LinkEndChild(pValueText);
    }
    pParentNode->LinkEndChild(pValueElement);
  }
}

bool SavePlist(const char * szPlistFile, CCObject * pObject)
{
  TiXmlDocument doc;

  TiXmlDeclaration * pDeclaration = new TiXmlDeclaration("1.0","UTF-8","");
  doc.LinkEndChild(pDeclaration);

  TiXmlElement * pRoot = new TiXmlElement("plist");
  XMLHelper::Exchange_String(pRoot, "version", "1.0", true);
  doc.LinkEndChild(pRoot);

  RecurseSave(pRoot, pObject);

  if (!SaveXMLFile(szPlistFile, doc))
  {
    return false;
  }
  return true;
}

void GetCCObjectValue(CCObject * pValueObject, wchar_t * rValue)
{
  assert(pValueObject && pValueObject->isOfClass(CCObject::ClassOfCCString));
  CCString * pValue = (CCString *)pValueObject;
  wcscpy(rValue, pValue->c_str());
}

void GetCCObjectValue(CCObject * pValueObject, char * rValue)
{
  assert(pValueObject && pValueObject->isOfClass(CCObject::ClassOfCCString));
  CCString * pValue = (CCString *)pValueObject;
  strcpy(rValue, ConvertToAString(pValue->c_str()).c_str());
}

void GetCCObjectValue(CCObject * pValueObject, std::wstring & rValue)
{
  assert(pValueObject && pValueObject->isOfClass(CCObject::ClassOfCCString));
  CCString * pValue = (CCString *)pValueObject;
  rValue = pValue->c_str();
}

void GetCCObjectValue(CCObject * pValueObject, std::string & rValue)
{
  assert(pValueObject && pValueObject->isOfClass(CCObject::ClassOfCCString));
  CCString * pValue = (CCString *)pValueObject;
  rValue = ConvertToAString(pValue->c_str());
}

void GetCCObjectValue(CCObject * pValueObject, float & rValue)
{
  assert(pValueObject && pValueObject->isOfClass(CCObject::ClassOfCCReal));
  CCReal * pValue = (CCReal *)pValueObject;
  rValue = (float)(*pValue);
}

void GetCCObjectValue(CCObject * pValueObject, bool & rValue)
{
  assert(pValueObject);
  if (pValueObject->isOfClass(CCObject::ClassOfCCBool))
  {
    CCBool * pValue = (CCBool *)pValueObject;
    rValue = (*pValue);
  }
  else if (pValueObject->isOfClass(CCObject::ClassOfCCInteger))
  {
    CCInteger * pValue = (CCInteger *)pValueObject;
    rValue = (*pValue) != 0;
  }
}

void GetCCObjectValue(CCObject * pValueObject, int & rValue)
{
  assert(pValueObject && pValueObject->isOfClass(CCObject::ClassOfCCInteger));
  CCInteger * pValue = (CCInteger *)pValueObject;
  rValue = (*pValue);
}
