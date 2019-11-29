/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2012-2018 DragonBones team and other contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "AttachedNode.h"
#include "ArmatureCache.h"

USING_NS_CC;
using namespace cocos2d::renderer;

DRAGONBONES_NAMESPACE_BEGIN

const std::vector<std::string>& AttachedNode::getSlotInfos(Armature* rootArmature) const
{
    static std::vector<std::string> attachedSlotNames;
    attachedSlotNames.clear();
    std::function<void(Armature*, int)> traverse = [&](Armature* armature, int parentIndex)
    {
        if (!armature) return;
        auto& slots = armature->getSlots();
        for (std::size_t i = 0, n = slots.size(); i < n; i++)
        {
            auto& slot = slots[i];
            std::string slotName = slot->getName();
            slotName.append("@PARENT_INDEX@");
            slotName.append(std::to_string(parentIndex));
            attachedSlotNames.push_back(slotName);
            
            Armature* childArmature = slot->getChildArmature();
            if (childArmature != nullptr)
            {
                traverse(childArmature, (int)attachedSlotNames.size() - 1);
            }
        }
    };
    traverse(rootArmature, -1);
    return attachedSlotNames;
}

void AttachedNode::releaseAttachedNode()
{
    _attachedSlots.clear();
    for (std::size_t i = 0, n = _attachedNodes.size(); i < n; i++)
    {
        auto node = _attachedNodes[i];
        if (node)
        {
            node->release();
        }
    }
    _attachedNodes.clear();
    
    if (_attachedRootNode)
    {
        _attachedRootNode->release();
        _attachedRootNode = nullptr;
    }
}

void AttachedNode::associateAttachedNode(Armature* rootArmature, NodeProxy* armatureNode)
{
    releaseAttachedNode();
    if (!armatureNode) return;
    
    auto rootNode = armatureNode->getChildByName("ATTACHED_NODE:ROOT");
    if (!rootNode || !rootNode->isValid()) return;
    _attachedRootNode = rootNode;
    _attachedRootNode->retain();
    _attachedRootNode->enableUpdateWorldMatrix(false);
    _attachedRootNode->switchTraverseToVisit();
    
    std::function<void(Armature*, NodeProxy*)> traverse = [&](Armature* armature, NodeProxy* parentNode) {
        if (!armature) return;
        
        auto& slots = armature->getSlots();
        for (std::size_t i = 0, n = slots.size(); i < n; i++) {
            CCSlot* slot = (CCSlot*)slots[i];
            std::string slotName = "ATTACHED_NODE:";
            slotName.append(slot->getName());
            _attachedSlots.push_back(slot);
            
            NodeProxy* slotNode = nullptr;
            if (parentNode)
            {
                slotNode = parentNode->getChildByName(slotName);
            }
            
            if (slotNode && slotNode->isValid())
            {
                slotNode->enableUpdateWorldMatrix(false);
                slotNode->retain();
            }
            else
            {
                slotNode = nullptr;
            }
            _attachedNodes.push_back(slotNode);
            
            Armature* childArmature = slot->getChildArmature();
            if (childArmature != nullptr)
            {
                traverse(childArmature, slotNode);
            }
        }
    };
    traverse(rootArmature, rootNode);
}

void RealTimeAttachedNode::syncAttachedNode(NodeProxy* armatureNode)
{
    static cocos2d::Mat4 tempWorldMat;
    if (!armatureNode || !_attachedRootNode) return;
    if (!_attachedRootNode->isValid())
    {
        _attachedRootNode->release();
        _attachedRootNode = nullptr;
        return;
    }
    
    auto& rootMatrix = armatureNode->getWorldMatrix();
    _attachedRootNode->updateWorldMatrix(rootMatrix);
    
    int lastValidIdx = -1;
    for (int i = 0, n = (int)_attachedNodes.size(); i < n; i++)
    {
        auto slotNode = _attachedNodes[i];
        if (!slotNode)
        {
            continue;
        }
        if (!slotNode->isValid())
        {
            slotNode->release();
            _attachedNodes[i] = nullptr;
            continue;
        }
        auto slot = _attachedSlots[i];
        if (!slot || slot->isInPool())
        {
            slotNode->enableVisit(false);
            slotNode->release();
            _attachedNodes[i] = nullptr;
            _attachedSlots[i] = nullptr;
            continue;
        }
        slotNode->enableVisit(slot->getVisible());
        slotNode->setLocalZOrder(slot->_zOrder);
        cocos2d::Mat4::multiply(rootMatrix, slot->worldMatrix, &tempWorldMat);
        slotNode->updateWorldMatrix(tempWorldMat);
        lastValidIdx = i;
    }
    _attachedNodes.resize(lastValidIdx + 1);
}

void CacheModeAttachedNode::syncAttachedNode(NodeProxy* armatureNode, ArmatureCache::FrameData* frameData)
{
    static cocos2d::Mat4 tempWorldMat;
    if (!armatureNode || !_attachedRootNode) return;
    if (!_attachedRootNode->isValid())
    {
        _attachedRootNode->release();
        _attachedRootNode = nullptr;
        return;
    }
    
    auto& rootMatrix = armatureNode->getWorldMatrix();
    _attachedRootNode->updateWorldMatrix(rootMatrix);
    
    auto& slotsData = frameData->getSlots();
    auto slotCount = frameData->getSlotCount();
    int lastValidIdx = -1;
    for (int i = 0, n = (int)_attachedNodes.size(); i < n; i++)
    {
        auto slotNode = _attachedNodes[i];
        if (!slotNode)
        {
            continue;
        }
        if (!slotNode->isValid())
        {
            slotNode->release();
            _attachedNodes[i] = nullptr;
            continue;
        }
        if (i >= slotCount)
        {
            slotNode->enableVisit(false);
            lastValidIdx = i;
            continue;
        }
        auto slot = slotsData[i];
        slotNode->enableVisit(slot->visible);
        slotNode->setLocalZOrder(slot->zOrder);
        cocos2d::Mat4::multiply(rootMatrix, slot->worldMatrix, &tempWorldMat);
        slotNode->updateWorldMatrix(tempWorldMat);
        lastValidIdx = i;
    }
    _attachedNodes.resize(lastValidIdx + 1);
}

DRAGONBONES_NAMESPACE_END
