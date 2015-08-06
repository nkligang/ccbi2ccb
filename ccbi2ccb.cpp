// ccbi2ccb.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "plistHelper.h"
#include "common.h"
#include "ccbiReader.h"

enum {
  kCCBPlatformAll = 0,
  kCCBPlatformIOS,
  kCCBPlatformMac
};

enum {
  kCCBCustomPropertyTypeInt = 0,
  kCCBCustomPropertyTypeFloat,
  kCCBCustomPropertyTypeBool,
  kCCBCustomPropertyTypeString,
};

enum {
  kCCBKeyframeEasingInstant = 0,
    
  kCCBKeyframeEasingLinear,
    
  kCCBKeyframeEasingCubicIn,
  kCCBKeyframeEasingCubicOut,
  kCCBKeyframeEasingCubicInOut,
    
  kCCBKeyframeEasingElasticIn,
  kCCBKeyframeEasingElasticOut,
  kCCBKeyframeEasingElasticInOut,
   
  kCCBKeyframeEasingBounceIn,
  kCCBKeyframeEasingBounceOut,
  kCCBKeyframeEasingBounceInOut,
    
  kCCBKeyframeEasingBackIn,
  kCCBKeyframeEasingBackOut,
  kCCBKeyframeEasingBackInOut,
};

enum {
  kCCBPropTypePosition = 0,
  kCCBPropTypeSize,
  kCCBPropTypePoint,
  kCCBPropTypePointLock,
  kCCBPropTypeScaleLock,
  kCCBPropTypeDegrees,
  kCCBPropTypeInteger,
  kCCBPropTypeFloat,
  kCCBPropTypeFloatVar,
  kCCBPropTypeCheck,
  kCCBPropTypeSpriteFrame,
  kCCBPropTypeTexture,
  kCCBPropTypeByte,
  kCCBPropTypeColor3,
  kCCBPropTypeColor4FVar,
  kCCBPropTypeFlip,
  kCCBPropTypeBlendmode,
  kCCBPropTypeFntFile,
  kCCBPropTypeText,
  kCCBPropTypeFontTTF,
  kCCBPropTypeIntegerLabeled,
  kCCBPropTypeBlock,
  kCCBPropTypeAnimation,
  kCCBPropTypeCCBFile,
  kCCBPropTypeString,
  kCCBPropTypeBlockCCControl,
  kCCBPropTypeFloatScale,
  kCCBPropTypeFloatXY,
};

enum {
  kCCBKeyframePropTypeVisible = 1,
  kCCBKeyframePropTypeRotation = 2,
  kCCBKeyframePropTypePosition = 3,
  kCCBKeyframePropTypeScale = 4,
  kCCBKeyframePropTypeOpacity = 5,
  kCCBKeyframePropTypeColor = 6,
  kCCBKeyframePropTypeDisplayFrame = 7,
  kCCBKeyframePropTypeSkew = 8,
};

const char * m_cszStandardNodeClass[] = {
  "CCNode",
  "CCLayer",
  "CCLayerColor",
  "CCScrollView",
  "CCSprite",
  "CCScale9Sprite",
  "CCLabelTTF",
  "CCLabelBMFont",
  "CCMenu",
  "CCMenuItemImage",
  "CCControlButton",
  "CCParticleSystemQuad",
  "CCBFile",
};
bool isStandardNodeClass(const std::string & klass)
{
  int iLen = sizeof(m_cszStandardNodeClass)/sizeof(const char *);
  for (int i = 0; i < iLen; i++)
  {
    if (klass == m_cszStandardNodeClass[i])
      return true;
  }
  return false;
}
std::map<std::string, std::map<std::string, std::string>> m_mapClassProperties;
std::map<std::string, int> m_mapClassPriorty;
void buildStandardClassProperties()
{
  {
    std::map<std::string, std::string> properties;
    properties.insert(std::pair<std::string, std::string>("touchEnabled", ""));
    properties.insert(std::pair<std::string, std::string>("isTouchEnabled", ""));
    properties.insert(std::pair<std::string, std::string>("accelerometerEnabled", ""));
    properties.insert(std::pair<std::string, std::string>("isAccelerometerEnabled", ""));
    properties.insert(std::pair<std::string, std::string>("mouseEnabled", ""));
    properties.insert(std::pair<std::string, std::string>("isMouseEnabled", ""));
    properties.insert(std::pair<std::string, std::string>("keyboardEnabled", ""));
    properties.insert(std::pair<std::string, std::string>("isKeyboardEnabled", ""));
    m_mapClassProperties.insert(std::pair<std::string, std::map<std::string, std::string>>("CCLayer", properties));
  }
  {
    std::map<std::string, std::string> properties;
    properties.insert(std::pair<std::string, std::string>("blendFunc", ""));
    m_mapClassProperties.insert(std::pair<std::string, std::map<std::string, std::string>>("CCLayerColor", properties));
  }
  {
    std::map<std::string, std::string> properties;
    properties.insert(std::pair<std::string, std::string>("container", ""));
    properties.insert(std::pair<std::string, std::string>("direction", ""));
    properties.insert(std::pair<std::string, std::string>("clipsToBounds", ""));
    properties.insert(std::pair<std::string, std::string>("bounces", ""));
    m_mapClassProperties.insert(std::pair<std::string, std::map<std::string, std::string>>("CCScrollView", properties));
  }
  {
    std::map<std::string, std::string> properties;
    properties.insert(std::pair<std::string, std::string>("displayFrame", ""));
    m_mapClassProperties.insert(std::pair<std::string, std::map<std::string, std::string>>("CCSprite", properties));
  }
  {
    std::map<std::string, std::string> properties;
    properties.insert(std::pair<std::string, std::string>("insetLeft", ""));
    properties.insert(std::pair<std::string, std::string>("insetTop", ""));
    properties.insert(std::pair<std::string, std::string>("insetRight", ""));
    properties.insert(std::pair<std::string, std::string>("insetBottom", ""));
    m_mapClassProperties.insert(std::pair<std::string, std::map<std::string, std::string>>("CCScale9Sprite", properties));
  }
  {
    std::map<std::string, std::string> properties;
    properties.insert(std::pair<std::string, std::string>("fontName", ""));
    properties.insert(std::pair<std::string, std::string>("fontSize", ""));
    properties.insert(std::pair<std::string, std::string>("dimensions", ""));
    properties.insert(std::pair<std::string, std::string>("horizontalAlignment", ""));
    properties.insert(std::pair<std::string, std::string>("verticalAlignment", ""));
    m_mapClassProperties.insert(std::pair<std::string, std::map<std::string, std::string>>("CCLabelTTF", properties));
  }
  {
    std::map<std::string, std::string> properties;
    properties.insert(std::pair<std::string, std::string>("fntFile", ""));
    m_mapClassProperties.insert(std::pair<std::string, std::map<std::string, std::string>>("CCLabelBMFont", properties));
  }
  {
    std::map<std::string, std::string> properties;
    properties.insert(std::pair<std::string, std::string>("block", ""));
    properties.insert(std::pair<std::string, std::string>("isEnabled", ""));
    properties.insert(std::pair<std::string, std::string>("normalSpriteFrame", ""));
    properties.insert(std::pair<std::string, std::string>("selectedSpriteFrame", ""));
    properties.insert(std::pair<std::string, std::string>("disabledSpriteFrame", ""));
    m_mapClassProperties.insert(std::pair<std::string, std::map<std::string, std::string>>("CCMenuItemImage", properties));
  }
  {
    std::map<std::string, std::string> properties;
    properties.insert(std::pair<std::string, std::string>("ccControl", ""));
    properties.insert(std::pair<std::string, std::string>("enabled", ""));
    properties.insert(std::pair<std::string, std::string>("title|1", ""));
    properties.insert(std::pair<std::string, std::string>("titleTTF|1", ""));
    properties.insert(std::pair<std::string, std::string>("titleTTFSize|1", ""));
    properties.insert(std::pair<std::string, std::string>("zoomOnTouchDown", ""));
    properties.insert(std::pair<std::string, std::string>("backgroundSpriteFrame|1", ""));
    properties.insert(std::pair<std::string, std::string>("titleColor|1", ""));
    properties.insert(std::pair<std::string, std::string>("backgroundSpriteFrame|2", ""));
    properties.insert(std::pair<std::string, std::string>("titleColor|2", ""));
    properties.insert(std::pair<std::string, std::string>("backgroundSpriteFrame|3", ""));
    properties.insert(std::pair<std::string, std::string>("titleColor|3", ""));
    m_mapClassProperties.insert(std::pair<std::string, std::map<std::string, std::string>>("CCControlButton", properties));
  }
  {
    std::map<std::string, std::string> properties;
    properties.insert(std::pair<std::string, std::string>("emitterMode", ""));
    properties.insert(std::pair<std::string, std::string>("posVar", ""));
    properties.insert(std::pair<std::string, std::string>("emissionRate", ""));
    properties.insert(std::pair<std::string, std::string>("duration", ""));
    properties.insert(std::pair<std::string, std::string>("totalParticles", ""));
    properties.insert(std::pair<std::string, std::string>("life", ""));
    properties.insert(std::pair<std::string, std::string>("startSize", ""));
    properties.insert(std::pair<std::string, std::string>("endSize", ""));
    properties.insert(std::pair<std::string, std::string>("startSpin", ""));
    properties.insert(std::pair<std::string, std::string>("endSpin", ""));
    properties.insert(std::pair<std::string, std::string>("angle", ""));
    properties.insert(std::pair<std::string, std::string>("startColor", ""));
    properties.insert(std::pair<std::string, std::string>("endColor", ""));
    properties.insert(std::pair<std::string, std::string>("gravity", ""));
    properties.insert(std::pair<std::string, std::string>("speed", ""));
    properties.insert(std::pair<std::string, std::string>("tangentialAccel", ""));
    properties.insert(std::pair<std::string, std::string>("radialAccel", ""));
    m_mapClassProperties.insert(std::pair<std::string, std::map<std::string, std::string>>("CCParticleSystemQuad", properties));
  }
  m_mapClassPriorty.insert(std::pair<std::string, int>("CCLayer", 1));
  m_mapClassPriorty.insert(std::pair<std::string, int>("CCLayerColor", 2));
  m_mapClassPriorty.insert(std::pair<std::string, int>("CCScrollView", 2));
  m_mapClassPriorty.insert(std::pair<std::string, int>("CCSprite", 2));
  m_mapClassPriorty.insert(std::pair<std::string, int>("CCScale9Sprite", 2));
  m_mapClassPriorty.insert(std::pair<std::string, int>("CCLabelTTF", 2));
  m_mapClassPriorty.insert(std::pair<std::string, int>("CCLabelBMFont", 2));
  m_mapClassPriorty.insert(std::pair<std::string, int>("CCMenu", 2));
  m_mapClassPriorty.insert(std::pair<std::string, int>("CCMenuItemImage", 2));
  m_mapClassPriorty.insert(std::pair<std::string, int>("CCControlButton", 2));
  m_mapClassPriorty.insert(std::pair<std::string, int>("CCParticleSystemQuad", 3));
}

std::string decideStandardClass(CCArray * pPropertiesArray)
{
  std::map<std::string, int> m_mapPropertyCount;
  for (CCArray::iterator it = pPropertiesArray->begin(); it != pPropertiesArray->end(); it++)
  {
    CCObject * pObject = *it;
    if (pObject->isOfClass("CCDictionary")) {
      CCDictionary * pDict = (CCDictionary *)pObject;
      CCDictionary::iterator itFound = pDict->find("name");
      if (itFound != pDict->end()) {
        const std::string & key = itFound->first;
        const std::string & value = ConvertToAString(*((CCString*)itFound->second));
        std::map<std::string, std::map<std::string, std::string>>::iterator itClass = m_mapClassProperties.begin();
        for (; itClass != m_mapClassProperties.end(); itClass++)
        {
          std::map<std::string, std::string> & classProperties = itClass->second;
          if (classProperties.find(value) != classProperties.end()) {
            if (m_mapPropertyCount.find(itClass->first) == m_mapPropertyCount.end()) {
              m_mapPropertyCount.insert(std::pair<std::string, int>(itClass->first, 0));
            }
            m_mapPropertyCount[itClass->first]++;
          }
        }
      }
    }
  }
  if (m_mapPropertyCount.size() == 1) {
    return m_mapPropertyCount.begin()->first;
  } else if (m_mapPropertyCount.size() > 1) {
    std::map<std::string, int>::iterator itClass = m_mapPropertyCount.begin();
    std::string klass = itClass->first;
    int prority = 0;
    itClass++;
    for (; itClass != m_mapPropertyCount.end(); itClass++)
    {
      std::map<std::string, int>::iterator parenetIt = m_mapClassPriorty.find(itClass->first);
      if (parenetIt != m_mapClassPriorty.end())
      {
        if (parenetIt->second > prority) {
          klass = itClass->first;
          prority = parenetIt->second;
        }
      }
    }
    return klass;
  }
  return "CCNode";
}

bool m_bJSControlled = false;
bool readNodeRecurse(ccbiReader & ccbi, CCDictionary * pDict)
{
  std::string baseClass;
  std::string displayName;
  // Read class
  std::string customClass = ccbi.readCachedString();
  bool bIsCustomClassIsStandardClass = isStandardNodeClass(customClass);
  if (bIsCustomClassIsStandardClass) {
    baseClass = customClass;
    displayName = customClass;
    customClass = "";
  } else {
    baseClass = "CCNode";
    displayName = customClass;
  }
  if (m_bJSControlled)
  {
    std::string jsControllerName = ccbi.readCachedString();
  }
  // Read assignment type and name
  int memberVarAssignmentType = ccbi.readIntWithSign(false);
  pDict->insert(std::pair<std::string, CCObject *>("memberVarAssignmentType", new CCInteger(memberVarAssignmentType)));
  std::string memberVarAssignmentName;
  if (memberVarAssignmentType)
  {
    memberVarAssignmentName = ccbi.readCachedString();
  }
  pDict->insert(std::pair<std::string, CCObject *>("memberVarAssignmentName", new CCString(memberVarAssignmentName)));
  // Read animated properties
  bool bAnimatedScale = false;
  bool bAnimatedOpacity = false;
  bool bAnimatedRotation = false;
  bool bAnimatedVisible = false;
  bool bAnimatedPosition = false;
  bool bAnimatedSkew = false;
  bool bAnimatedColor = false;
  bool bAnimatedDisplayFrame = false;
  int bAnimatedVisibleKeyframeCount = 0;
  CCDictionary * pSequencesDict = new CCDictionary();
  int numSequences = ccbi.readIntWithSign(false);
  for (int i = 0; i < numSequences; i++)
  {
    CCDictionary * pSequenceDict = new CCDictionary();
    int seqId = ccbi.readIntWithSign(false);
    int numProps = ccbi.readIntWithSign(false);
    for (int j = 0; j < numProps; j++)
    {
      CCDictionary * pSequencePropDict = new CCDictionary();
      std::string szKey = ccbi.readCachedString();
      int seqPropType = ccbi.readIntWithSign(false);
      int keyframeType = kCCBKeyframePropTypeOpacity;
      if (szKey == "opacity") {
        keyframeType = kCCBKeyframePropTypeOpacity;
        bAnimatedOpacity = true;
      } else if (szKey == "scale") {
        keyframeType = kCCBKeyframePropTypeScale;
        bAnimatedScale = true;
      } else if (szKey == "visible") {
        keyframeType = kCCBKeyframePropTypeVisible;
        bAnimatedVisible = true;
      } else if (szKey == "rotation") {
        keyframeType = kCCBKeyframePropTypeRotation;
        bAnimatedRotation = true;
      } else if (szKey == "position") {
        keyframeType = kCCBKeyframePropTypePosition;
        bAnimatedPosition = true;
      } else if (szKey == "skew") {
        keyframeType = kCCBKeyframePropTypeSkew;
        bAnimatedSkew = true;
      } else if (szKey == "color") {
        keyframeType = kCCBKeyframePropTypeColor;
        bAnimatedColor = true;
      } else if (szKey == "displayFrame") {
        keyframeType = kCCBKeyframePropTypeDisplayFrame;
        bAnimatedDisplayFrame = true;
      } else {
        assert(false);
      }
      CCArray * pKeyframesArray = new CCArray();
      int numKeyframes = ccbi.readIntWithSign(false);
      for (int k = 0; k < numKeyframes; k++)
      {
        CCDictionary * pKeyframeDict = new CCDictionary();
        float time = ccbi.readFloat();
        pKeyframeDict->insert(std::pair<std::string, CCObject *>("time", new CCReal(time)));
        CCDictionary * pEasingDict = new CCDictionary();
        int type = ccbi.readIntWithSign(false);
        pEasingDict->insert(std::pair<std::string, CCObject *>("type", new CCInteger(type)));
        float opt = 0;
        if (type == kCCBKeyframeEasingCubicIn || 
            type == kCCBKeyframeEasingCubicOut || 
            type == kCCBKeyframeEasingCubicInOut || 
            type == kCCBKeyframeEasingElasticIn || 
            type == kCCBKeyframeEasingElasticOut || 
            type == kCCBKeyframeEasingElasticInOut)
        {
          opt = ccbi.readFloat();
          pEasingDict->insert(std::pair<std::string, CCObject *>("opt", new CCReal(opt)));
        }
        pKeyframeDict->insert(std::pair<std::string, CCObject *>("easing", pEasingDict));
        pKeyframeDict->insert(std::pair<std::string, CCObject *>("name", new CCString(szKey)));
        pKeyframeDict->insert(std::pair<std::string, CCObject *>("type", new CCInteger(keyframeType)));
        if (seqPropType == kCCBPropTypeCheck)
        {
          bool value = ccbi.readBool();
          pKeyframeDict->insert(std::pair<std::string, CCObject *>("value", new CCBool(true)));
          if (k == 0 && szKey == "visible" && !value) {
            V_SAFE_DELETE(pKeyframeDict);
            V_SAFE_DELETE(pEasingDict);
            continue;
          }
          bAnimatedVisibleKeyframeCount++;
        }
        else if (seqPropType == kCCBPropTypeByte)
        {
          unsigned char value = ccbi.readByte();
          pKeyframeDict->insert(std::pair<std::string, CCObject *>("value", new CCInteger(value)));
        }
        else if (seqPropType == kCCBPropTypeColor3)
        {
          CCArray * pValueArray = new CCArray();
          unsigned char v1 = ccbi.readByte();
          pValueArray->push_back(new CCInteger(v1));
          unsigned char v2 = ccbi.readByte();
          pValueArray->push_back(new CCInteger(v2));
          unsigned char v3 = ccbi.readByte();
          pValueArray->push_back(new CCInteger(v3));
          pKeyframeDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
        }
        else if (seqPropType == kCCBPropTypeSpriteFrame)
        {
          CCArray * pValueArray = new CCArray();
          std::string v1 = ccbi.readCachedString();
          std::string v2 = ccbi.readCachedString();
          pValueArray->push_back(new CCString(v2));
          pValueArray->push_back(new CCString(v1));
          pKeyframeDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
        }
        else if (seqPropType == kCCBPropTypeScaleLock || 
                 seqPropType == kCCBPropTypePosition || 
                 seqPropType == kCCBPropTypeFloatXY)
        {
          CCArray * pValueArray = new CCArray();
          float v1 = ccbi.readFloat();
          pValueArray->push_back(new CCReal(v1));
          float v2 = ccbi.readFloat();
          pValueArray->push_back(new CCReal(v2));
          pKeyframeDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
        }
        else if (seqPropType == kCCBPropTypeDegrees)
        {
          float value = ccbi.readFloat();
          pKeyframeDict->insert(std::pair<std::string, CCObject *>("value", new CCReal(value)));
        }
        else
        {
          assert(false);
        }
        pKeyframesArray->push_back(pKeyframeDict);
      }
      pSequencePropDict->insert(std::pair<std::string, CCObject *>("keyframes", pKeyframesArray));
      pSequencePropDict->insert(std::pair<std::string, CCObject *>("name", new CCString(szKey)));
      pSequencePropDict->insert(std::pair<std::string, CCObject *>("type", new CCInteger(keyframeType)));
      pSequenceDict->insert(std::pair<std::string, CCObject *>(szKey, pSequencePropDict));
    }
    pSequencesDict->insert(std::pair<std::string, CCObject *>(IntToString(seqId), pSequenceDict));
  }
  if (pSequencesDict->size() > 0) {
    pDict->insert(std::pair<std::string, CCObject *>("animatedProperties", pSequencesDict));
  }
  // Read properties
  CCArray * pPropertiesArray = new CCArray();
  CCArray * pCustomPropertiesArray = new CCArray();
  int numRegularProps = ccbi.readIntWithSign(false);
  int numExtraProps = ccbi.readIntWithSign(false);
  int numProps = numRegularProps + numExtraProps;
  for (int i = 0; i < numProps; i++)
  {
    CCDictionary * pPropsDict = new CCDictionary();
    bool isExtraProp = (i >= numRegularProps);

    // Read type and property name
    int type = ccbi.readIntWithSign(false);
    std::string name = ccbi.readCachedString();
    pPropsDict->insert(std::pair<std::string, CCObject *>("name", new CCString(name)));

    // Check if the property can be set for this platform
    bool setProp = false;
    int platform = ccbi.readByte();
    if (platform == kCCBPlatformAll) setProp = true;
    if (platform == kCCBPlatformIOS) setProp = true;
    if (platform == kCCBPlatformMac) setProp = true;
    if (type == kCCBPropTypePosition)
    {
      if (bAnimatedPosition && name == "position") {
        CCArray * pValueArray = new CCArray();
        pValueArray->push_back(new CCReal(0));
        pValueArray->push_back(new CCReal(0));
        pPropsDict->insert(std::pair<std::string, CCObject *>("baseValue", pValueArray));
      }
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("Position")));
      CCArray * pValueArray = new CCArray();
      float x = ccbi.readFloat();
      pValueArray->push_back(new CCReal(x));
      float y = ccbi.readFloat();
      pValueArray->push_back(new CCReal(y));
      int type = ccbi.readIntWithSign(false);
      pValueArray->push_back(new CCInteger(type));
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
    }
    else if(type == kCCBPropTypePoint || 
            type == kCCBPropTypePointLock)
    {
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString(type == kCCBPropTypePoint ? "Point" : "PointLock")));
      CCArray * pValueArray = new CCArray();
      float x = ccbi.readFloat();
      pValueArray->push_back(new CCReal(x));
      float y = ccbi.readFloat();
      pValueArray->push_back(new CCReal(y));
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
    }
    else if (type == kCCBPropTypeSize)
    {
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("Size")));
      CCArray * pValueArray = new CCArray();
      float w = ccbi.readFloat();
      pValueArray->push_back(new CCReal(w));
      float h = ccbi.readFloat();
      pValueArray->push_back(new CCReal(h));
      int tp = ccbi.readIntWithSign(false);
      pValueArray->push_back(new CCInteger(tp));
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
    }
    else if (type == kCCBPropTypeScaleLock)
    {
      if (bAnimatedScale && name == "scale") {
        CCArray * pValueArray = new CCArray();
        pValueArray->push_back(new CCReal(1));
        pValueArray->push_back(new CCReal(1));
        pPropsDict->insert(std::pair<std::string, CCObject *>("baseValue", pValueArray));
      }
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("ScaleLock")));
      CCArray * pValueArray = new CCArray();
      float x = ccbi.readFloat();
      pValueArray->push_back(new CCReal(x));
      float y = ccbi.readFloat();
      pValueArray->push_back(new CCReal(y));
      bool b = false;
      pValueArray->push_back(new CCBool(b));
      int tp = ccbi.readIntWithSign(false);
      pValueArray->push_back(new CCInteger(tp));
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
    }
    else if (type == kCCBPropTypeDegrees || type == kCCBPropTypeFloat)
    {
      float f = ccbi.readFloat();
      if (isExtraProp) {
        pPropsDict->insert(std::pair<std::string, CCObject *>("optimized", new CCBool(false)));
        pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCInteger(kCCBCustomPropertyTypeFloat)));
        pPropsDict->insert(std::pair<std::string, CCObject *>("value", new CCString(FloatToString(f))));
      } else {
        if (bAnimatedRotation && name == "rotation") {
          pPropsDict->insert(std::pair<std::string, CCObject *>("baseValue", new CCReal(0)));
        }
        pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString(type == kCCBPropTypeDegrees ? "Degrees" : "Float")));
        pPropsDict->insert(std::pair<std::string, CCObject *>("value", new CCReal(f)));
      }
    }
    else if (type == kCCBPropTypeCheck)
    {
      bool b = ccbi.readBool();
      if (isExtraProp) {
        pPropsDict->insert(std::pair<std::string, CCObject *>("optimized", new CCBool(false)));
        pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCInteger(kCCBCustomPropertyTypeBool)));
        pPropsDict->insert(std::pair<std::string, CCObject *>("value", new CCString(IntToString(b ? 1 : 0))));
      } else {
        if (name == "touchEnabled" || name == "isTouchEnabled" || name == "accelerometerEnabled" || name == "isAccelerometerEnabled") {
          pPropsDict->insert(std::pair<std::string, CCObject *>("platform", new CCString("iOS")));
        } else if (name == "mouseEnabled" || name == "isMouseEnabled" || name == "keyboardEnabled" || name == "isKeyboardEnabled") {
          pPropsDict->insert(std::pair<std::string, CCObject *>("platform", new CCString("Mac")));
        } else if (bAnimatedVisible && name == "visible") {
          pPropsDict->insert(std::pair<std::string, CCObject *>("baseValue", new CCBool(false)));
          if (bAnimatedVisibleKeyframeCount > 0) {
            b = bAnimatedVisibleKeyframeCount % 2 != 0;
          }
        }
        pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("Check")));
        pPropsDict->insert(std::pair<std::string, CCObject *>("value", new CCBool(b)));
      }
    }
    else if (type == kCCBPropTypeSpriteFrame)
    {
      if (bAnimatedDisplayFrame && name == "displayFrame") {
        CCArray * pValueArray = new CCArray();
        pValueArray->push_back(new CCString(""));
        pValueArray->push_back(new CCString(""));
        pPropsDict->insert(std::pair<std::string, CCObject *>("baseValue", pValueArray));
      }
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("SpriteFrame")));
      CCArray * pValueArray = new CCArray();
      std::string spriteSheet = ccbi.readCachedString();
      pValueArray->push_back(new CCString(spriteSheet));
      std::string spriteFile = ccbi.readCachedString();
      pValueArray->push_back(new CCString(spriteFile));
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
    }
    else if (type == kCCBPropTypeTexture)
    {
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("Texture")));
      std::string spriteFile = ccbi.readCachedString();
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", new CCString(spriteFile)));
    }
    else if (type == kCCBPropTypeByte)
    {
      if (bAnimatedOpacity && name == "opacity") {
        pPropsDict->insert(std::pair<std::string, CCObject *>("baseValue", new CCInteger(255)));
      }
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("Byte")));
      int byte = ccbi.readByte();
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", new CCInteger(byte)));
    }
    else if (type == kCCBPropTypeColor3)
    {
      if (bAnimatedColor && name == "color") {
        CCArray * pValueArray = new CCArray();
        pValueArray->push_back(new CCInteger(255));
        pValueArray->push_back(new CCInteger(255));
        pValueArray->push_back(new CCInteger(255));
        pPropsDict->insert(std::pair<std::string, CCObject *>("baseValue", pValueArray));
      }
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("Color3")));
      CCArray * pValueArray = new CCArray();
      unsigned char r = ccbi.readByte();
      pValueArray->push_back(new CCInteger(r));
      unsigned char g = ccbi.readByte();
      pValueArray->push_back(new CCInteger(g));
      unsigned char b = ccbi.readByte();
      pValueArray->push_back(new CCInteger(b));
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
    }
    else if (type == kCCBPropTypeFlip)
    {
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("Flip")));
      CCArray * pValueArray = new CCArray();
      bool xFlip = ccbi.readBool();
      pValueArray->push_back(new CCBool(xFlip));
      bool yFlip = ccbi.readBool();
      pValueArray->push_back(new CCBool(yFlip));
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
    }
    else if (type == kCCBPropTypeBlendmode)
    {
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("Blendmode")));
      CCArray * pValueArray = new CCArray();
      int src = ccbi.readIntWithSign(false);
      pValueArray->push_back(new CCInteger(src));
      int dst = ccbi.readIntWithSign(false);
      pValueArray->push_back(new CCInteger(dst));
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
    }
    else if (type == kCCBPropTypeCCBFile)
    {
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("CCBFile")));
      std::string ccbFileName = ccbi.readCachedString();
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", new CCString(ccbFileName)));
    }
    else if (type == kCCBPropTypeInteger || type == kCCBPropTypeIntegerLabeled)
    {
      int d = ccbi.readIntWithSign(true);
      if (type == kCCBPropTypeInteger) {
        if (isExtraProp) {
          pPropsDict->insert(std::pair<std::string, CCObject *>("optimized", new CCBool(false)));
          pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCInteger(kCCBCustomPropertyTypeInt)));
          pPropsDict->insert(std::pair<std::string, CCObject *>("value", new CCString(IntToString(d))));
        } else {
          pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("Integer")));
          pPropsDict->insert(std::pair<std::string, CCObject *>("value", new CCInteger(d)));
        }
      } else {
        pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("IntegerLabeled")));
        pPropsDict->insert(std::pair<std::string, CCObject *>("value", new CCInteger(d)));
      }
    }
    else if (type == kCCBPropTypeFloatVar)
    {
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("FloatVar")));
      CCArray * pValueArray = new CCArray();
      float f = ccbi.readFloat();
      pValueArray->push_back(new CCReal(f));
      float fVar = ccbi.readFloat();
      pValueArray->push_back(new CCReal(fVar));
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
    }
    else if (type == kCCBPropTypeColor4FVar)
    {
      float r = ccbi.readFloat();
      float g = ccbi. readFloat();
      float b = ccbi.readFloat();
      float a = ccbi.readFloat();
      float rVar = ccbi.readFloat();
      float gVar = ccbi.readFloat();
      float bVar = ccbi.readFloat();
      float aVar = ccbi.readFloat();
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("Color4FVar")));
      CCArray * pValueArray = new CCArray();
      CCArray * pValueArray1 = new CCArray();
      pValueArray1->push_back(new CCReal(r));
      pValueArray1->push_back(new CCReal(g));
      pValueArray1->push_back(new CCReal(b));
      pValueArray1->push_back(new CCReal(a));
      pValueArray->push_back(pValueArray1);
      CCArray * pValueArray2 = new CCArray();
      pValueArray2->push_back(new CCReal(rVar));
      pValueArray2->push_back(new CCReal(gVar));
      pValueArray2->push_back(new CCReal(bVar));
      pValueArray2->push_back(new CCReal(aVar));
      pValueArray->push_back(pValueArray2);
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
    }
    else if (type == kCCBPropTypeFloatXY)
    {
      if (bAnimatedSkew && name == "skew") {
        CCArray * pValueArray = new CCArray();
        pValueArray->push_back(new CCReal(0));
        pValueArray->push_back(new CCReal(0));
        pPropsDict->insert(std::pair<std::string, CCObject *>("baseValue", pValueArray));
      }
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("FloatXY")));
      CCArray * pValueArray = new CCArray();
      float x = ccbi.readFloat();
      pValueArray->push_back(new CCReal(x));
      float y = ccbi.readFloat();
      pValueArray->push_back(new CCReal(y));
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
    }
    else if (type == kCCBPropTypeFontTTF)
    {
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("FontTTF")));
      std::string fontTTF = ccbi.readCachedString();
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", new CCString(fontTTF)));
    }
    else if (type == kCCBPropTypeFloatScale)
    {
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("FloatScale")));
      CCArray * pValueArray = new CCArray();
      float f = ccbi.readFloat();
      pValueArray->push_back(new CCReal(f));
      int type = ccbi.readIntWithSign(false);
      pValueArray->push_back(new CCInteger(type));
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
    }
    else if (type == kCCBPropTypeText)
    {
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("Text")));
      std::string text = ccbi.readCachedString();
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", new CCString(text)));
    }
    else if (type == kCCBPropTypeString)
    {
      std::string text = ccbi.readCachedString();
      if (isExtraProp) {
        pPropsDict->insert(std::pair<std::string, CCObject *>("optimized", new CCBool(false)));
        pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCInteger(kCCBCustomPropertyTypeString)));
      } else {
        pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("String")));
      }
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", new CCString(text)));
    }
    else if (type == kCCBPropTypeFntFile)
    {
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("FntFile")));
      std::string fntFile = ccbi.readCachedString();
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", new CCString(fntFile)));
    }
    else if (type == kCCBPropTypeBlock)
    {
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("Block")));
      CCArray * pValueArray = new CCArray();
      std::string selectorName = ccbi.readCachedString();
      pValueArray->push_back(new CCString(selectorName));
      int selectorTarget = ccbi.readIntWithSign(false);
      pValueArray->push_back(new CCInteger(selectorTarget));
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
    }
    else if (type == kCCBPropTypeBlockCCControl)
    {
      pPropsDict->insert(std::pair<std::string, CCObject *>("type", new CCString("BlockCCControl")));
      CCArray * pValueArray = new CCArray();
      std::string selectorName = ccbi.readCachedString();
      pValueArray->push_back(new CCString(selectorName));
      int selectorTarget = ccbi.readIntWithSign(false);
      pValueArray->push_back(new CCInteger(selectorTarget));
      int controlEvents = ccbi.readIntWithSign(false);
      pValueArray->push_back(new CCInteger(controlEvents));
      pPropsDict->insert(std::pair<std::string, CCObject *>("value", pValueArray));
    }
    else
    {
      assert(false);
    }
    if (isExtraProp) {
      pCustomPropertiesArray->push_back(pPropsDict);
    } else {
      pPropertiesArray->push_back(pPropsDict);
    }
  }
  if (pCustomPropertiesArray->size() > 0) {
    pDict->insert(std::pair<std::string, CCObject *>("customProperties", pCustomPropertiesArray));
  }
  pDict->insert(std::pair<std::string, CCObject *>("properties", pPropertiesArray));
  bool bContainCCMenuItemImageChild = false;
  // Read and add children
  CCArray * pChildrenArray = new CCArray();
  int numChildren = ccbi.readIntWithSign(false);
  for (int i = 0; i < numChildren; i++)
  {
    CCDictionary * pChildDict = new CCDictionary();
    readNodeRecurse(ccbi, pChildDict);
    CCDictionary::iterator it = pChildDict->find("baseClass");
    if (it != pChildDict->end()) {
      if (*((CCString *)it->second) == L"CCMenuItemImage") {
        bContainCCMenuItemImageChild = true;
      }
    }
    pChildrenArray->push_back(pChildDict);
  }
  pDict->insert(std::pair<std::string, CCObject *>("children", pChildrenArray));
  // 最终决定基类和自定义类
  if (!bIsCustomClassIsStandardClass)
  {
    baseClass = decideStandardClass(pPropertiesArray);
    // 尽可能区分"CCLayer"还是"CCMenu"
    if (baseClass == "CCLayer" && bContainCCMenuItemImageChild) {
      baseClass = "CCMenu";
    }
  }
  pDict->insert(std::pair<std::string, CCObject *>("baseClass", new CCString(baseClass)));
  pDict->insert(std::pair<std::string, CCObject *>("displayName", new CCString(displayName)));
  pDict->insert(std::pair<std::string, CCObject *>("customClass", new CCString(customClass)));
  return true;
}

#define kCCBVersion 5
bool readccbi(ccbiReader & ccbi, CCDictionary * pDict, CCDictionary * pTemplateDict)
{
  // 读取头信息
  char magicString[4];
  ccbi.readBytes(magicString, 4, 1);
  if( strncmp(magicString, "ibcc", 4) != 0 )
    return false;
  // 读取版本信息
  int iVersion = ccbi.readIntWithSign(false);
  if (iVersion != kCCBVersion)
    return false;
  // JS Controlled
  m_bJSControlled = ccbi.readBool();
  pDict->insert(std::pair<std::string, CCObject *>("jsControlled", new CCBool(m_bJSControlled)));
  // 读取字符串缓存
  ccbi.readStringCache();
  // 读取动画序列
  CCArray * pSequenceArray = new CCArray();
  int iSequenceInfosCount = ccbi.readIntWithSign(false);
  for (int i = 0; i < iSequenceInfosCount; i++)
  {
    CCDictionary * pSequenceDict = new CCDictionary();
    float length = ccbi.readFloat();
    pSequenceDict->insert(std::pair<std::string, CCObject *>("length", new CCReal(length)));
    std::string name = ccbi.readCachedString();
    pSequenceDict->insert(std::pair<std::string, CCObject *>("name", new CCString(name)));
    int sequenceId = ccbi.readIntWithSign(false);
    pSequenceDict->insert(std::pair<std::string, CCObject *>("sequenceId", new CCInteger(sequenceId)));
    int chainedSequenceId = ccbi.readIntWithSign(true);
    pSequenceDict->insert(std::pair<std::string, CCObject *>("chainedSequenceId", new CCInteger(chainedSequenceId)));
    bool autoPlay = true;
    pSequenceDict->insert(std::pair<std::string, CCObject *>("autoPlay", new CCBool(autoPlay)));
    float offset = 0.0f;
    pSequenceDict->insert(std::pair<std::string, CCObject *>("offset", new CCReal(offset)));
    float position = 0.0f;
    pSequenceDict->insert(std::pair<std::string, CCObject *>("position", new CCReal(position)));
    float resolution = 30.0f;
    pSequenceDict->insert(std::pair<std::string, CCObject *>("resolution", new CCReal(resolution)));
    float scale = 128.0f;
    pSequenceDict->insert(std::pair<std::string, CCObject *>("scale", new CCReal(scale)));

    // read callback key frames for sequence
    int numCallbackKeyframes = ccbi.readIntWithSign(false);
    CCDictionary * pCallbackChannelDict = new CCDictionary();
    CCArray * pCallbackKeyFrameArray = new CCArray();
    pCallbackChannelDict->insert(std::pair<std::string, CCObject *>("keyframes", pCallbackKeyFrameArray));
    pCallbackChannelDict->insert(std::pair<std::string, CCObject *>("type", new CCInteger(10)));
    pSequenceDict->insert(std::pair<std::string, CCObject *>("callbackChannel", pCallbackChannelDict));

    // read sound key frames for sequence
    int numSoundKeyframes = ccbi.readIntWithSign(false);
    CCDictionary * pSoundChannelDict = new CCDictionary();
    CCArray * pSoundKeyFrameArray = new CCArray();
    for (int j = 0; j < numSoundKeyframes; j++)
    {
      float time = ccbi.readFloat();
      std::string soundFile = ccbi.readCachedString();
      float pitch = ccbi.readFloat();
      float pan = ccbi.readFloat();
      float gain = ccbi.readFloat();
    }
    pSoundChannelDict->insert(std::pair<std::string, CCObject *>("keyframes", pSoundKeyFrameArray));
    pSoundChannelDict->insert(std::pair<std::string, CCObject *>("type", new CCInteger(9)));
    pSequenceDict->insert(std::pair<std::string, CCObject *>("soundChannel", pSoundChannelDict));
    pSequenceArray->push_back(pSequenceDict);
  }
  pDict->insert(std::pair<std::string, CCObject *>("sequences", pSequenceArray));

  if (pTemplateDict != NULL) {
    pDict->insert(pTemplateDict->begin(), pTemplateDict->end());
  } else {
    pDict->insert(std::pair<std::string, CCObject *>("centeredOrigin", new CCBool(false)));
    pDict->insert(std::pair<std::string, CCObject *>("currentResolution", new CCInteger(0)));
    pDict->insert(std::pair<std::string, CCObject *>("currentSequenceId", new CCInteger(0)));
    pDict->insert(std::pair<std::string, CCObject *>("fileType", new CCString(L"CocosBuilder")));
    pDict->insert(std::pair<std::string, CCObject *>("fileVersion", new CCInteger(4)));
    pDict->insert(std::pair<std::string, CCObject *>("guides", new CCArray()));
    CCArray * pNoteArray = new CCArray();
    pDict->insert(std::pair<std::string, CCObject *>("notes", pNoteArray));
    CCArray * pResolutionsArray = new CCArray();
    {
      CCDictionary * pResolutionDict = new CCDictionary();
      pResolutionDict->insert(std::pair<std::string, CCObject *>("centeredOrigin", new CCBool(false)));
      pResolutionDict->insert(std::pair<std::string, CCObject *>("ext", new CCString("large")));
      pResolutionDict->insert(std::pair<std::string, CCObject *>("width", new CCInteger(1280)));
      pResolutionDict->insert(std::pair<std::string, CCObject *>("height", new CCInteger(800)));
      pResolutionDict->insert(std::pair<std::string, CCObject *>("name", new CCString("Android Large Landscape")));
      pResolutionDict->insert(std::pair<std::string, CCObject *>("scale", new CCReal(2)));
      pResolutionsArray->push_back(pResolutionDict);
    }
    pDict->insert(std::pair<std::string, CCObject *>("resolutions", pResolutionsArray));
    int stageBorder = 3;
    pDict->insert(std::pair<std::string, CCObject *>("stageBorder", new CCInteger(stageBorder)));
  }

  int autoPlaySequenceId = ccbi.readIntWithSign(true);

  CCDictionary * pNodeGraphDict = new CCDictionary();
  readNodeRecurse(ccbi, pNodeGraphDict);
  pDict->insert(std::pair<std::string, CCObject *>("nodeGraph", pNodeGraphDict));
  return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
  std::string ccbiFile = "test.ccbi";
  if (argc > 1) {
    ccbiFile = ConvertToAString(argv[1]);
  }
  CCDictionary * pTemplateDict = (CCDictionary *)LoadPlist("ccb.template");
  buildStandardClassProperties();
  CCDictionary * pDict = new CCDictionary();
  unsigned long size = 0;
  void * pData = LoadResourceFromFile(ccbiFile.c_str(), &size);
  ccbiReader ccbi(pData, size);
  readccbi(ccbi, pDict, pTemplateDict);
  ccbiFile[ccbiFile.length() - 1] = 0;
  SavePlist(ccbiFile.c_str(), pDict);
	return 0;
}
