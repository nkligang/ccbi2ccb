#include "tinyxmlhelper.h"
#if defined(_WIN32)
#include <direct.h>
#define chdir _chdir
#else
#include "string.h"
#define _stricmp strcasecmp
#endif
#define FALSE 0

static char g_TempBuffer[256];

const char *XMLHelper::FloatsToString(const float *pArray, int iCount, char *szBuffer)
{
  if (!szBuffer) szBuffer=g_TempBuffer;
  const char *szStart = szBuffer;
  szBuffer[0]=0;
  int i;
  for (i=0;i<iCount;i++)
    if (i==0)
      szBuffer += sprintf(szBuffer,"%.4f",pArray[i]); else
      szBuffer += sprintf(szBuffer,",%.4f",pArray[i]);
  return szStart;
}

const char *XMLHelper::IntsToString(const int *pArray, int iCount, char *szBuffer)
{
  if (!szBuffer) szBuffer=g_TempBuffer;
  const char *szStart = szBuffer;
  szBuffer[0]=0;
  int i;
  for (i=0;i<iCount;i++)
    if (i==0)
      szBuffer += sprintf(szBuffer,"%i",pArray[i]); else
      szBuffer += sprintf(szBuffer,",%i",pArray[i]);
  return szStart;
}

const char *XMLHelper::UIntsToString(const unsigned int *pArray, int iCount, char *szBuffer)
{
  if (!szBuffer) szBuffer=g_TempBuffer;
  const char *szStart = szBuffer;
  szBuffer[0]=0;
  int i;
  for (i=0;i<iCount;i++)
    if (i==0)
      szBuffer += sprintf(szBuffer,"%u",pArray[i]); else
      szBuffer += sprintf(szBuffer,",%u",pArray[i]);
  return szStart;
}


int XMLHelper::StringToFloats(const char *szBuffer,float *pDestArray,int iMaxCount)
{
  if (!szBuffer || !szBuffer[0]) return 0;
  int iCount = 0;
  while (szBuffer && iCount<iMaxCount)
  {
    if (!sscanf(szBuffer,"%f",&pDestArray[iCount]))
      return iCount;
    szBuffer = strchr(szBuffer,',');
    if (szBuffer) szBuffer++;
    iCount++;
  }
  return iCount;
}

int XMLHelper::StringToInts(const char *szBuffer,int *pDestArray,int iMaxCount)
{
  if (!szBuffer || !szBuffer[0]) return 0;
  int iCount = 0;
  while (szBuffer && iCount<iMaxCount)
  {
    if (!sscanf(szBuffer,"%i",&pDestArray[iCount]))
      return iCount;
    szBuffer = strchr(szBuffer,',');
    if (szBuffer) szBuffer++;
    iCount++;
  }
  return iCount;
}

int XMLHelper::StringToUInts(const char *szBuffer,unsigned int *pDestArray,int iMaxCount)
{
  if (!szBuffer || !szBuffer[0]) return 0;
  int iCount = 0;
  while (szBuffer && iCount<iMaxCount)
  {
    if (!sscanf(szBuffer,"%u",&pDestArray[iCount]))
      return iCount;
    szBuffer = strchr(szBuffer,',');
    if (szBuffer) szBuffer++;
    iCount++;
  }
  return iCount;
}


/*const char *XMLHelper::ColorToString(VColorRef iColor, int iComponentCount,char *szBuffer)
{
  if (!szBuffer) szBuffer=g_TempBuffer;
  if (iComponentCount==4)
    sprintf(szBuffer,"%i,%i,%i,%i",(int)iColor.r,(int)iColor.g,(int)iColor.b,(int)iColor.a); else
    sprintf(szBuffer,"%i,%i,%i",(int)iColor.r,(int)iColor.g,(int)iColor.b);
  return szBuffer;
}

int XMLHelper::StringToColor(const char *szBuffer,VColorRef &dstColor,int iMaxCount)
{
  if (!szBuffer || !szBuffer[0]) return 0;
  if (szBuffer[0]=='#') // color format #aabbccdd
  {
    szBuffer++;
    unsigned int i=0;
    sscanf(szBuffer,"%x",i);
    dstColor.SetRGBA(i);
    return 4;
  }

  int iComponents[4];
  int iCount = sscanf(szBuffer,"%i,%i,%i,%i",&iComponents[0],&iComponents[1],&iComponents[2],&iComponents[3]);
  if (iCount>iMaxCount) iCount=iMaxCount;
  for (int i=0;i<iCount;i++)
  {
    int c = iComponents[i];
    if (c<0) c=0; else if (c>255) c=255;
    switch (i) // make it endianess independent
    {
      case 0:dstColor.r=c;break;
      case 1:dstColor.g=c;break;
      case 2:dstColor.b=c;break;
      case 3:dstColor.a=c;break;
    }
  }
  return iCount;
}*/



TiXmlElement *XMLHelper::SubNode(TiXmlElement *pParent,const char *szName, bool bWrite)
{
  if (bWrite)
  {
    assert(pParent);
    TiXmlElement newElem(szName);
    TiXmlNode *pNode = pParent->InsertEndChild(newElem);
    return pNode->ToElement();
  }
  if (!pParent)
    return NULL;
  TiXmlNode *pNode = pParent->FirstChild(szName);
  if (pNode) return pNode->ToElement();
  return NULL;
}

void XMLHelper::Exchange_Int(TiXmlElement *pParent,const char *szAttribName, int &iVar, bool bWrite)
{
  if (!pParent) return;
  if (bWrite)
    pParent->SetAttribute(szAttribName,iVar);
  else
    pParent->QueryIntAttribute(szAttribName,&iVar);
}

int XMLHelper::Exchange_Ints(TiXmlElement *pParent,const char *szAttribName, int *pArray, int iMaxCount, bool bWrite)
{
  if (!pParent) return 0;
  if (bWrite)
  {
    char * Buffer = new char[iMaxCount*18];
    pParent->SetAttribute(szAttribName,IntsToString(pArray,iMaxCount,Buffer));
    delete [] Buffer;
    return iMaxCount;
  }

  return StringToInts(pParent->Attribute(szAttribName),pArray,iMaxCount);
}


void XMLHelper::Exchange_UInt(TiXmlElement *pParent,const char *szAttribName, unsigned int &iVar, bool bWrite)
{
  if (!pParent) return;
  char szBuffer[256];
  
  if (bWrite)
    pParent->SetAttribute(szAttribName,UIntsToString(&iVar,1,szBuffer));
  else
    StringToUInts(pParent->Attribute(szAttribName),&iVar,1);
}

int XMLHelper::Exchange_UInts(TiXmlElement *pParent,const char *szAttribName, unsigned int *pArray, int iMaxCount, bool bWrite)
{
  if (!pParent) return 0;
  if (bWrite)
  {
    char * Buffer = new char[iMaxCount*18];
    pParent->SetAttribute(szAttribName,UIntsToString(pArray,iMaxCount,Buffer));
    delete [] Buffer;
    return iMaxCount;
  }

  return StringToUInts(pParent->Attribute(szAttribName),pArray,iMaxCount);
}


void XMLHelper::Exchange_Bool(TiXmlElement *pParent,const char *szAttribName, bool &bVar, bool bWrite)
{
  if (!pParent) return;
  if (bWrite)
    pParent->SetAttribute(szAttribName,bVar?"true":"false");
  else
  {
    const char *szVar = pParent->Attribute(szAttribName);
    if (!szVar || !szVar[0])
      return;
    if (!_stricmp(szVar,"true") || !_stricmp(szVar,"1") || !_stricmp(szVar,"yes")) bVar = true;
      else if (!_stricmp(szVar,"false") || !_stricmp(szVar,"0") || !_stricmp(szVar,"no")) bVar = false;
        else assert(FALSE);
  }
}

void XMLHelper::Exchange_Float(TiXmlElement *pParent,const char *szAttribName, float &fVar, bool bWrite)
{
  if (!pParent) return;
  if (bWrite)
    pParent->SetDoubleAttribute(szAttribName,fVar);
  else
  {
    double dVar = 0.0;
    if (pParent->QueryDoubleAttribute(szAttribName,&dVar)==TIXML_SUCCESS)
      fVar = (float)dVar;
  }
}

int XMLHelper::Exchange_Floats(TiXmlElement *pParent,const char *szAttribName, float *pArray, int iMaxCount, bool bWrite)
{
  if (!pParent) return 0;
  if (bWrite)
  {
    char * Buffer = new char[iMaxCount*18];
    pParent->SetAttribute(szAttribName,FloatsToString(pArray,iMaxCount,Buffer));
    delete [] Buffer;
    return iMaxCount;
  }

  return StringToFloats(pParent->Attribute(szAttribName),pArray,iMaxCount);
}


/*void XMLHelper::Exchange_Color(TiXmlElement *pParent,const char *szAttribName, VColorRef &iColor, bool bWrite)
{
  if (!pParent) return;
  if (bWrite)
    pParent->SetAttribute(szAttribName,ColorToString(iColor));
  else
    StringToColor(pParent->Attribute(szAttribName),iColor);
}*/


void XMLHelper::Exchange_BBox(TiXmlElement *pParent,const char *szAttribName, float pMin[3], float pMax[3], bool bWrite)
{
  if (!pParent) return;
  float fVal[6] = {pMin[0],pMin[1],pMin[2], pMax[0],pMax[1],pMax[2]};
  if (bWrite)
  {
    pParent->SetAttribute(szAttribName,FloatsToString(fVal,6));
  } else
  {
    StringToFloats(pParent->Attribute(szAttribName),fVal,6);
    for (int i=0;i<3;i++) 
      {pMin[i]=fVal[i];pMax[i]=fVal[i+3];}
  }
}


/*#define CHECK_TRANSP(eType,name) \
  if (bWrite) {if (iVar==eType) {pParent->SetAttribute(szAttribName,name);return true;} }\
    else {if (!_stricmp(szVal,name)) {iVar=eType;return true;}}

bool XMLHelper::Exchange_Transparency(TiXmlElement *pParent,const char *szAttribName, VIS_TransparencyType &iVar, bool bWrite)
{
  // in theory could use Exchange_Enum now
  if (!pParent) return false;
  const char *szVal=NULL;
  if (!bWrite)
  {
    szVal = pParent->Attribute(szAttribName);
    if (!szVal) return false;
  }

  // this function could also wrap around Exchange_Enum
  CHECK_TRANSP(VIS_TRANSP_NONE, "opaque");
  CHECK_TRANSP(VIS_TRANSP_MULTIPLICATIVE,"modulate");
  CHECK_TRANSP(VIS_TRANSP_ALPHA,"alpha");
  CHECK_TRANSP(VIS_TRANSP_ADDITIVE,"additive");
  CHECK_TRANSP(VIS_TRANSP_COLORKEY,"colorkey");
  CHECK_TRANSP(VIS_TRANSP_ADD_MODULATE,"addmodulate");
  CHECK_TRANSP(VIS_TRANSP_MODULATE2X,"modulate2x");
  CHECK_TRANSP(VIS_TRANSP_ALPHA_NOALPHATEST,"alphasmooth");
 
  return false;
}


bool XMLHelper::Exchange_LightingMethod(TiXmlElement *pParent, const char *szAttribName, VisLightingMethod_e &mode, bool bWrite)
{
  const char *szTypeNames[] = {"None","Lightmapping","LightGrid"};
  const int iTypeValues[] = {VIS_LIGHTING_NONE,VIS_LIGHTING_LIGHTMAPPING,VIS_LIGHTING_LIGHTGRID};
  return XMLHelper::Exchange_Enum(pParent,"Lighting",(int &)mode, sizeof(iTypeValues)/sizeof(int), szTypeNames, iTypeValues, bWrite);
}*/



bool XMLHelper::Exchange_Enum(TiXmlElement *pParent,const char *szAttribName, int &eVar, int iEnumCount, const char* pszEnumNames[], const int pEnumValues[], bool bWrite)
{
  if (!pParent) return false;
  const char *szVal=NULL;
  if (!bWrite)
  {
    szVal = pParent->Attribute(szAttribName);
    if (!szVal) return false;
  }
  for (int i=0;i<iEnumCount;i++)
  {
    const char *name=pszEnumNames[i];
    int iEnumVal = pEnumValues ? pEnumValues[i]:i; // use identity if pEnumValues is NULL
    if (bWrite) 
    {
      if (eVar==iEnumVal)
      {
        pParent->SetAttribute(szAttribName,name);
        return true;
      } 
    }
    else 
    {
      if (!_stricmp(szVal,name)) 
      {
        eVar=iEnumVal;
        return true;
      }
    }
  }

  return false; // not found
}

bool XMLHelper::Exchange_Bitmask(TiXmlElement *pParent,const char *szAttribName, int &eVar, int iEnumCount, const char* pszEnumNames[], const int pEnumValues[], bool bWrite)
{
  if (!pParent) 
    return false;

  char szLine[4096];
  szLine[0] = 0;

  if (bWrite)
  {
    char *szVal=szLine;
    int iUsedBits = 0;
    for (int i=0;i<iEnumCount;i++)
    {
      const char *name=pszEnumNames[i];
      int iEnumVal = pEnumValues ? pEnumValues[i] : (1<<i); // use bit #i if pEnumValues is NULL
      if ((eVar&iEnumVal)==iEnumVal)
      {
        if (iUsedBits>0)
          szVal += sprintf(szVal,"|%s",name);
        else
          szVal += sprintf(szVal,"%s",name);
        iUsedBits++;
      } 
    }

    pParent->SetAttribute(szAttribName,szLine);
    return true;
  } 
  else
  {
    const char *szVal = pParent->Attribute(szAttribName);
    if (!szVal) 
      return false;
    eVar = 0;

    while (szVal)
    {
      const char *p=strchr(szVal,'|');
      if (p)
      {
        int iLen = (int)(p-szVal);
        strncpy(szLine,szVal,iLen);
        szLine[iLen] = 0;
        szVal = p+1;
      } else
      {
        strcpy(szLine,szVal);
        szVal = NULL;
      }
      if (!szLine[0])
        continue;
      for (int i=0;i<iEnumCount;i++)
      {
        const char *name=pszEnumNames[i];
        int iEnumVal = pEnumValues ? pEnumValues[i] : (1<<i); // use bit #i if pEnumValues is NULL
        if (!_stricmp(szLine,name))
          eVar |= iEnumVal;
      }
    }
  }

  return true;
}



const char* XMLHelper::Exchange_String(TiXmlElement *pParent,const char *szAttribName, const char *szString, bool bWrite)
{
  if (!pParent) return NULL;
  if (bWrite)
  {
    if (szString)
      pParent->SetAttribute(szAttribName,szString);
    return NULL; // only for loading != NULL
  }
  return pParent->Attribute(szAttribName);
}


/*const char* XMLHelper::Exchange_VString(TiXmlElement *pParent,const char *szAttribName, VString &sString, bool bWrite)
{
  if (!pParent) return NULL;
  if (bWrite)
  {
    if (!sString.IsEmpty())
      pParent->SetAttribute(szAttribName,sString.AsChar());
    return NULL; // only for loading != NULL
  }
  sString = pParent->Attribute(szAttribName);
  return sString.AsChar();
}*/
/*#include "Path.h"
*/
