#include "dragonBones-creator-support/CCSlot.h"
#include "dragonBones-creator-support/CCTextureAtlasData.h"
#include "dragonBones-creator-support/CCArmatureDisplay.h"

DRAGONBONES_NAMESPACE_BEGIN

void CCSlot::_onClear()
{
    Slot::_onClear();
    disposeTriangles();
}

void CCSlot::disposeTriangles(){
    if (_triangles.verts) {
        delete[] _triangles.verts;
        _triangles.verts = nullptr;
    }
    if (_triangles.indices) {
        delete[] _triangles.indices;
        _triangles.indices = nullptr;
    }
    _triangles.indexCount = 0;
    _triangles.vertCount = 0;
}

void CCSlot::adjustTriangles(const unsigned vertexCount, const unsigned indicesCount){
    if (_triangles.vertCount < vertexCount) {
        if (_triangles.verts) {
            delete[] _triangles.verts;
        }
        _triangles.verts = new editor::V2F_T2F_C4B[vertexCount];
    }
    _triangles.vertCount = vertexCount;
    
    if (_triangles.indexCount < indicesCount) {
        if (_triangles.indices) {
            delete[] _triangles.indices;
        }
        _triangles.indices = new unsigned short[indicesCount];
    }
    _triangles.indexCount = indicesCount;
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

void CCSlot::_updateFrame()
{
    const auto currentVerticesData = (_deformVertices != nullptr && _display == _meshDisplay) ? _deformVertices->verticesData : nullptr;
    const auto currentTextureData = static_cast<CCTextureData*>(_textureData);

    if (_displayIndex >= 0 && _display != nullptr && currentTextureData != nullptr)
    {
        if (currentTextureData->spriteFrame != nullptr)
        {
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

                const auto& region = currentTextureData->region;
                const auto texture = currentTextureData->spriteFrame->getTexture();
                const auto textureWidth = texture->getPixelsWide();
                const auto textureHeight = texture->getPixelsHigh();
                const unsigned indicesCount = triangleCount * 3;
                adjustTriangles(vertexCount, indicesCount);
                
                auto vertices = _triangles.verts;
                auto vertexIndices = _triangles.indices;
                _boundsRect.origin.x = 999999.0f;
                _boundsRect.origin.y = 999999.0f;
                _boundsRect.size.width = -999999.0f;
                _boundsRect.size.height = -999999.0f;
                
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

                    if (_boundsRect.origin.x > x)
                    {
                        _boundsRect.origin.x = x;
                    }

                    if (_boundsRect.size.width < x)
                    {
                        _boundsRect.size.width = x;
                    }

                    if (_boundsRect.origin.y > -y)
                    {
                        _boundsRect.origin.y = -y;
                    }

                    if (_boundsRect.size.height < -y)
                    {
                        _boundsRect.size.height = -y;
                    }
                }

                _boundsRect.size.width -= _boundsRect.origin.x;
                _boundsRect.size.height -= _boundsRect.origin.y;

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
            }

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
    const auto vertices = _triangles.verts;
    
    _boundsRect.origin.x = 999999.0f;
    _boundsRect.origin.y = 999999.0f;
    _boundsRect.size.width = -999999.0f;
    _boundsRect.size.height = -999999.0f;

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

        if (vertexCount > _triangles.vertCount) {
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

            if (_boundsRect.origin.x > xG)
            {
                _boundsRect.origin.x = xG;
            }

            if (_boundsRect.size.width < xG)
            {
                _boundsRect.size.width = xG;
            }

            if (_boundsRect.origin.y > -yG)
            {
                _boundsRect.origin.y = -yG;
            }

            if (_boundsRect.size.height < -yG)
            {
                _boundsRect.size.height = -yG;
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

        if (vertexCount > _triangles.vertCount) {
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

            if (_boundsRect.origin.x > xG)
            {
                _boundsRect.origin.x = xG;
            }

            if (_boundsRect.size.width < xG)
            {
                _boundsRect.size.width = xG;
            }

            if (_boundsRect.origin.y > -yG)
            {
                _boundsRect.origin.y = -yG;
            }

            if (_boundsRect.size.height < -yG)
            {
                _boundsRect.size.height = -yG;
            }
        }
    }

    _boundsRect.size.width -= _boundsRect.origin.x;
    _boundsRect.size.height -= _boundsRect.origin.y;

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

void CCSlot::updateWorldMatrix(){
    if (!_armature)return;
    
    CCSlot* parent = (CCSlot*)_armature->getParent();
    if (parent) {
        parent->updateWorldMatrix();
    }
    
    if (_worldMatDirty) {
        calculWorldMatrix();
        
        auto childArmature = getChildArmature();
        if(!childArmature)return;
        
        auto slots = childArmature->getSlots();
        for (int i = 0; i < slots.size(); i++) {
            CCSlot* slot = (CCSlot*)slots[i];
            slot->_worldMatDirty = true;
        }
    }
}

void CCSlot::calculWorldMatrix(){
    CCSlot* parent = (CCSlot*)_armature->getParent();
    if (parent) {
        _worldMatrix = parent->_worldMatrix * _localMatrix;
    } else {
        _worldMatrix = _localMatrix;
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
    _color.r = _colorTransform.redMultiplier * 255.0f;
    _color.g = _colorTransform.greenMultiplier * 255.0f;
    _color.b = _colorTransform.blueMultiplier * 255.0f;
    _color.a = _colorTransform.alphaMultiplier * 255.0f;
}

DRAGONBONES_NAMESPACE_END
