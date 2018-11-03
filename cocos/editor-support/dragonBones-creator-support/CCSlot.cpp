#include "dragonBones-creator-support/CCSlot.h"
#include "dragonBones-creator-support/CCTextureAtlasData.h"
#include "dragonBones-creator-support/CCArmatureDisplay.h"

DRAGONBONES_NAMESPACE_BEGIN

void CCSlot::_onClear()
{
    Slot::_onClear();
    disposeTriangles();
}

void CCSlot::disposeTriangles()
{
    if (worldVerts){
        delete[] worldVerts;
        worldVerts = nullptr;
    }
    if (triangles.verts)
    {
        delete[] triangles.verts;
        triangles.verts = nullptr;
    }
    if (triangles.indices)
    {
        delete[] triangles.indices;
        triangles.indices = nullptr;
    }
    triangles.indexCount = 0;
    triangles.vertCount = 0;
}

void CCSlot::adjustTriangles(const unsigned vertexCount, const unsigned indicesCount)
{
    if (triangles.vertCount < vertexCount)
    {
        if (triangles.verts)
        {
            delete[] triangles.verts;
        }
        triangles.verts = new editor::V2F_T2F_C4B[vertexCount];
        
        if (worldVerts)
        {
            delete[] worldVerts;
        }
        worldVerts = new editor::V2F_T2F_C4B[vertexCount];
    }
    triangles.vertCount = vertexCount;
    
    if (triangles.indexCount < indicesCount)
    {
        if (triangles.indices)
        {
            delete[] triangles.indices;
        }
        triangles.indices = new unsigned short[indicesCount];
    }
    triangles.indexCount = indicesCount;
}

void CCSlot::_initDisplay(void* value, bool isRetain)
{
    
}

void CCSlot::_disposeDisplay(void* value, bool isRelease)
{
    
}

void CCSlot::_onUpdateDisplay()
{
    
}

void CCSlot::_addDisplay()
{
    
}

void CCSlot::_replaceDisplay(void* value, bool isArmatureDisplay)
{
    _textureScale = 1.0f;
}

void CCSlot::_removeDisplay()
{
    
}

void CCSlot::_updateZOrder()
{
    
}

editor::Texture2D* CCSlot::getTexture() const
{
    const auto currentTextureData = static_cast<CCTextureData*>(_textureData);
    if (!currentTextureData || !currentTextureData->spriteFrame)
    {
        return nullptr;
    }
    return currentTextureData->spriteFrame->getTexture();
}

//void CCSlot::_updateFrame()
//{
//    if (this->_display && this->_displayIndex >= 0)
//    {
//        const unsigned displayIndex = this->_displayIndex;
//        const auto& rawDisplayDatas = *(this->_rawDisplayDatas);
//        const auto rawDisplayData = displayIndex < rawDisplayDatas.size() ? rawDisplayDatas[displayIndex] : nullptr;
//        const auto displayData = displayIndex <  this->_displayDatas.size() ?  this->_displayDatas[displayIndex] : nullptr;
//        const auto currentDisplayData = displayData ? displayData : rawDisplayData;
//        const auto currentTextureData = static_cast<CCTextureData*>(this->_textureData);
//
//        if (currentTextureData)
//        {
//            auto texture = (editor::Texture2D*)(this->_armature->getReplacedTexture());
//            if (!texture)
//            {
//                texture = static_cast<CCTextureAtlasData*>(currentTextureData->parent)->getRenderTexture();
//            }
//
//
//            if (this->_meshData && this->_display == this->_meshDisplay)
//            {
//                const auto& region = currentTextureData->region;
//                const auto& textureAtlasSize = currentTextureData->texture->getTexture()->getContentSizeInPixels();
//                auto displayVertices = new cocos2d::V3F_C4B_T2F[(unsigned)(this->_meshData->uvs.size() / 2)]; // does cocos2dx release it?
//                auto vertexIndices = new unsigned short[this->_meshData->vertexIndices.size()]; // does cocos2dx release it?
//                cocos2d::Rect boundsRect(999999.f, 999999.f, -999999.f, -999999.f);
//
//                if (this->_meshData != rawDisplayData->mesh && rawDisplayData && rawDisplayData != currentDisplayData)
//                {
//                    this->_pivotX = rawDisplayData->transform.x - currentDisplayData->transform.x;
//                    this->_pivotY = rawDisplayData->transform.y - currentDisplayData->transform.y;
//                }
//                else
//                {
//                    this->_pivotX = 0.f;
//                    this->_pivotY = 0.f;
//                }
//
//                for (std::size_t i = 0, l = this->_meshData->uvs.size(); i < l; i += 2)
//                {
//                    const auto iH = (unsigned)(i / 2);
//                    const auto x = this->_meshData->vertices[i];
//                    const auto y = this->_meshData->vertices[i + 1];
//                    cocos2d::V3F_C4B_T2F vertexData;
//                    vertexData.vertices.set(x, -y, 0.f);
//                    vertexData.texCoords.u = (region.x + this->_meshData->uvs[i] * region.width) / textureAtlasSize.width;
//                    vertexData.texCoords.v = (region.y + this->_meshData->uvs[i + 1] * region.height) / textureAtlasSize.height;
//                    vertexData.colors = cocos2d::Color4B::WHITE;
//                    displayVertices[iH] = vertexData;
//
//                    if (boundsRect.origin.x > x)
//                    {
//                        boundsRect.origin.x = x;
//                    }
//
//                    if (boundsRect.size.width < x)
//                    {
//                        boundsRect.size.width = x;
//                    }
//
//                    if (boundsRect.origin.y > -y)
//                    {
//                        boundsRect.origin.y = -y;
//                    }
//
//                    if (boundsRect.size.height < -y)
//                    {
//                        boundsRect.size.height = -y;
//                    }
//                }
//
//                boundsRect.size.width -= boundsRect.origin.x;
//                boundsRect.size.height -= boundsRect.origin.y;
//
//                for (std::size_t i = 0, l = this->_meshData->vertexIndices.size(); i < l; ++i)
//                {
//                    vertexIndices[i] = this->_meshData->vertexIndices[i];
//                }
//
//                // In cocos2dx render meshDisplay and frameDisplay are the same display
//                if (currentTextureData->texture)
//                {
//                    frameDisplay->setSpriteFrame(currentTextureData->texture); // polygonInfo will be override
//                    if (texture != currentTextureData->texture->getTexture())
//                    {
//                        frameDisplay->setTexture(texture); // Relpace texture // polygonInfo will be override
//                    }
//                }
//
//                //
//                cocos2d::PolygonInfo polygonInfo;
//                auto& triangles = polygonInfo.triangles;
//                triangles.verts = displayVertices;
//                triangles.indices = vertexIndices;
//                triangles.vertCount = (unsigned)(this->_meshData->uvs.size() / 2);
//                triangles.indexCount = (unsigned)(this->_meshData->vertexIndices.size());
//#if COCOS2D_VERSION >= 0x00031400
//                polygonInfo.setRect(boundsRect);
//#else
//                polygonInfo.rect = boundsRect; // Copy
//#endif
//                frameDisplay->setContentSize(boundsRect.size);
//                frameDisplay->setPolygonInfo(polygonInfo);
//                frameDisplay->setColor(frameDisplay->getColor()); // Backup
//
//                if (this->_meshData->skinned)
//                {
//                    frameDisplay->setPosition(0.f, 0.f);
//                    frameDisplay->setRotation(0.f);
//                    frameDisplay->setRotationSkewX(0.f);
//                    frameDisplay->setRotationSkewY(0.f);
//                    frameDisplay->setScale(1.f, 1.f);
//                }
//            }
//            else
//            {
//                const auto scale = this->_armature->getArmatureData().scale;
//                this->_pivotX = currentDisplayData->pivot.x;
//                this->_pivotY = currentDisplayData->pivot.y;
//
//                if (currentDisplayData->isRelativePivot)
//                {
//                    const auto& rectData = currentTextureData->frame ? *currentTextureData->frame : currentTextureData->region;
//                    auto width = rectData.width * scale;
//                    auto height = rectData.height * scale;
//                    if (!currentTextureData->frame && currentTextureData->rotated)
//                    {
//                        width = rectData.height;
//                        height = rectData.width;
//                    }
//
//                    this->_pivotX *= width;
//                    this->_pivotY *= height;
//                }
//
//                if (currentTextureData->frame)
//                {
//                    this->_pivotX += currentTextureData->frame->x * scale;
//                    this->_pivotY += currentTextureData->frame->y * scale;
//                }
//
//                if (rawDisplayData && rawDisplayData != currentDisplayData)
//                {
//                    this->_pivotX += rawDisplayData->transform.x - currentDisplayData->transform.x;
//                    this->_pivotY += rawDisplayData->transform.y - currentDisplayData->transform.y;
//                }
//
//                this->_pivotY -= currentTextureData->region.height * this->_armature->getArmatureData().scale;
//
//                frameDisplay->setSpriteFrame(currentTextureData->texture); // polygonInfo will be override
//
//                if (texture != currentTextureData->texture->getTexture())
//                {
//                    frameDisplay->setTexture(texture); // Relpace texture // polygonInfo will be override
//                }
//
//                this->_blendModeDirty = true; // Relpace texture // blendMode will be override
//            }
//
//            this->_updateVisible();
//
//            return;
//        }
//    }
//
//    this->_pivotX = 0.f;
//    this->_pivotY = 0.f;
//
//    frameDisplay->setTexture(nullptr);
//    frameDisplay->setTextureRect(cocos2d::Rect::ZERO);
//    frameDisplay->setVisible(false);
//    frameDisplay->setPosition(this->origin.x, this->origin.y);
//}

void CCSlot::_updateFrame()
{
    const auto currentVerticesData = (_deformVertices != nullptr && _display == _meshDisplay) ? _deformVertices->verticesData : nullptr;
    const auto currentTextureData = static_cast<CCTextureData*>(_textureData);

    if (_displayIndex >= 0 && _display != nullptr && currentTextureData != nullptr)
    {
        if (currentTextureData->spriteFrame != nullptr)
        {
            const auto& region = currentTextureData->region;
            const auto texture = currentTextureData->spriteFrame->getTexture();
            const auto textureWidth = texture->getPixelsWide();
            const auto textureHeight = texture->getPixelsHigh();
            
            if (currentVerticesData != nullptr) // Mesh.
            {
                const auto data = currentVerticesData->data;
                const auto intArray = data->intArray;
                const auto floatArray = data->floatArray;
                const unsigned vertexCount = intArray[currentVerticesData->offset + (unsigned)BinaryOffset::MeshVertexCount];
                const unsigned triangleCount = intArray[currentVerticesData->offset + (unsigned)BinaryOffset::MeshTriangleCount];
                int vertexOffset = intArray[currentVerticesData->offset + (unsigned)BinaryOffset::MeshFloatOffset];

                if (vertexOffset < 0)
                {
                    vertexOffset += 65536; // Fixed out of bouds bug.
                }

                const unsigned uvOffset = vertexOffset + vertexCount * 2;
                const unsigned indicesCount = triangleCount * 3;
                adjustTriangles(vertexCount, indicesCount);
                auto vertices = triangles.verts;
                auto vertexIndices = triangles.indices;
                
                boundsRect.origin.x = 999999.0f;
                boundsRect.origin.y = 999999.0f;
                boundsRect.size.width = -999999.0f;
                boundsRect.size.height = -999999.0f;

                for (std::size_t i = 0, l = vertexCount * 2; i < l; i += 2)
                {
                    const auto iH = i / 2;
                    const auto x = floatArray[vertexOffset + i];
                    const auto y = floatArray[vertexOffset + i + 1];
                    auto u = floatArray[uvOffset + i];
                    auto v = floatArray[uvOffset + i + 1];
                    editor::V2F_T2F_C4B& vertexData = vertices[iH];
                    vertexData.vertices.x = x;
                    vertexData.vertices.y = -y;

                    if (currentTextureData->rotated)
                    {
                        vertexData.texCoords.u = (region.x + (1.0f - v) * region.width) / textureWidth;
                        vertexData.texCoords.v = (region.y + u * region.height) / textureHeight;
                    }
                    else
                    {
                        vertexData.texCoords.u = (region.x + u * region.width) / textureWidth;
                        vertexData.texCoords.v = (region.y + v * region.height) / textureHeight;
                    }

                    vertexData.colors = cocos2d::Color4B::WHITE;

                    if (boundsRect.origin.x > x)
                    {
                        boundsRect.origin.x = x;
                    }

                    if (boundsRect.size.width < x)
                    {
                        boundsRect.size.width = x;
                    }

                    if (boundsRect.origin.y > -y)
                    {
                        boundsRect.origin.y = -y;
                    }

                    if (boundsRect.size.height < -y)
                    {
                        boundsRect.size.height = -y;
                    }
                }

                boundsRect.size.width -= boundsRect.origin.x;
                boundsRect.size.height -= boundsRect.origin.y;

                for (std::size_t i = 0; i < triangleCount * 3; ++i)
                {
                    vertexIndices[i] = intArray[currentVerticesData->offset + (unsigned)BinaryOffset::MeshVertexIndices + i];
                }

                _textureScale = 1.0f;

                const auto isSkinned = currentVerticesData->weight != nullptr;
                if (isSkinned)
                {
                    _identityTransform();
                }
            } else {
                adjustTriangles(4, 6);
                auto vertices = triangles.verts;
                auto vertexIndices = triangles.indices;
                
                float l = region.x / textureWidth;
                float b = (region.y + region.height) / textureHeight;
                float r = (region.x + region.width) / textureWidth;
                float t = region.y / textureHeight;
                
                vertices[0].texCoords.u = l; vertices[0].texCoords.v = b;
                vertices[1].texCoords.u = r; vertices[1].texCoords.v = b;
                vertices[2].texCoords.u = l; vertices[2].texCoords.v = t;
                vertices[3].texCoords.u = r; vertices[3].texCoords.v = t;
                
                vertices[0].vertices.x = vertices[2].vertices.x = 0;
                vertices[1].vertices.x = vertices[3].vertices.x = region.width;
                vertices[0].vertices.y = vertices[1].vertices.y = 0;
                vertices[2].vertices.y = vertices[3].vertices.y = region.height;
                
                vertexIndices[0] = 0;
                vertexIndices[1] = 1;
                vertexIndices[2] = 2;
                vertexIndices[3] = 1;
                vertexIndices[4] = 3;
                vertexIndices[5] = 2;
            }

            memcpy(worldVerts, triangles.verts, triangles.vertCount * sizeof(editor::V2F_T2F_C4B));
            
            _visibleDirty = true;
            _blendModeDirty = true; // Relpace texture will override blendMode and color.
            _colorDirty = true;

            return;
        }
    }
}

void CCSlot::_updateMesh() 
{
    const auto scale = _armature->_armatureData->scale;
    const auto& deformVertices = _deformVertices->vertices;
    const auto& bones = _deformVertices->bones;
    const auto verticesData = _deformVertices->verticesData;
    const auto weightData = verticesData->weight;

    const auto hasFFD = !deformVertices.empty();
    const auto textureData = static_cast<CCTextureData*>(_textureData);
    const auto vertices = triangles.verts;
    
    boundsRect.origin.x = 999999.0f;
    boundsRect.origin.y = 999999.0f;
    boundsRect.size.width = -999999.0f;
    boundsRect.size.height = -999999.0f;

    if (!textureData)
    {
        return;
    }

    if (weightData != nullptr)
    {
        const auto data = verticesData->data;
        const auto intArray = data->intArray;
        const auto floatArray = data->floatArray;
        const auto vertexCount = (std::size_t)intArray[verticesData->offset + (unsigned)BinaryOffset::MeshVertexCount];
        int weightFloatOffset = intArray[weightData->offset + (unsigned)BinaryOffset::WeigthFloatOffset];

        if (vertexCount > triangles.vertCount) {
            return;
        }
        
        if (weightFloatOffset < 0)
        {
            weightFloatOffset += 65536; // Fixed out of bouds bug. 
        }

        for (
            std::size_t i = 0, iB = weightData->offset + (unsigned)BinaryOffset::WeigthBoneIndices + bones.size(), iV = (std::size_t)weightFloatOffset, iF = 0;
            i < vertexCount;
            ++i
        )
        {
            const auto boneCount = (std::size_t)intArray[iB++];
            auto xG = 0.0f, yG = 0.0f;
            for (std::size_t j = 0; j < boneCount; ++j)
            {
                const auto boneIndex = (unsigned)intArray[iB++];
                const auto bone = bones[boneIndex];
                if (bone != nullptr) 
                {
                    const auto& matrix = bone->globalTransformMatrix;
                    const auto weight = floatArray[iV++];
                    auto xL = floatArray[iV++] * scale;
                    auto yL = floatArray[iV++] * scale;

                    if (hasFFD) 
                    {
                        xL += deformVertices[iF++];
                        yL += deformVertices[iF++];
                    }

                    xG += (matrix.a * xL + matrix.c * yL + matrix.tx) * weight;
                    yG += (matrix.b * xL + matrix.d * yL + matrix.ty) * weight;
                }
            }

            auto& vertex = vertices[i];
            auto& vertexPosition = vertex.vertices;

            vertexPosition.x = xG;
            vertexPosition.y = -yG;

            if (boundsRect.origin.x > xG)
            {
                boundsRect.origin.x = xG;
            }

            if (boundsRect.size.width < xG)
            {
                boundsRect.size.width = xG;
            }

            if (boundsRect.origin.y > -yG)
            {
                boundsRect.origin.y = -yG;
            }

            if (boundsRect.size.height < -yG)
            {
                boundsRect.size.height = -yG;
            }
        }
    }
    else if (hasFFD)
    {
        const auto data = verticesData->data;
        const auto intArray = data->intArray;
        const auto floatArray = data->floatArray;
        const auto vertexCount = (std::size_t)intArray[verticesData->offset + (unsigned)BinaryOffset::MeshVertexCount];
        int vertexOffset = (std::size_t)intArray[verticesData->offset + (unsigned)BinaryOffset::MeshFloatOffset];

        if (vertexCount > triangles.vertCount) {
            return;
        }
        
        if (vertexOffset < 0)
        {
            vertexOffset += 65536; // Fixed out of bouds bug. 
        }

        for (std::size_t i = 0, l = vertexCount * 2; i < l; i += 2)
        {
            const auto iH = i / 2;
            const auto xG = floatArray[vertexOffset + i] * scale + deformVertices[i];
            const auto yG = floatArray[vertexOffset + i + 1] * scale + deformVertices[i + 1];

            auto& vertex = vertices[iH];
            auto& vertexPosition = vertex.vertices;

            vertexPosition.x = xG;
            vertexPosition.y = -yG;

            if (boundsRect.origin.x > xG)
            {
                boundsRect.origin.x = xG;
            }

            if (boundsRect.size.width < xG)
            {
                boundsRect.size.width = xG;
            }

            if (boundsRect.origin.y > -yG)
            {
                boundsRect.origin.y = -yG;
            }

            if (boundsRect.size.height < -yG)
            {
                boundsRect.size.height = -yG;
            }
        }
    }

    boundsRect.size.width -= boundsRect.origin.x;
    boundsRect.size.height -= boundsRect.origin.y;

    if (weightData != nullptr) 
    {
        _identityTransform();
    }
}

void CCSlot::_updateTransform()
{
    _localMatrix.m[0] = globalTransformMatrix.a;
    _localMatrix.m[1] = globalTransformMatrix.b;
    _localMatrix.m[4] = -globalTransformMatrix.c;
    _localMatrix.m[5] = -globalTransformMatrix.d;

    if (_childArmature)
    {
        _localMatrix.m[12] = globalTransformMatrix.tx;
        _localMatrix.m[13] = globalTransformMatrix.ty;
    }
    else 
    {
        if (_textureScale != 1.0f)
        {
            _localMatrix.m[0] *= _textureScale;
            _localMatrix.m[1] *= _textureScale;
            _localMatrix.m[4] *= _textureScale;
            _localMatrix.m[5] *= _textureScale;
        }
        
        _localMatrix.m[12] = globalTransformMatrix.tx - (globalTransformMatrix.a * _pivotX - globalTransformMatrix.c * _pivotY);
        _localMatrix.m[13] = globalTransformMatrix.ty - (globalTransformMatrix.b * _pivotX - globalTransformMatrix.d * _pivotY);
    }
    
    _worldMatDirty = true;
}

void CCSlot::updateWorldMatrix()
{
    if (!_armature)return;
    
    CCSlot* parent = (CCSlot*)_armature->getParent();
    if (parent)
    {
        parent->updateWorldMatrix();
    }
    
    if (_worldMatDirty)
    {
        calculWorldMatrix();
        
        Armature* childArmature = getChildArmature();
        if(!childArmature)return;
        
        auto& slots = childArmature->getSlots();
        for (int i = 0; i < slots.size(); i++)
        {
            CCSlot* slot = (CCSlot*)slots[i];
            slot->_worldMatDirty = true;
        }
    }
}

void CCSlot::calculWorldMatrix()
{
    CCSlot* parent = (CCSlot*)_armature->getParent();
    if (parent)
    {
        worldMatrix = parent->worldMatrix * _localMatrix;
    }
    else
    {
        worldMatrix = _localMatrix;
    }
    
    _worldMatDirty = false;
}

void CCSlot::_identityTransform()
{
    _localMatrix.m[0] = 1.0f;
    _localMatrix.m[1] = 0.0f;
    _localMatrix.m[4] = -0.0f;
    _localMatrix.m[5] = -1.0f;
    _localMatrix.m[12] = 0.0f;
    _localMatrix.m[13] = 0.0f;
    
    _worldMatDirty = true;
}

void CCSlot::_updateVisible()
{
    
}

void CCSlot::_updateBlendMode()
{
    if (_childArmature != nullptr)
    {
        for (const auto slot : _childArmature->getSlots())
        {
            slot->_blendMode = _blendMode;
            slot->_updateBlendMode();
        }
    }
}

void CCSlot::_updateColor()
{
    color.r = _colorTransform.redMultiplier * 255.0f;
    color.g = _colorTransform.greenMultiplier * 255.0f;
    color.b = _colorTransform.blueMultiplier * 255.0f;
    color.a = _colorTransform.alphaMultiplier * 255.0f;
}

DRAGONBONES_NAMESPACE_END
