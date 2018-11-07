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
#include "TypeArrayPool.h"
#include "base/CCLog.h"
#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"
#include "base/ccMacros.h"

#define POOL_DEBUG 0

#if POOL_DEBUG > 0
#define PoolLog(...) do { fprintf(stdout, __VA_ARGS__); fflush(stdout); } while (false)
#else
#define PoolLog(...)
#endif

namespace editor{
    
    const static std::size_t maxPoolSize = 50;
    
    TypeArrayPool* TypeArrayPool::_instance = nullptr;
    
    TypeArrayPool::TypeArrayPool()
    {
        
    }
    
    TypeArrayPool::~TypeArrayPool()
    {
        clearPool();
    }
    
    void TypeArrayPool::clearPool()
    {
        #if POOL_DEBUG > 0
        dump();
        #endif
        
        se::AutoHandleScope hs;
        //map
        for (auto it = _pool.begin(); it != _pool.end(); it++)
        {
            //map
            fitMap& mapPool = *(it->second);
            for (auto itMapPool = mapPool.begin(); itMapPool != mapPool.end(); itMapPool++)
            {
                //vector
                objPool& itFitPool = *(itMapPool->second);
                for (auto itFit = itFitPool.begin(); itFit != itFitPool.end(); itFit++)
                {
                    (*itFit)->unroot();
                    (*itFit)->decRef();
                }
                delete &itFitPool;
            }
            delete &mapPool;
        }
        _pool.clear();
    }
    
    void TypeArrayPool::dump()
    {
        //map
        for (auto it = _pool.begin(); it != _pool.end(); it++)
        {
            //map
            fitMap& mapPool = *(it->second);
            for (auto itMapPool = mapPool.begin(); itMapPool != mapPool.end(); itMapPool++)
            {
                //vector
                CC_UNUSED objPool& itFitPool = *(itMapPool->second);
                PoolLog("arrayType:%d,fitSize:%lu,objSize:%lu\n", it->first, itMapPool->first, itFitPool.size());
            }
        }
    }
    
    se::Object* TypeArrayPool::pop(arrayType type, std::size_t size)
    {
        // allocate n * 4096,fit to memory each page.
        std::size_t fitSize = ceil(size / 4096.0f) * 4096;
        objPool* objPoolPtr = getObjPool(type, fitSize);
        
        if (objPoolPtr->size() > 0)
        {
            se::Object* obj = objPoolPtr->back();
            objPoolPtr->pop_back();
            PoolLog("TypeArrayPool:pop result:success,type:%d,fitSize:%lu,objSize:%lu\n", (int)type, fitSize, objPoolPtr->size());
            return obj;
        }
        
        PoolLog("TypeArrayPool:pop result:empty,type:%d,fitSize:%lu,objSize:%lu\n", (int)type, fitSize, objPoolPtr->size());
        se::AutoHandleScope hs;
        auto typeArray = se::Object::createTypedArray(type, nullptr, fitSize);
        typeArray->root();
        return typeArray;
    }
    
    TypeArrayPool::objPool* TypeArrayPool::getObjPool(arrayType type, std::size_t fitSize)
    {
        auto it = _pool.find(type);
        fitMap* fitMapPtr = nullptr;
        if (it == _pool.end())
        {
            fitMapPtr = new fitMap();
            _pool[type] = fitMapPtr;
        }
        else
        {
            fitMapPtr = it->second;
        }
        
        auto itPool = fitMapPtr->find(fitSize);
        objPool* objPoolPtr = nullptr;
        if (itPool == fitMapPtr->end())
        {
            objPoolPtr = new objPool();
            (*fitMapPtr)[fitSize] = objPoolPtr;
        }
        else
        {
            objPoolPtr = itPool->second;
        }
        
        return objPoolPtr;
    }
    
    void TypeArrayPool::push(arrayType type, std::size_t fitSize, se::Object* object)
    {
        if (object == nullptr) return;
     
        se::AutoHandleScope hs;
        
        // If script engine is cleaning,delete object directly
        if (!allowPush)
        {
            object->unroot();
            object->decRef();
            object = nullptr;
            PoolLog("TypeArrayPool:push result:not allow,type:%d,fitSize:%lu\n", (int)type, fitSize);
            return;
        }
        
        objPool* objPoolPtr = getObjPool(type, fitSize);
        auto it = std::find(objPoolPtr->begin(), objPoolPtr->end(), object);
        if (it != objPoolPtr->end())
        {
            PoolLog("TypeArrayPool:push result:repeat\n");
            return;
        }
        
        if (objPoolPtr->size() < maxPoolSize)
        {
            objPoolPtr->push_back(object);
            PoolLog("TypeArrayPool:push result:success,type:%d,fitSize:%lu,objSize:%lu\n", (int)type, fitSize, objPoolPtr->size());
        }
        else{
            object->unroot();
            object->decRef();
            object = nullptr;
        }
    }
}
