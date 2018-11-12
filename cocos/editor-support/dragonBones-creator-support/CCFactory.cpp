#include "dragonBones-creator-support/CCFactory.h"
#include "dragonBones-creator-support/CCTextureAtlasData.h"
#include "dragonBones-creator-support/CCArmatureDisplay.h"
#include "dragonBones-creator-support/CCSlot.h"
#include "platform/CCFileUtils.h"

DRAGONBONES_NAMESPACE_BEGIN

DragonBones* CCFactory::_dragonBonesInstance = nullptr;
CCFactory* CCFactory::_factory = nullptr;

TextureAtlasData* CCFactory::_buildTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const
{
    if (textureAtlasData != nullptr)
    {
        const auto pos = _prevPath.find_last_of("/");
        if (pos != std::string::npos)
        {
            const auto basePath = _prevPath.substr(0, pos + 1);
            textureAtlasData->imagePath = basePath + textureAtlasData->imagePath;
        }

        if (textureAtlas != nullptr)
        {
            static_cast<CCTextureAtlasData*>(textureAtlasData)->setRenderTexture(static_cast<editor::Texture2D*>(textureAtlas));
        }
    }
    else
    {
        textureAtlasData = BaseObject::borrowObject<CCTextureAtlasData>();
    }

    return textureAtlasData;
}

Armature* CCFactory::_buildArmature(const BuildArmaturePackage& dataPackage) const
{
    const auto armature = BaseObject::borrowObject<Armature>();
    const auto armatureDisplay = CCArmatureDisplay::create();

    // will release when armature destructor
    armatureDisplay->retain();

    armature->init(
        dataPackage.armature,
        armatureDisplay, armatureDisplay, _dragonBones
    );

    return armature;
}

Slot* CCFactory::_buildSlot(const BuildArmaturePackage& dataPackage, const SlotData* slotData, Armature* armature) const
{
    const auto slot = BaseObject::borrowObject<CCSlot>();

    slot->init(
        slotData, armature,
        slot, slot
    );

    return slot;
}

DragonBonesData* CCFactory::loadDragonBonesData(const std::string& filePath, const std::string& name, float scale)
{
    if (!name.empty())
    {
        const auto existedData = getDragonBonesData(name);
        if (existedData)
        {
            return existedData;
        }
    }

    const auto fullpath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filePath);
    if (cocos2d::FileUtils::getInstance()->isFileExist(filePath)) 
    {
        const auto pos = fullpath.find(".json");

        if (pos != std::string::npos)
        {
            const auto data = cocos2d::FileUtils::getInstance()->getStringFromFile(filePath);

            return parseDragonBonesData(data.c_str(), name, scale);
        }
        else
        {
            cocos2d::Data cocos2dData;
            cocos2d::FileUtils::getInstance()->getContents(fullpath, &cocos2dData);
            const auto binary = (unsigned char*)malloc(sizeof(unsigned char)* cocos2dData.getSize());
            memcpy(binary, cocos2dData.getBytes(), cocos2dData.getSize());
            const auto data = parseDragonBonesData((char*)binary, name, scale);

            return data;
        }
    }

    return nullptr;
}

DragonBonesData* CCFactory::parseDragonBonesDataOnly(const std::string& filePath, const std::string& name, float scale)
{
    if (!name.empty())
    {
        const auto existedData = getDragonBonesData(name);
        if (existedData)
        {
            return existedData;
        }
    }
    
    const auto fullpath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filePath);
    char* rawData = nullptr;
    
    if (cocos2d::FileUtils::getInstance()->isFileExist(filePath))
    {
        const auto pos = fullpath.find(".json");
        
        if (pos != std::string::npos)
        {
            const auto data = cocos2d::FileUtils::getInstance()->getStringFromFile(filePath);
            rawData = (char*)data.c_str();
        }
        else
        {
            cocos2d::Data cocos2dData;
            cocos2d::FileUtils::getInstance()->getContents(fullpath, &cocos2dData);
            rawData = (char*)malloc(sizeof(char)* cocos2dData.getSize());
            memcpy(rawData, cocos2dData.getBytes(), cocos2dData.getSize());
        }
    }
    
    DataParser* dataParser = nullptr;
    
    if (
        rawData[0] == 'D' &&
        rawData[1] == 'B' &&
        rawData[2] == 'D' &&
        rawData[3] == 'T'
        )
    {
        dataParser = &_binaryParser;
    }
    else
    {
        dataParser = _dataParser;
    }
    
    return dataParser->parseDragonBonesData(rawData, scale);
}

void CCFactory::handleTextureAtlasData(bool isBinary, const std::string& name, float scale)
{
    DataParser* dataParser = nullptr;
    
    if (isBinary)
    {
        dataParser = &_binaryParser;
    }
    else
    {
        dataParser = _dataParser;
    }
    
    while (true)
    {
        const auto textureAtlasData = _buildTextureAtlasData(nullptr, nullptr);
        if (dataParser->parseTextureAtlasData(nullptr, *textureAtlasData, scale))
        {
            addTextureAtlasData(textureAtlasData, name);
        }
        else
        {
            textureAtlasData->returnToPool();
            break;
        }
    }
}

TextureAtlasData* CCFactory::loadTextureAtlasData(const std::string& filePath, const std::string& name, float scale)
{
    _prevPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filePath);
    const auto data = cocos2d::FileUtils::getInstance()->getStringFromFile(_prevPath);
    if (data.empty())
    {
        return nullptr;
    }

    return static_cast<CCTextureAtlasData*>(BaseFactory::parseTextureAtlasData(data.c_str(), nullptr, name, scale));
}

CCArmatureDisplay* CCFactory::buildArmatureDisplay(const std::string& armatureName, const std::string& dragonBonesName, const std::string& skinName, const std::string& textureAtlasName) const
{
    const auto armature = buildArmature(armatureName, dragonBonesName, skinName, textureAtlasName);
    if (armature != nullptr)
    {
        _dragonBones->getClock()->add(armature);

        return static_cast<CCArmatureDisplay*>(armature->getDisplay());
    }

    return nullptr;
}

void CCFactory::removeTextureAtlasDataByIndex(const std::string& name, int textureIndex)
{
    const auto iterator = _textureAtlasDataMap.find(name);
    if (iterator != _textureAtlasDataMap.end())
    {
        auto& textureAtlasDataList = iterator->second;
        for (auto it = textureAtlasDataList.begin(); it != textureAtlasDataList.end(); it++)
        {
            editor::Texture2D* texture = ((CCTextureAtlasData*)*it)->getRenderTexture();
            if (texture && texture->getRealTextureIndex() == textureIndex)
            {
                textureAtlasDataList.erase(it);
                break;
            }
        }
        if (textureAtlasDataList.size() == 0)
        {
            _textureAtlasDataMap.erase(iterator);
        }
    }
}

CCTextureAtlasData* CCFactory::getTextureAtlasDataByIndex(const std::string& name, int textureIndex) const
{
    const auto iterator = _textureAtlasDataMap.find(name);
    if (iterator != _textureAtlasDataMap.end())
    {
        auto& textureAtlasDataList = iterator->second;
        for (auto it = textureAtlasDataList.begin(); it != textureAtlasDataList.end(); it++)
        {
            editor::Texture2D* texture = ((CCTextureAtlasData*)*it)->getRenderTexture();
            if (texture && texture->getRealTextureIndex() == textureIndex)
            {
                return (CCTextureAtlasData*)*it;
            }
        }
    }
    return nullptr;
}

DRAGONBONES_NAMESPACE_END
