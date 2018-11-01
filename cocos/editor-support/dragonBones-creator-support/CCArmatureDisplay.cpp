#include "dragonBones-creator-support/CCArmatureDisplay.h"
#include "dragonBones-creator-support/CCSlot.h"
#include "IOBuffer.h"

using namespace editor;

USING_NS_CC;

DRAGONBONES_NAMESPACE_BEGIN

CCArmatureDisplay* CCArmatureDisplay::create()
{
    CCArmatureDisplay* displayContainer = new (std::nothrow) CCArmatureDisplay();
    if (displayContainer)
    {
        displayContainer->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(displayContainer);
    }
    return displayContainer;
}

CCArmatureDisplay::CCArmatureDisplay()
:_vertexBuffer(se::Object::TypedArrayType::UINT32)
,_indiceBuffer(se::Object::TypedArrayType::UINT16)
,_debugBuffer(se::Object::TypedArrayType::FLOAT32)
{
    
}

CCArmatureDisplay::~CCArmatureDisplay()
{
    dispose();
    for (auto it : _listenerIDMap)
    {
        std::vector<uint32_t>& idArr = *(it.second);
        for (auto i : idArr)
        {
            EventDispatcher::removeCustomEventListener(it.first, i);
        }
        delete &idArr;
    }
}

void CCArmatureDisplay::dbInit(Armature* armature)
{
    _armature = armature;
}

void CCArmatureDisplay::dbClear()
{
    _armature = nullptr;
    release();
}

void CCArmatureDisplay::dispose(bool disposeProxy)
{
    if (_armature != nullptr) 
    {
        _armature->dispose();
        _armature = nullptr;
    }
}

void CCArmatureDisplay::dbUpdate()
{
    _vertexBuffer.reset();
    _debugBuffer.reset();
    _indiceBuffer.reset();
    
    _preBlendSrc = -1;
    _preBlendDst = -1;
    _preTextureIndex = -1;
    _curBlendSrc = -1;
    _curBlendDst = -1;
    _curTextureIndex = -1;
    
    _preVSegWritePos = -1;
    _preISegWritePos = -1;
    _curVSegLen = 0;
    _curISegLen = 0;
    
    _debugSlotsLen = 0;
    _materialLen = 0;
    
    //reserved 4 bytes to save material len
    _vertexBuffer.writeUint32(0);
    
    traverseArmature(_armature);
    
    _vertexBuffer.writeUint32(0, _materialLen);
    if (_preVSegWritePos != -1)
    {
        _vertexBuffer.writeUint32(_preVSegWritePos, _curVSegLen);
        _vertexBuffer.writeUint32(_preISegWritePos, _curISegLen);
    }
    
    if (_debugDraw)
    {
        auto& bones = _armature->getBones();
        std::size_t count = bones.size();
        _debugBuffer.writeFloat32(count*4 + 0.1);
        for (int i = 0; i < count; i++)
        {
            Bone* bone = (Bone*)bones[i];
            float boneLen = 5;
            if (bone->_boneData->length > boneLen)
            {
                boneLen = bone->_boneData->length;
            }
            
            float bx = bone->globalTransformMatrix.tx;
            float by = bone->globalTransformMatrix.ty;
            float endx = bx + bone->globalTransformMatrix.a * boneLen;
            float endy = by + bone->globalTransformMatrix.b * boneLen;
            
            _debugBuffer.writeFloat32(bx);
            _debugBuffer.writeFloat32(by);
            _debugBuffer.writeFloat32(endx);
            _debugBuffer.writeFloat32(endy);
        }
    }
    
    // update js array buffer
    if (_vertexBuffer.isNewBuffer || _indiceBuffer.isNewBuffer || _debugBuffer.isNewBuffer)
    {
        if (_changeBufferCallback)
        {
            _changeBufferCallback(_vertexBuffer.getTypeArray(), _indiceBuffer.getTypeArray(), _debugBuffer.getTypeArray());
        }
        _vertexBuffer.isNewBuffer = false;
        _indiceBuffer.isNewBuffer = false;
        _debugBuffer.isNewBuffer = false;
    }
}

void CCArmatureDisplay::traverseArmature(Armature* armature)
{
    auto& slots = _armature->getSlots();
    for (std::size_t i = 0, len = slots.size(); i < len; i++)
    {
        CCSlot* slot = (CCSlot*)slots[i];
        if (!slot->getVisible())
        {
            continue;
        }
        
        slot->updateWorldMatrix();
        
        Armature* childArmature = slot->getChildArmature();
        if (childArmature != nullptr)
        {
            traverseArmature(childArmature);
            continue;
        }
        
        switch (slot->_blendMode)
        {
            case BlendMode::Add:
                _curBlendSrc = _premultipliedAlpha ? GL_ONE : GL_SRC_ALPHA;
                _curBlendDst = GL_ONE;
                break;
            case BlendMode::Multiply:
                _curBlendSrc = GL_DST_COLOR;
                _curBlendDst = GL_ONE_MINUS_SRC_ALPHA;
                break;
            case BlendMode::Screen:
                _curBlendSrc = GL_ONE;
                _curBlendDst = GL_ONE_MINUS_SRC_COLOR;
                break;
            default:
                _curBlendSrc = _premultipliedAlpha ? GL_ONE : GL_SRC_ALPHA;
                _curBlendDst = GL_ONE_MINUS_SRC_ALPHA;
                break;
        }
        
        editor::Texture2D* texture = slot->getTexture();
        if (!texture) continue;
        _curTextureIndex = texture->getRealTextureIndex();
        if (_preTextureIndex != _curTextureIndex || _preBlendDst != _curBlendDst || _preBlendSrc != _curBlendSrc)
        {
            if (_preVSegWritePos != -1)
            {
                _vertexBuffer.writeUint32(_preVSegWritePos,_curVSegLen);
                _vertexBuffer.writeUint32(_preISegWritePos,_curISegLen);
            }
            
            _vertexBuffer.writeUint32(_curTextureIndex);
            _vertexBuffer.writeUint32(_curBlendSrc);
            _vertexBuffer.writeUint32(_curBlendDst);
            
            //reserve vertex segamentation lenght
            _preVSegWritePos = _vertexBuffer.getCurPos();
            _vertexBuffer.writeUint32(0);
            //reserve indice segamentation lenght
            _preISegWritePos = _vertexBuffer.getCurPos();
            _vertexBuffer.writeUint32(0);
            
            _preTextureIndex = _curTextureIndex;
            _preBlendDst = _curBlendDst;
            _preBlendSrc = _curBlendSrc;
            
            _curVSegLen = 0;
            _curISegLen = 0;
            
            _materialLen++;
        }
        
        _finalColor.a *= _nodeColor.a * slot->color.a * 255;
        float multiplier = _premultipliedAlpha ? slot->color.a : 255;
        _finalColor.r *= _nodeColor.r * slot->color.r * multiplier;
        _finalColor.g *= _nodeColor.g * slot->color.g * multiplier;
        _finalColor.b *= _nodeColor.b * slot->color.b * multiplier;
        
        editor::Triangles& triangles = slot->triangles;
        cocos2d::Mat4& worldMatrix = slot->worldMatrix;
        for (int v = 0, w = 0, vn = triangles.vertCount; v < vn; ++v, w += 2)
        {
            editor::V2F_T2F_C4B* vertex = triangles.verts + v;
            vertex->vertices.x = vertex->vertices.x * worldMatrix.m[0] + vertex->vertices.y * worldMatrix.m[4] + worldMatrix.m[12];
            vertex->vertices.y = vertex->vertices.x * worldMatrix.m[1] + vertex->vertices.y * worldMatrix.m[11] + worldMatrix.m[13];
            vertex->colors.r = (GLubyte)_finalColor.r;
            vertex->colors.g = (GLubyte)_finalColor.g;
            vertex->colors.b = (GLubyte)_finalColor.b;
            vertex->colors.a = (GLubyte)_finalColor.a;
        }
        
        _vertexBuffer.writeBytes((char*)triangles.verts,triangles.vertCount*sizeof(editor::V2F_T2F_C4B));
        
        if (_curVSegLen > 0)
        {
            for (int ii = 0, nn = triangles.indexCount; ii < nn; ii++)
            {
                _indiceBuffer.writeUint16(triangles.indices[ii] + _curVSegLen);
            }
        }
        else
        {
            _indiceBuffer.writeBytes((char*)triangles.indices,triangles.indexCount*sizeof(unsigned short));
        }
        
        _curVSegLen += triangles.vertCount;
        _curISegLen += triangles.indexCount;
    }
}

bool CCArmatureDisplay::hasDBEventListener(const std::string& type) const
{
    auto it = _listenerIDMap.find(type);
    return it != _listenerIDMap.end();
}

void CCArmatureDisplay::addDBEventListener(const std::string& type, const std::function<void(EventObject*)>& callback)
{
    auto lambda = [callback](const CustomEvent& event) -> void
    {
        callback(static_cast<EventObject*>(event.args[0].ptrVal));
    };
    
    uint32_t listenerID = EventDispatcher::addCustomEventListener(type, lambda);
    auto it = _listenerIDMap.find(type);
    if (it != _listenerIDMap.end())
    {
        auto idArr = it->second;
        idArr->push_back(listenerID);
    }
    else
    {
        auto idArr = new std::vector<uint32_t>;
        idArr->push_back(listenerID);
        _listenerIDMap[type] = idArr;
    }
}

void CCArmatureDisplay::dispatchDBEvent(const std::string& type, EventObject* value)
{
    CustomEvent customEvent;
    customEvent.name = type;
    customEvent.args[0].ptrVal = (void*)value;
    EventDispatcher::dispatchCustomEvent(customEvent);
}

void CCArmatureDisplay::removeDBEventListener(const std::string& type, const std::function<void(EventObject*)>& callback)
{
    auto it = _listenerIDMap.find(type);
    if (it == _listenerIDMap.end())
    {
        return;
    }
    
    std::vector<uint32_t>& idArr = *(it->second);
    for (auto i : idArr)
    {
        EventDispatcher::removeCustomEventListener(type, i);
    }
    delete &idArr;
    _listenerIDMap.erase(it);
}

DRAGONBONES_NAMESPACE_END
