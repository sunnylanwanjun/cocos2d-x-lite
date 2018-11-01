/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "spine-creator-support/SpineRenderer.h"
#include "spine/extension.h"
#include "spine-creator-support/AttachmentVertices.h"
#include "spine-creator-support/CreatorAttachmentLoader.h"
#include <algorithm>
#include "IOBuffer.h"

USING_NS_CC;
using std::min;
using std::max;

using namespace editor;
using namespace spine;

static IOBuffer* _vertexBuffer = nullptr;
static IOBuffer* _indiceBuffer = nullptr;
static IOBuffer* _debugBuffer = nullptr;

SpineRenderer* SpineRenderer::create ()
{
    SpineRenderer* skeleton = new SpineRenderer();
    skeleton->autorelease();
    return skeleton;
}

SpineRenderer* SpineRenderer::createWithData (spSkeletonData* skeletonData, bool ownsSkeletonData)
{
	SpineRenderer* node = new SpineRenderer(skeletonData, ownsSkeletonData);
	node->autorelease();
	return node;
}

SpineRenderer* SpineRenderer::createWithFile (const std::string& skeletonDataFile, spAtlas* atlas, float scale)
{
	SpineRenderer* node = new SpineRenderer(skeletonDataFile, atlas, scale);
	node->autorelease();
	return node;
}

SpineRenderer* SpineRenderer::createWithFile (const std::string& skeletonDataFile, const std::string& atlasFile, float scale)
{
	SpineRenderer* node = new SpineRenderer(skeletonDataFile, atlasFile, scale);
	node->autorelease();
	return node;
}

void SpineRenderer::initialize ()
{

    // SpineRenderer::initalize may be invoked twice, need to check whether _worldVertics is already allocated to avoid memory leak.
    if (_worldVertices == nullptr)
        _worldVertices = new float[1000]; // Max number of vertices per mesh.
    
    if(_vertexBuffer == nullptr)
    {
        _vertexBuffer = new IOBuffer(se::Object::TypedArrayType::UINT32);
    }
    
    if(_indiceBuffer == nullptr)
    {
        _indiceBuffer = new IOBuffer(se::Object::TypedArrayType::UINT16);
    }
    
    if(_debugBuffer == nullptr)
    {
        _debugBuffer = new IOBuffer(se::Object::TypedArrayType::FLOAT32);
    }
}

void SpineRenderer::setSkeletonData (spSkeletonData *skeletonData, bool ownsSkeletonData)
{
	_skeleton = spSkeleton_create(skeletonData);
	_ownsSkeletonData = ownsSkeletonData;
}

SpineRenderer::SpineRenderer ()
{
}

SpineRenderer::SpineRenderer (spSkeletonData *skeletonData, bool ownsSkeletonData)
{
	initWithData(skeletonData, ownsSkeletonData);
}

SpineRenderer::SpineRenderer (const std::string& skeletonDataFile, spAtlas* atlas, float scale)
{
	initWithJsonFile(skeletonDataFile, atlas, scale);
}

SpineRenderer::SpineRenderer (const std::string& skeletonDataFile, const std::string& atlasFile, float scale)
{
	initWithJsonFile(skeletonDataFile, atlasFile, scale);
}

SpineRenderer::~SpineRenderer ()
{
	if (_ownsSkeletonData) spSkeletonData_dispose(_skeleton->data);
	spSkeleton_dispose(_skeleton);
	if (_atlas) spAtlas_dispose(_atlas);
	if (_attachmentLoader) spAttachmentLoader_dispose(_attachmentLoader);
	delete [] _worldVertices;
}

void SpineRenderer::initWithData (spSkeletonData* skeletonData, bool ownsSkeletonData)
{
	setSkeletonData(skeletonData, ownsSkeletonData);
	initialize();
}

void SpineRenderer::initWithJsonFile (const std::string& skeletonDataFile, spAtlas* atlas, float scale)
{
    _atlas = atlas;
	_attachmentLoader = SUPER(CreatorAttachmentLoader_create(_atlas));

	spSkeletonJson* json = spSkeletonJson_createWithLoader(_attachmentLoader);
	json->scale = scale;
	spSkeletonData* skeletonData = spSkeletonJson_readSkeletonDataFile(json, skeletonDataFile.c_str());
	CCASSERT(skeletonData, json->error ? json->error : "Error reading skeleton data.");
	spSkeletonJson_dispose(json);

	setSkeletonData(skeletonData, true);
	initialize();
}

void SpineRenderer::initWithJsonFile (const std::string& skeletonDataFile, const std::string& atlasFile, float scale)
{
	_atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
	CCASSERT(_atlas, "Error reading atlas file.");

	_attachmentLoader = SUPER(CreatorAttachmentLoader_create(_atlas));

	spSkeletonJson* json = spSkeletonJson_createWithLoader(_attachmentLoader);
	json->scale = scale;
	spSkeletonData* skeletonData = spSkeletonJson_readSkeletonDataFile(json, skeletonDataFile.c_str());
	CCASSERT(skeletonData, json->error ? json->error : "Error reading skeleton data file.");
	spSkeletonJson_dispose(json);

	setSkeletonData(skeletonData, true);
	initialize();
}
    
void SpineRenderer::initWithBinaryFile (const std::string& skeletonDataFile, spAtlas* atlas, float scale)
{
    _atlas = atlas;
    _attachmentLoader = SUPER(CreatorAttachmentLoader_create(_atlas));
    
    spSkeletonBinary* binary = spSkeletonBinary_createWithLoader(_attachmentLoader);
    binary->scale = scale;
    spSkeletonData* skeletonData = spSkeletonBinary_readSkeletonDataFile(binary, skeletonDataFile.c_str());
    CCASSERT(skeletonData, binary->error ? binary->error : "Error reading skeleton data file.");
    spSkeletonBinary_dispose(binary);
    
    setSkeletonData(skeletonData, true);
    initialize();
}

void SpineRenderer::initWithBinaryFile (const std::string& skeletonDataFile, const std::string& atlasFile, float scale)
{
    _atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
    CCASSERT(_atlas, "Error reading atlas file.");
    
    _attachmentLoader = SUPER(CreatorAttachmentLoader_create(_atlas));
    
    spSkeletonBinary* binary = spSkeletonBinary_createWithLoader(_attachmentLoader);
    binary->scale = scale;
    spSkeletonData* skeletonData = spSkeletonBinary_readSkeletonDataFile(binary, skeletonDataFile.c_str());
    CCASSERT(skeletonData, binary->error ? binary->error : "Error reading skeleton data file.");
    spSkeletonBinary_dispose(binary);
    
    setSkeletonData(skeletonData, true);
    initialize();
}

void SpineRenderer::update (float deltaTime)
{
	spSkeleton_update(_skeleton, deltaTime * _timeScale);
}

se_object_ptr SpineRenderer::getRenderData ()
{
    _skeleton->r = _nodeColor.r / (float)255;
    _skeleton->g = _nodeColor.g / (float)255;
    _skeleton->b = _nodeColor.b / (float)255;
    _skeleton->a = _nodeColor.a / (float)255;
    
    Color4F color;
    AttachmentVertices* attachmentVertices = nullptr;
    
    int preBlendSrc = -1;
    int preBlendDst = -1;
    int preTextureIndex = -1;
    int curBlendSrc = -1;
    int curBlendDst = -1;
    int curTextureIndex = -1;
    
    int preVSegWritePos = -1;
    int preISegWritePos = -1;
    int curVSegLen = 0;
    int curISegLen = 0;
    
    int debugSlotsLen = 0;
    int materialLen = 0;
    
    _vertexBuffer->reset();
    _debugBuffer->reset();
    _indiceBuffer->reset();
    
    //reserved 4 bytes to save material len
    _vertexBuffer->writeUint32(0);
    if (_debugSlots)
    {
        //reserved 4 bytes to save debug slots len
        _debugBuffer->writeUint32(0);
    }
    
    for (int i = 0, n = _skeleton->slotsCount; i < n; ++i)
    {
        spSlot* slot = _skeleton->drawOrder[i];
        if (!slot->attachment) continue;
        
        switch (slot->attachment->type)
        {
            case SP_ATTACHMENT_REGION:
            {
                spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
                spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
                attachmentVertices = getAttachmentVertices(attachment);
                color.r = attachment->r;
                color.g = attachment->g;
                color.b = attachment->b;
                color.a = attachment->a;
                
                if(_debugSlots)
                {
                    _debugBuffer->writeFloat32(_worldVertices[0]);
                    _debugBuffer->writeFloat32(_worldVertices[1]);
                    _debugBuffer->writeFloat32(_worldVertices[2]);
                    _debugBuffer->writeFloat32(_worldVertices[3]);
                    _debugBuffer->writeFloat32(_worldVertices[4]);
                    _debugBuffer->writeFloat32(_worldVertices[5]);
                    _debugBuffer->writeFloat32(_worldVertices[6]);
                    _debugBuffer->writeFloat32(_worldVertices[7]);
                    debugSlotsLen+=8;
                }
                
                break;
            }
            case SP_ATTACHMENT_MESH:
            {
                spMeshAttachment* attachment = (spMeshAttachment*)slot->attachment;
                spMeshAttachment_computeWorldVertices(attachment, slot, _worldVertices);
                attachmentVertices = getAttachmentVertices(attachment);
                color.r = attachment->r;
                color.g = attachment->g;
                color.b = attachment->b;
                color.a = attachment->a;
                break;
            }
            default:
                continue;
        }
        
        switch (slot->data->blendMode)
        {
            case SP_BLEND_MODE_ADDITIVE:
                curBlendSrc = _premultipliedAlpha ? GL_ONE : GL_SRC_ALPHA;
                curBlendDst = GL_ONE;
                break;
            case SP_BLEND_MODE_MULTIPLY:
                curBlendSrc = GL_DST_COLOR;
                curBlendDst = GL_ONE_MINUS_SRC_ALPHA;
                break;
            case SP_BLEND_MODE_SCREEN:
                curBlendSrc = GL_ONE;
                curBlendDst = GL_ONE_MINUS_SRC_COLOR;
                break;
            default:
                curBlendSrc = _premultipliedAlpha ? GL_ONE : GL_SRC_ALPHA;
                curBlendDst = GL_ONE_MINUS_SRC_ALPHA;
        }
        
        //jsb
        curTextureIndex = attachmentVertices->_texture->getRealTextureIndex();
        if (preTextureIndex != curTextureIndex || preBlendDst != curBlendDst || preBlendSrc != curBlendSrc)
        {
            
            if (preVSegWritePos != -1)
            {
                _vertexBuffer->writeUint32(preVSegWritePos,curVSegLen);
                _vertexBuffer->writeUint32(preISegWritePos,curISegLen);
            }
            
            _vertexBuffer->writeUint32(curTextureIndex);
            _vertexBuffer->writeUint32(curBlendSrc);
            _vertexBuffer->writeUint32(curBlendDst);
            
            //reserve vertex segamentation lenght
            preVSegWritePos = _vertexBuffer->getCurPos();
            _vertexBuffer->writeUint32(0);
            //reserve indice segamentation lenght
            preISegWritePos = _vertexBuffer->getCurPos();
            _vertexBuffer->writeUint32(0);
            
            preTextureIndex = curTextureIndex;
            preBlendDst = curBlendDst;
            preBlendSrc = curBlendSrc;
            
            curVSegLen = 0;
            curISegLen = 0;
            
            materialLen++;
        }
        
        color.a *= _skeleton->a * slot->a * 255;
        float multiplier = _premultipliedAlpha ? color.a : 255;
        color.r *= _skeleton->r * slot->r * multiplier;
        color.g *= _skeleton->g * slot->g * multiplier;
        color.b *= _skeleton->b * slot->b * multiplier;
        
        for (int v = 0, w = 0, vn = attachmentVertices->_triangles->vertCount; v < vn; ++v, w += 2)
        {
            V2F_T2F_C4B* vertex = attachmentVertices->_triangles->verts + v;
            vertex->vertices.x = _worldVertices[w];
            vertex->vertices.y = _worldVertices[w + 1];
            vertex->colors.r = (GLubyte)color.r;
            vertex->colors.g = (GLubyte)color.g;
            vertex->colors.b = (GLubyte)color.b;
            vertex->colors.a = (GLubyte)color.a;
        }
        
        _vertexBuffer->writeBytes((char*)attachmentVertices->_triangles->verts,
                               attachmentVertices->_triangles->vertCount*sizeof(editor::V2F_T2F_C4B));
        
        if (curVSegLen > 0)
        {
            for (int ii = 0, nn = attachmentVertices->_triangles->indexCount; ii < nn; ii++)
            {
                _indiceBuffer->writeUint16(attachmentVertices->_triangles->indices[ii] + curVSegLen);
            }
        }
        else
        {
            _indiceBuffer->writeBytes((char*)attachmentVertices->_triangles->indices,
                                      attachmentVertices->_triangles->indexCount*sizeof(unsigned short));
        }
        
        curVSegLen += attachmentVertices->_triangles->vertCount;
        curISegLen += attachmentVertices->_triangles->indexCount;
    }
    
    if (_debugSlots)
    {
        //add 0.1 is avoid precision trouble
        _debugBuffer->writeFloat32(0, debugSlotsLen+0.1);
    }
    
    _vertexBuffer->writeUint32(0, materialLen);
    if (preVSegWritePos != -1)
    {
        _vertexBuffer->writeUint32(preVSegWritePos, curVSegLen);
        _vertexBuffer->writeUint32(preISegWritePos, curISegLen);
    }
    
    if (_debugBones)
    {
        _debugBuffer->writeFloat32(_skeleton->bonesCount*4 + 0.1);
        for (int i = 0, n = _skeleton->bonesCount; i < n; i++)
        {
            spBone *bone = _skeleton->bones[i];
            float x = bone->data->length * bone->a + bone->worldX;
            float y = bone->data->length * bone->c + bone->worldY;
            _debugBuffer->writeFloat32(bone->worldX);
            _debugBuffer->writeFloat32(bone->worldY);
            _debugBuffer->writeFloat32(x);
            _debugBuffer->writeFloat32(y);
        }
    }
    
    return _vertexBuffer->getTypeArray();
}

AttachmentVertices* SpineRenderer::getAttachmentVertices (spRegionAttachment* attachment) const
{
    return (AttachmentVertices*)attachment->rendererObject;
}

AttachmentVertices* SpineRenderer::getAttachmentVertices (spMeshAttachment* attachment) const
{
    return (AttachmentVertices*)attachment->rendererObject;
}

void SpineRenderer::updateWorldTransform ()
{
	spSkeleton_updateWorldTransform(_skeleton);
}

void SpineRenderer::setToSetupPose ()
{
	spSkeleton_setToSetupPose(_skeleton);
}

void SpineRenderer::setBonesToSetupPose ()
{
	spSkeleton_setBonesToSetupPose(_skeleton);
}

void SpineRenderer::setSlotsToSetupPose ()
{
	spSkeleton_setSlotsToSetupPose(_skeleton);
}

spBone* SpineRenderer::findBone (const std::string& boneName) const
{
	return spSkeleton_findBone(_skeleton, boneName.c_str());
}

spSlot* SpineRenderer::findSlot (const std::string& slotName) const
{
	return spSkeleton_findSlot(_skeleton, slotName.c_str());
}

bool SpineRenderer::setSkin (const std::string& skinName)
{
	return spSkeleton_setSkinByName(_skeleton, skinName.empty() ? 0 : skinName.c_str()) ? true : false;
}

bool SpineRenderer::setSkin (const char* skinName)
{
	return spSkeleton_setSkinByName(_skeleton, skinName) ? true : false;
}

spAttachment* SpineRenderer::getAttachment (const std::string& slotName, const std::string& attachmentName) const
{
	return spSkeleton_getAttachmentForSlotName(_skeleton, slotName.c_str(), attachmentName.c_str());
}

bool SpineRenderer::setAttachment (const std::string& slotName, const std::string& attachmentName)
{
	return spSkeleton_setAttachment(_skeleton, slotName.c_str(), attachmentName.empty() ? 0 : attachmentName.c_str()) ? true : false;
}

bool SpineRenderer::setAttachment (const std::string& slotName, const char* attachmentName)
{
	return spSkeleton_setAttachment(_skeleton, slotName.c_str(), attachmentName) ? true : false;
}

se_object_ptr SpineRenderer::getIndiceData() const
{
    return _indiceBuffer->getTypeArray();
}

se_object_ptr SpineRenderer::getDebugData() const
{
    return _debugBuffer->getTypeArray();
}

spSkeleton* SpineRenderer::getSkeleton () const
{
	return _skeleton;
}

void SpineRenderer::setTimeScale (float scale)
{
	_timeScale = scale;
}

float SpineRenderer::getTimeScale () const
{
	return _timeScale;
}

void SpineRenderer::setColor (cocos2d::Color4B& color)
{
    _nodeColor = color;
}

void SpineRenderer::setDebugBonesEnabled (bool enabled)
{
    _debugBones = enabled;
}

void SpineRenderer::setDebugSlotsEnabled (bool enabled)
{
    _debugSlots = enabled;
}
    
void SpineRenderer::setOpacityModifyRGB (bool value)
{
    _premultipliedAlpha = value;
}

bool SpineRenderer::isOpacityModifyRGB () const
{
    return _premultipliedAlpha;
}
