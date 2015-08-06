#ifndef TINYXML_HELPER_H_INCLUDED
#define TINYXML_HELPER_H_INCLUDED

#include "tinyxml.h"

// Helper functions based on TinyXml
namespace XMLHelper
{
  // string <-> value conversion
  const char *IntsToString(const int *pArray, int iCount, char *szBuffer=NULL);
  int StringToInts(const char *szBuffer,int *pDestArray,int iMaxCount);
  const char *UIntsToString(const unsigned int *pArray, int iCount, char *szBuffer=NULL);
  int StringToUInts(const char *szBuffer,unsigned int *pDestArray,int iMaxCount);

  const char *FloatsToString(const float *pArray, int iCount, char *szBuffer=NULL);
  int StringToFloats(const char *szBuffer,float *pDestArray,int iMaxCount);

  /*const char *ColorToString(VColorRef iColor, int iComponentCount=4,char *szBuffer=NULL);
  int StringToColor(const char *szBuffer,VColorRef &dstColor,int iMaxCount=4);*/

  // serialising
  TiXmlElement *SubNode(TiXmlElement *pParent,const char *szName, bool bWrite);

  void Exchange_Int(TiXmlElement *pParent,const char *szAttribName, int &iVar, bool bWrite);
  int Exchange_Ints(TiXmlElement *pParent,const char *szAttribName, int *pArray, int iMaxCount, bool bWrite);
  void Exchange_UInt(TiXmlElement *pParent,const char *szAttribName, unsigned int &iVar, bool bWrite);
  int Exchange_UInts(TiXmlElement *pParent,const char *szAttribName, unsigned int *pArray, int iMaxCount, bool bWrite);
  void Exchange_Bool(TiXmlElement *pParent,const char *szAttribName, bool &bVar, bool bWrite);
  void Exchange_Float(TiXmlElement *pParent,const char *szAttribName, float &fVar, bool bWrite);
  int Exchange_Floats(TiXmlElement *pParent,const char *szAttribName, float *pArray, int iMaxCount, bool bWrite);
 // void Exchange_Color(TiXmlElement *pParent,const char *szAttribName, VColorRef &iColor, bool bWrite);
  void Exchange_BBox(TiXmlElement *pParent,const char *szAttribName, float pMin[3], float pMax[3], bool bWrite);
  const char* Exchange_String(TiXmlElement *pParent,const char *szAttribName, const char *szString, bool bWrite);
  //const char* Exchange_VString(TiXmlElement *pParent,const char *szAttribName, VString &sString, bool bWrite);
  //bool Exchange_Transparency(TiXmlElement *pParent,const char *szAttribName, VIS_TransparencyType &iVar, bool bWrite);
  //bool Exchange_LightingMethod(TiXmlElement *pParent, const char *szAttribName, VisLightingMethod_e &mode, bool bWrite);
  bool Exchange_Enum(TiXmlElement *pParent,const char *szAttribName, int &eVar, int iEnumCount, const char* pszEnumNames[], const int pEnumValues[], bool bWrite);
  bool Exchange_Bitmask(TiXmlElement *pParent,const char *szAttribName, int &eVar, int iEnumCount, const char* pszEnumNames[], const int pEnumValues[], bool bWrite);
}

#endif
