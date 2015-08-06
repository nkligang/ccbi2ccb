#include "stdafx.h"
#include "ccbiReader.h"
#include "common.h"

ccbiReader::ccbiReader(void * pData, int iSize) {
  bytes = (unsigned char*)pData;
  byteSize = iSize;
  currentByte = 0;
  currentBit = 0;
  stringArray = NULL;
  stringArraySize = 0;
}

ccbiReader::~ccbiReader()
{
  delete [] stringArray;
  stringArray = 0;
}

bool ccbiReader::readBool()
{
  return readByte() != 0;
}

unsigned char ccbiReader::readByte()
{
  unsigned char byte = bytes[currentByte];
  currentByte++;
  return byte;
}

bool ccbiReader::getBit() 
{
  bool bit;
  unsigned char byte = *(bytes+currentByte);
  if (byte & (1 << currentBit)) bit = true;
  else bit = false;
  
  currentBit++;
  if (currentBit >= 8)
  {
    currentBit = 0;
    currentByte++;
  }
  return bit;
}

void ccbiReader::alignBits()
{
  if (currentBit) {
    currentBit = 0;
    currentByte++;
  }
}

int ccbiReader::readIntWithSign(bool sign)
{
  // Read encoded int
  int numBits = 0;
  while (!getBit())
  {
    numBits++;
  }
    
  long long current = 0;
  for (int a=numBits-1; a >= 0; a--)
  {
    if (getBit())
    {
      current |= (long long)(1 << a);
    }
  }
  current |= (long long)(1 << numBits);
    
  int num;
  if (sign)
  {
    int s = (int)(current%2);
    if (s) num = (int)(current/2);
    else num = (int)(-current/2);
  }
  else
  {
    num = (int)(current-1);
  }
    
  alignBits();
    
  return num;
}

float ccbiReader::readFloat()
{
  enum {
    kCCBFloat0 = 0,
    kCCBFloat1,
    kCCBFloatMinus1,
    kCCBFloat05,
    kCCBFloatInteger,
    kCCBFloatFull
  };
  unsigned char type = readByte();

  switch (type) {
    case kCCBFloat0:
      return 0;
    case kCCBFloat1:
      return 1;
    case kCCBFloatMinus1:
      return -1;
    case kCCBFloat05:
      return 0.5f;
    case kCCBFloatInteger:
      return (float)readIntWithSign(true);
    default: {
      // Copy the float byte by byte
      // memcpy dosn't work on latest Xcode (4.6)
      float * pF = (float*)(bytes+currentByte);
      float f = 0;
            
      unsigned char* src = (unsigned char*) pF;
      unsigned char* dst = (unsigned char*) &f;
            
      for (int i = 0; i < 4; i++)
      {
        dst[i] = src[i];
      }
           
      currentByte+=4;
      return f;
    }
  }
}

std::string ccbiReader::readUTF8()
{
  int b0 = readByte();
  int b1 = readByte();
  int numBytes = b0 << 8 | b1;
  std::string str((char*)(bytes+currentByte), numBytes);
  currentByte += numBytes;
  return str;
}

size_t ccbiReader::readBytes(void * _DstBuf, size_t _ElementSize, size_t _Count)
{
  size_t _s = _ElementSize * _Count;
  if (currentByte + _s <= (size_t)byteSize)
  {
    memcpy(_DstBuf, (char*)bytes + currentByte, _s);
    currentByte += (int)_s;
    return _s;
  }
  else if (currentByte < byteSize)
  {
    memcpy(_DstBuf, (char*)bytes + currentByte, byteSize - currentByte);
    currentByte = byteSize;
    return byteSize - currentByte;
  }
  else
  {
    return 0;
  }
}

void ccbiReader::readStringCache()
{
  stringArraySize = readIntWithSign(false);
  if (stringArraySize > 0) stringArray = new std::string[stringArraySize];
  for (int i = 0; i < stringArraySize; i++)
  {
    stringArray[i] = readUTF8();
  }
}

const std::string & ccbiReader::readCachedString()
{
  int n = readIntWithSign(false);
  assert(n >= 0 && n < stringArraySize);
  return stringArray[n];
}

void ccbiReader::readCachedString(wchar_t * pBuf)
{
  const std::string & rStr = readCachedString();
  wcscpy(pBuf, ConvertToWString(rStr).c_str());
}

void ccbiReader::readCachedString(char * pBuf)
{
  const std::string & rStr = readCachedString();
  strcpy(pBuf, rStr.c_str());
}
