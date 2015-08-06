#ifndef CCBI_READER_H_INCLUDE
#define CCBI_READER_H_INCLUDE

class ccbiReader
{
public:
  ccbiReader(void * pData, int iSize);
  ~ccbiReader();
  unsigned char readByte();
  bool readBool();
  bool getBit();
  void alignBits();

  int readIntWithSign(bool sign);

  float readFloat();
  std::string readUTF8();
  size_t readBytes(void * _DstBuf, size_t _ElementSize, size_t _Count);
  void readStringCache();
  const std::string & readCachedString();
  void readCachedString(wchar_t * pBuf);
  void readCachedString(char * pBuf);
  
private:
  unsigned char* bytes;
  int byteSize;
  int currentByte;
  int currentBit;
  
  std::string * stringArray;
  int stringArraySize;
};

#endif//CCBI_READER_H_INCLUDE