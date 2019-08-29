/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated May 1, 2019. Replaces all prior versions.
 *
 * Copyright (c) 2013-2019, Esoteric Software LLC
 *
 * Integration of the Spine Runtimes into software or otherwise creating
 * derivative works of the Spine Runtimes is permitted under the terms and
 * conditions of Section 2 of the Spine Editor License Agreement:
 * http://esotericsoftware.com/spine-editor-license
 *
 * Otherwise, it is permitted to integrate the Spine Runtimes into software
 * or otherwise create derivative works of the Spine Runtimes (collectively,
 * "Products"), provided that each user of the Products must obtain their own
 * Spine Editor license and redistribution of the Products in any form must
 * include this license and copyright notice.
 *
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE LLC "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL ESOTERIC SOFTWARE LLC BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, BUSINESS
 * INTERRUPTION, OR LOSS OF USE, DATA, OR PROFITS) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "SkeletonCacheAnimation.h"
#include "MiddlewareMacro.h"
#include "SkeletonCacheMgr.h"
#include "renderer/renderer/Pass.h"
#include "renderer/renderer/Technique.h"
#include "renderer/scene/assembler/CustomAssembler.hpp"

USING_NS_CC;
USING_NS_MW;
using namespace cocos2d::renderer;

namespace spine {
    
    SkeletonCacheAnimation::SkeletonCacheAnimation (const std::string& uuid, bool isShare)
    :_isShare(isShare) {
        if (isShare) {
            _skeletonCache = SkeletonCacheMgr::getInstance()->retainSkeletonCache(uuid);
        } else {
            _skeletonCache = new SkeletonCache();
            _skeletonCache->initWithUUID(uuid);
        }
    }
    
    SkeletonCacheAnimation::~SkeletonCacheAnimation () {
        if (_isShare) {
            SkeletonCacheMgr::getInstance()->releaseSkeletonCache(_uuid);
            _skeletonCache = nullptr;
        } else {
            delete _skeletonCache;
            _skeletonCache = nullptr;
        }
        while (!_animationQueue.empty()) {
            auto ani = _animationQueue.front();
            _animationQueue.pop();
            delete ani;
        }
        CC_SAFE_RELEASE(_nodeProxy);
        CC_SAFE_RELEASE(_effect);
        stopSchedule();
    }
    
    void SkeletonCacheAnimation::update(float dt) {
        if (_paused) return;
        
        auto gTimeScale = SkeletonAnimation::GlobalTimeScale;
        dt *= _timeScale * gTimeScale;
        
        if (_isAniComplete) {
            if (_animationQueue.empty() && !_headAnimation) {
                return;
            }
            if (!_headAnimation) {
                _headAnimation = _animationQueue.front();
                _animationQueue.pop();
            }
            if (!_headAnimation) {
                return;
            }
            _accTime += dt;
            if (_accTime > _headAnimation->delay) {
                std::string name = _headAnimation->animationName;
                bool loop = _headAnimation->loop;
                delete _headAnimation;
                _headAnimation = nullptr;
                setAnimation(name, loop);
                return;
            }
        }
        
        if (!_animationData) return;
        
        if (_accTime <= 0.00001 && _playCount == 0) {
            if (_startListener) {
                _startListener(_animationName);
            }
        }
        
        _accTime += dt;
        int frameIdx = floor(_accTime / SkeletonCache::FrameTime);
        if (!_animationData->isComplete()) {
            _skeletonCache->updateToFrame(_animationName, frameIdx);
        }
        
        int finalFrameIndex = (int)_animationData->getFrameCount() - 1;
        if (_animationData->isComplete() && frameIdx >= finalFrameIndex) {
            _playCount++;
            _accTime = 0.0f;
            if (_playTimes > 0 && _playCount >= _playTimes) {
                frameIdx = finalFrameIndex;
                _playCount = 0;
                _isAniComplete = true;
            } else {
                frameIdx = 0;
            }
            if (_endListener) {
                _endListener(_animationName);
            }
            if (_completeListener) {
                _completeListener(_animationName);
            }
        }
        _curFrameIndex = frameIdx;
    }
    
    void SkeletonCacheAnimation::render(float dt) {
        if (!_animationData) return;
        SkeletonCache::FrameData* frameData = _animationData->getFrameData(_curFrameIndex);
        if (!frameData) return;
        
        auto mgr = MiddlewareManager::getInstance();
        if (!mgr->isRendering) return;
        
        if (_nodeProxy == nullptr) {
            return;
        }
        
        CustomAssembler* assembler = (CustomAssembler*)_nodeProxy->getAssembler();
        if (assembler == nullptr) {
            return;
        }
        assembler->reset();
        assembler->setUseModel(!_batch);
        
        _nodeColor.a = _nodeProxy->getRealOpacity() / (float)255;
        
        middleware::MeshBuffer* mb = mgr->getMeshBuffer(VF_XYUVCC);
        middleware::IOBuffer& vb = mb->getVB();
        middleware::IOBuffer& ib = mb->getIB();
        
        const cocos2d::Mat4& nodeWorldMat = _nodeProxy->getWorldMatrix();
        auto& segments = frameData->getSegments();
        auto& colors = frameData->getColors();
        
        int colorOffset = 0;
        SkeletonCache::ColorData* nowColor = colors[colorOffset++];
        auto maxVFOffset = nowColor->vertexFloatOffset;
        
        Color4F finalColor;
        Color4F darkColor;
        Color4F tempColor;
        
        for (int i = 0, n = segments.size(); i < n; i++) {
            auto segment = segments[i];
            
        }
    }
    
    Skeleton* SkeletonCacheAnimation::getSkeleton() const {
        return _skeletonCache->getSkeleton();
    }
    
    void SkeletonCacheAnimation::setTimeScale (float scale) {
        _timeScale = scale;
    }
    
    float SkeletonCacheAnimation::getTimeScale () const {
        return _timeScale;
    }
    
    void SkeletonCacheAnimation::paused (bool value) {
        _paused = value;
    }
    
    Bone* SkeletonCacheAnimation::findBone (const std::string& boneName) const {
        return _skeletonCache->findBone(boneName);
    }
    
    Slot* SkeletonCacheAnimation::findSlot (const std::string& slotName) const {
        return _skeletonCache->findSlot(slotName);
    }
    
    void SkeletonCacheAnimation::setSkin (const std::string& skinName) {
        _skeletonCache->setSkin(skinName);
        _skeletonCache->resetAllAnimationData();
    }

    void SkeletonCacheAnimation::setSkin (const char* skinName) {
        _skeletonCache->setSkin(skinName);
        _skeletonCache->resetAllAnimationData();
    }
    
    Attachment* SkeletonCacheAnimation::getAttachment (const std::string& slotName, const std::string& attachmentName) const {
        return _skeletonCache->getAttachment(slotName, attachmentName);
    }
    
    bool SkeletonCacheAnimation::setAttachment (const std::string& slotName, const std::string& attachmentName) {
        _skeletonCache->setAttachment(slotName, attachmentName);
        _skeletonCache->resetAllAnimationData();
    }
    
    bool SkeletonCacheAnimation::setAttachment (const std::string& slotName, const char* attachmentName) {
        _skeletonCache->setAttachment(slotName, attachmentName);
        _skeletonCache->resetAllAnimationData();
    }
    
    void SkeletonCacheAnimation::setColor (cocos2d::Color4B& color) {
        _nodeColor.r = color.r / 255.0f;
        _nodeColor.g = color.g / 255.0f;
        _nodeColor.b = color.b / 255.0f;
        _nodeColor.a = color.a / 255.0f;
    }
    
    void SkeletonCacheAnimation::setBatchEnabled (bool enabled) {
        _batch = enabled;
    }
    
    void SkeletonCacheAnimation::setOpacityModifyRGB (bool value) {
        _premultipliedAlpha = value;
    }
    
    bool SkeletonCacheAnimation::isOpacityModifyRGB () const {
        return _premultipliedAlpha;
    }
    
    void SkeletonCacheAnimation::beginSchedule() {
        MiddlewareManager::getInstance()->addTimer(this);
    }
    
    void SkeletonCacheAnimation::stopSchedule() {
        MiddlewareManager::getInstance()->removeTimer(this);
    }
    
    void SkeletonCacheAnimation::onEnable() {
        beginSchedule();
    }
    
    void SkeletonCacheAnimation::onDisable() {
        stopSchedule();
    }
    
    void SkeletonCacheAnimation::setAnimation (const std::string& name, bool loop) {
        _playTimes = loop ? 0 : 1;
        _animationName = name;
        _animationData = _skeletonCache->buildAnimationData(_animationName);
        _isAniComplete = false;
        _accTime = 0.0f;
        _playCount = 0;
        _curFrameIndex = 0;
    }
    
    void SkeletonCacheAnimation::addAnimation (const std::string& name, bool loop, float delay) {
        auto aniInfo = new AniQueueData();
        aniInfo->animationName = name;
        aniInfo->loop = loop;
        aniInfo->delay = delay;
        _animationQueue.push(aniInfo);
    }
    
    Animation* SkeletonCacheAnimation::findAnimation(const std::string& name) const {
        return _skeletonCache->findAnimation(name);
    }
    
    void SkeletonCacheAnimation::setStartListener (const CacheFrameEvent& listener) {
        _startListener = listener;
    }
    
    void SkeletonCacheAnimation::setEndListener (const CacheFrameEvent& listener) {
        _endListener = listener;
    }
    
    void SkeletonCacheAnimation::setCompleteListener (const CacheFrameEvent& listener) {
        _completeListener = listener;
    }
}
