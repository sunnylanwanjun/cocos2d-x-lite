/******************************************************************************
 * Spine Runtimes Software License v2.5
 *
 * Copyright (c) 2013-2016, Esoteric Software
 * All rights reserved.
 *
 * You are granted a perpetual, non-exclusive, non-sublicensable, and
 * non-transferable license to use, install, execute, and perform the Spine
 * Runtimes software and derivative works solely for personal or internal
 * use. Without the written permission of Esoteric Software (see Section 2 of
 * the Spine Software License Agreement), you may not (a) modify, translate,
 * adapt, or develop new applications using the Spine Runtimes or otherwise
 * create derivative works or improvements of the Spine Runtimes or (b) remove,
 * delete, alter, or obscure any trademarks or any copyright, trademark, patent,
 * or other intellectual property or proprietary rights notices on or in the
 * Software, including any copy thereof. Redistributions in binary or source
 * form must include this license and terms.
 *
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ESOTERIC SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, BUSINESS INTERRUPTION, OR LOSS OF
 * USE, DATA, OR PROFITS) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "spine/jsb/SkeletonRenderer.h"
#include "spine/extension.h"
#include "spine/jsb/AttachmentVertices.h"
#include "spine/jsb/Cocos2dAttachmentLoader.h"
#include <algorithm>
#include "IOBuffer.hpp"

USING_NS_CC;
using std::min;
using std::max;

using namespace spine;

static cocos2d::IOBuffer * _vertexBuffer;
static cocos2d::IOBuffer * _indiceBuffer;
static cocos2d::IOBuffer * _debugBuffer;

SkeletonRenderer* SkeletonRenderer::create(){
    SkeletonRenderer* skeleton = new SkeletonRenderer();
    skeleton->autorelease();
    return skeleton;
}

SkeletonRenderer* SkeletonRenderer::createWithData (spSkeletonData* skeletonData, bool ownsSkeletonData) {
	SkeletonRenderer* node = new SkeletonRenderer(skeletonData, ownsSkeletonData);
	node->autorelease();
	return node;
}

SkeletonRenderer* SkeletonRenderer::createWithFile (const std::string& skeletonDataFile, spAtlas* atlas, float scale) {
	SkeletonRenderer* node = new SkeletonRenderer(skeletonDataFile, atlas, scale);
	node->autorelease();
	return node;
}

SkeletonRenderer* SkeletonRenderer::createWithFile (const std::string& skeletonDataFile, const std::string& atlasFile, float scale) {
	SkeletonRenderer* node = new SkeletonRenderer(skeletonDataFile, atlasFile, scale);
	node->autorelease();
	return node;
}

void SkeletonRenderer::initialize () {

    // SkeletonRenderer::initalize may be invoked twice, need to check whether _worldVertics is already allocated to avoid memory leak.
    if (_worldVertices == nullptr)
        _worldVertices = new float[1000]; // Max number of vertices per mesh.
    
    if(_vertexBuffer == nullptr){
        _vertexBuffer = new cocos2d::IOBuffer(se::Object::TypedArrayType::UINT32);
    }
    if(_indiceBuffer == nullptr){
        _indiceBuffer = new cocos2d::IOBuffer(se::Object::TypedArrayType::UINT16);
    }
    if(_debugBuffer == nullptr){
        _debugBuffer = new cocos2d::IOBuffer(se::Object::TypedArrayType::FLOAT32);
    }
}

void SkeletonRenderer::setSkeletonData (spSkeletonData *skeletonData, bool ownsSkeletonData) {
	_skeleton = spSkeleton_create(skeletonData);
	_ownsSkeletonData = ownsSkeletonData;
}

SkeletonRenderer::SkeletonRenderer ()
	: _atlas(nullptr)
    , _attachmentLoader(nullptr)
    , _worldVertices(nullptr)
    , _timeScale(1)
    , _debugBones(false)
    , _nodeColor(cocos2d::Color4B::WHITE)
    , _premultipliedAlpha(false)
    , _debugSlots(false){
}

SkeletonRenderer::SkeletonRenderer (spSkeletonData *skeletonData, bool ownsSkeletonData)
    : _atlas(nullptr)
    , _attachmentLoader(nullptr)
    , _worldVertices(nullptr)
    , _timeScale(1)
    , _debugBones(false)
    , _nodeColor(cocos2d::Color4B::WHITE)
    , _premultipliedAlpha(false)
    , _debugSlots(false){
	initWithData(skeletonData, ownsSkeletonData);
}

SkeletonRenderer::SkeletonRenderer (const std::string& skeletonDataFile, spAtlas* atlas, float scale)
    : _atlas(nullptr)
    , _attachmentLoader(nullptr)
    , _worldVertices(nullptr)
    , _timeScale(1)
    , _debugBones(false)
    , _nodeColor(cocos2d::Color4B::WHITE)
    , _premultipliedAlpha(false)
    , _debugSlots(false){
	initWithJsonFile(skeletonDataFile, atlas, scale);
}

SkeletonRenderer::SkeletonRenderer (const std::string& skeletonDataFile, const std::string& atlasFile, float scale)
    : _atlas(nullptr)
    , _attachmentLoader(nullptr)
    , _worldVertices(nullptr)
    , _timeScale(1)
    , _debugBones(false)
    , _nodeColor(cocos2d::Color4B::WHITE)
    , _premultipliedAlpha(false)
    , _debugSlots(false){
	initWithJsonFile(skeletonDataFile, atlasFile, scale);
}

SkeletonRenderer::~SkeletonRenderer () {
	if (_ownsSkeletonData) spSkeletonData_dispose(_skeleton->data);
	spSkeleton_dispose(_skeleton);
	if (_atlas) spAtlas_dispose(_atlas);
	if (_attachmentLoader) spAttachmentLoader_dispose(_attachmentLoader);
	delete [] _worldVertices;
}

void SkeletonRenderer::initWithData (spSkeletonData* skeletonData, bool ownsSkeletonData) {
	setSkeletonData(skeletonData, ownsSkeletonData);

	initialize();
}

void SkeletonRenderer::initWithJsonFile (const std::string& skeletonDataFile, spAtlas* atlas, float scale) {
    _atlas = atlas;
	_attachmentLoader = SUPER(Cocos2dAttachmentLoader_create(_atlas));

	spSkeletonJson* json = spSkeletonJson_createWithLoader(_attachmentLoader);
	json->scale = scale;
	spSkeletonData* skeletonData = spSkeletonJson_readSkeletonDataFile(json, skeletonDataFile.c_str());
	CCASSERT(skeletonData, json->error ? json->error : "Error reading skeleton data.");
	spSkeletonJson_dispose(json);

	setSkeletonData(skeletonData, true);

	initialize();
}

void SkeletonRenderer::initWithJsonFile (const std::string& skeletonDataFile, const std::string& atlasFile, float scale) {
	_atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
	CCASSERT(_atlas, "Error reading atlas file.");

	_attachmentLoader = SUPER(Cocos2dAttachmentLoader_create(_atlas));

	spSkeletonJson* json = spSkeletonJson_createWithLoader(_attachmentLoader);
	json->scale = scale;
	spSkeletonData* skeletonData = spSkeletonJson_readSkeletonDataFile(json, skeletonDataFile.c_str());
	CCASSERT(skeletonData, json->error ? json->error : "Error reading skeleton data file.");
	spSkeletonJson_dispose(json);

	setSkeletonData(skeletonData, true);

	initialize();
}
    
void SkeletonRenderer::initWithBinaryFile (const std::string& skeletonDataFile, spAtlas* atlas, float scale) {
    _atlas = atlas;
    _attachmentLoader = SUPER(Cocos2dAttachmentLoader_create(_atlas));
    
    spSkeletonBinary* binary = spSkeletonBinary_createWithLoader(_attachmentLoader);
    binary->scale = scale;
    spSkeletonData* skeletonData = spSkeletonBinary_readSkeletonDataFile(binary, skeletonDataFile.c_str());
    CCASSERT(skeletonData, binary->error ? binary->error : "Error reading skeleton data file.");
    spSkeletonBinary_dispose(binary);
    
    setSkeletonData(skeletonData, true);
    
    initialize();
}

void SkeletonRenderer::initWithBinaryFile (const std::string& skeletonDataFile, const std::string& atlasFile, float scale) {
    _atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
    CCASSERT(_atlas, "Error reading atlas file.");
    
    _attachmentLoader = SUPER(Cocos2dAttachmentLoader_create(_atlas));
    
    spSkeletonBinary* binary = spSkeletonBinary_createWithLoader(_attachmentLoader);
    binary->scale = scale;
    spSkeletonData* skeletonData = spSkeletonBinary_readSkeletonDataFile(binary, skeletonDataFile.c_str());
    CCASSERT(skeletonData, binary->error ? binary->error : "Error reading skeleton data file.");
    spSkeletonBinary_dispose(binary);
    
    setSkeletonData(skeletonData, true);
    
    initialize();
}

void SkeletonRenderer::update (float deltaTime) {
	spSkeleton_update(_skeleton, deltaTime * _timeScale);
}

se_object_ptr SkeletonRenderer::getRenderData(){
    
    _skeleton->r = _nodeColor.r / (float)255;
    _skeleton->g = _nodeColor.g / (float)255;
    _skeleton->b = _nodeColor.b / (float)255;
    _skeleton->a = _nodeColor.a / (float)255;
    
    Color4F color;
    AttachmentVertices* attachmentVertices = nullptr;
    
    int preBlendSrc=-1;
    int preBlendDst=-1;
    int preTextureIndex=-1;
    int curBlendSrc=-1;
    int curBlendDst=-1;
    int curTextureIndex=-1;
    
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
    if(_debugSlots){
        //reserved 4 bytes to save debug slots len
        _debugBuffer->writeUint32(0);
    }
    
    for (int i = 0, n = _skeleton->slotsCount; i < n; ++i) {
        
        spSlot* slot = _skeleton->drawOrder[i];
        if (!slot->attachment) continue;
        
        switch (slot->attachment->type) {
            case SP_ATTACHMENT_REGION: {
                spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
                spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
                attachmentVertices = getAttachmentVertices(attachment);
                color.r = attachment->r;
                color.g = attachment->g;
                color.b = attachment->b;
                color.a = attachment->a;
                
                if(_debugSlots){
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
            case SP_ATTACHMENT_MESH: {
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
        
        switch (slot->data->blendMode) {
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
        if(preTextureIndex!=curTextureIndex||preBlendDst!=curBlendDst||preBlendSrc!=curBlendSrc){
            
            if(preVSegWritePos!=-1){
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
        
        for (int v = 0, w = 0, vn = attachmentVertices->_triangles->vertCount; v < vn; ++v, w += 2) {
            V2F_T2F_C4B* vertex = attachmentVertices->_triangles->verts + v;
            vertex->vertices.x = _worldVertices[w];
            vertex->vertices.y = _worldVertices[w + 1];
            vertex->colors.r = (GLubyte)color.r;
            vertex->colors.g = (GLubyte)color.g;
            vertex->colors.b = (GLubyte)color.b;
            vertex->colors.a = (GLubyte)color.a;
        }
        
        _vertexBuffer->writeBytes((char*)attachmentVertices->_triangles->verts,
                               attachmentVertices->_triangles->vertCount*sizeof(spine::V2F_T2F_C4B));
        
        if(curVSegLen>0){
            for(int ii=0,nn=attachmentVertices->_triangles->indexCount;ii<nn;ii++){
                _indiceBuffer->writeUint16(attachmentVertices->_triangles->indices[ii]+curVSegLen);
            }
        }else{
            _indiceBuffer->writeBytes((char*)attachmentVertices->_triangles->indices,
                                      attachmentVertices->_triangles->indexCount*sizeof(unsigned short));
        }
        
        curVSegLen += attachmentVertices->_triangles->vertCount;
        curISegLen += attachmentVertices->_triangles->indexCount;
    }
    if(_debugSlots){
        //add 0.1 is avoid precision trouble
        _debugBuffer->writeFloat32(0,debugSlotsLen+0.1);
    }
    _vertexBuffer->writeUint32(0, materialLen);
    if(preVSegWritePos!=-1){
        _vertexBuffer->writeUint32(preVSegWritePos,curVSegLen);
        _vertexBuffer->writeUint32(preISegWritePos,curISegLen);
    }
    
    if (_debugBones) {
        _debugBuffer->writeFloat32(_skeleton->bonesCount*4+0.1);
        for (int i = 0, n = _skeleton->bonesCount; i < n; i++) {
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

AttachmentVertices* SkeletonRenderer::getAttachmentVertices (spRegionAttachment* attachment) const {
    return (AttachmentVertices*)attachment->rendererObject;
}

AttachmentVertices* SkeletonRenderer::getAttachmentVertices (spMeshAttachment* attachment) const {
    return (AttachmentVertices*)attachment->rendererObject;
}

void SkeletonRenderer::updateWorldTransform () {
	spSkeleton_updateWorldTransform(_skeleton);
}

void SkeletonRenderer::setToSetupPose () {
	spSkeleton_setToSetupPose(_skeleton);
}
void SkeletonRenderer::setBonesToSetupPose () {
	spSkeleton_setBonesToSetupPose(_skeleton);
}
void SkeletonRenderer::setSlotsToSetupPose () {
	spSkeleton_setSlotsToSetupPose(_skeleton);
}

spBone* SkeletonRenderer::findBone (const std::string& boneName) const {
	return spSkeleton_findBone(_skeleton, boneName.c_str());
}

spSlot* SkeletonRenderer::findSlot (const std::string& slotName) const {
	return spSkeleton_findSlot(_skeleton, slotName.c_str());
}

bool SkeletonRenderer::setSkin (const std::string& skinName) {
	return spSkeleton_setSkinByName(_skeleton, skinName.empty() ? 0 : skinName.c_str()) ? true : false;
}

bool SkeletonRenderer::setSkin (const char* skinName) {
	return spSkeleton_setSkinByName(_skeleton, skinName) ? true : false;
}

spAttachment* SkeletonRenderer::getAttachment (const std::string& slotName, const std::string& attachmentName) const {
	return spSkeleton_getAttachmentForSlotName(_skeleton, slotName.c_str(), attachmentName.c_str());
}

bool SkeletonRenderer::setAttachment (const std::string& slotName, const std::string& attachmentName) {
	return spSkeleton_setAttachment(_skeleton, slotName.c_str(), attachmentName.empty() ? 0 : attachmentName.c_str()) ? true : false;
}

bool SkeletonRenderer::setAttachment (const std::string& slotName, const char* attachmentName) {
	return spSkeleton_setAttachment(_skeleton, slotName.c_str(), attachmentName) ? true : false;
}

se_object_ptr SkeletonRenderer::getIndiceData(){
    return _indiceBuffer->getTypeArray();
}

se_object_ptr SkeletonRenderer::getDebugData(){
    return _debugBuffer->getTypeArray();
}

spSkeleton* SkeletonRenderer::getSkeleton () {
	return _skeleton;
}

void SkeletonRenderer::setTimeScale (float scale) {
	_timeScale = scale;
}

float SkeletonRenderer::getTimeScale () const {
	return _timeScale;
}

void SkeletonRenderer::setColor(cocos2d::Color4B& color){
    _nodeColor = color;
}

void SkeletonRenderer::setDebugBonesEnabled(bool enabled){
    _debugBones = enabled;
}

void SkeletonRenderer::setDebugSlotsEnabled(bool enabled){
    _debugSlots = enabled;
}
    
void SkeletonRenderer::setOpacityModifyRGB (bool value) {
    _premultipliedAlpha = value;
}

bool SkeletonRenderer::isOpacityModifyRGB () const {
    return _premultipliedAlpha;
}
