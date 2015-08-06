#ifndef PLIST_HELPER_H_INCLUDED
#define PLIST_HELPER_H_INCLUDED

#include "common.h"

#include <vector>
#include <map>

class CCObject
{
public:  
  static const char * ClassOfCCObject;
  static const char * ClassOfCCString;
  static const char * ClassOfCCInteger;
  static const char * ClassOfCCBool;
  static const char * ClassOfCCReal;
  static const char * ClassOfCCDate;
  static const char * ClassOfCCArray;
  static const char * ClassOfCCDictionary;
  virtual bool isOfClass(const char * szClass) { return strcmp(szClass, CCObject::ClassOfCCObject) == 0; }
};

class CCString : public CCObject, public std::wstring
{
public:
  CCString(const wchar_t * cszString) : std::wstring(cszString) {}
  CCString(const std::wstring & cszString) : std::wstring(cszString) {}
  CCString(const std::string & cszString) : std::wstring(ConvertToWString(cszString)) {}
  
  virtual bool isOfClass(const char * szClass) { if (strcmp(szClass, CCObject::ClassOfCCString) == 0) return true; else return CCObject::isOfClass(szClass); }
};

class CCInteger : public CCObject
{
public:
  CCInteger(int _v) : _integer(_v) {}
  virtual bool isOfClass(const char * szClass) { if (strcmp(szClass, CCObject::ClassOfCCInteger) == 0) return true; else return CCObject::isOfClass(szClass); }
  inline operator int() { return _integer; }
  
private:
  int _integer;
};

class CCBool : public CCObject
{
public:
  CCBool(bool _b) : _bool(_b) {}
  virtual bool isOfClass(const char * szClass) { if (strcmp(szClass, CCObject::ClassOfCCBool) == 0) return true; else return CCObject::isOfClass(szClass); }
  inline operator bool() { return _bool; }
  
private:
  bool _bool;
};

class CCReal : public CCObject
{
public:
  CCReal(double _v) : _real(_v) {}
  virtual bool isOfClass(const char * szClass) { if (strcmp(szClass, CCObject::ClassOfCCReal) == 0) return true; else return CCObject::isOfClass(szClass); }
  inline operator double() { return _real; }
  
private:
  double _real;
};

class CCDate : public CCObject
{
public:
  CCDate(unsigned long long _t) : _date(_t) {}
  virtual bool isOfClass(const char * szClass) { if (strcmp(szClass, CCObject::ClassOfCCDate) == 0) return true; else return CCObject::isOfClass(szClass); }
  inline operator unsigned long long() { return _date; }
  
private:
  unsigned long long _date;
};

class CCArray : public CCObject, public std::vector<CCObject *>
{
public:
  virtual bool isOfClass(const char * szClass) { if (strcmp(szClass, CCObject::ClassOfCCArray) == 0) return true; else return CCObject::isOfClass(szClass); }
};

class CCDictionary : public CCObject, public std::map<std::string, CCObject *>
{
public:
  virtual bool isOfClass(const char * szClass) { if (strcmp(szClass, CCObject::ClassOfCCDictionary) == 0) return true; else return CCObject::isOfClass(szClass); }
};

void Recurse(TiXmlNode * pParentNode, CCObject * pParentObj);
void RecurseDestroy(CCObject * pObj);
CCObject * LoadPlist(const char * szPlistFile);
bool SavePlist(const char * szPlistFile, CCObject * pObject);

void GetCCObjectValue(CCObject * pValueObject, char * rValue);
void GetCCObjectValue(CCObject * pValueObject, wchar_t * rValue);
void GetCCObjectValue(CCObject * pValueObject, std::string & rValue);
void GetCCObjectValue(CCObject * pValueObject, std::wstring & rValue);
void GetCCObjectValue(CCObject * pValueObject, float & rValue);
void GetCCObjectValue(CCObject * pValueObject, bool & rValue);
void GetCCObjectValue(CCObject * pValueObject, int & rValue);

#endif//PLIST_HELPER_H_INCLUDED
