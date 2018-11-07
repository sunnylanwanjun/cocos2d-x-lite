
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
#pragma once
#include <map>
#include <vector>
#include "scripting/js-bindings/jswrapper/SeApi.h"

namespace editor {
    /** 
     * TypeArray Pool
     */
    class TypeArrayPool
    {
    private:
        static TypeArrayPool* _instance;
    public:
        static TypeArrayPool* getInstance()
        {
            if (_instance == nullptr)
            {
                _instance = new TypeArrayPool();
                
                se::ScriptEngine::getInstance()->addAfterCleanupHook([&](){
                    _instance->allowPush = false;
                    _instance->clearPool();
                });
                
                se::ScriptEngine::getInstance()->addAfterInitHook([&](){
                    _instance->allowPush = true;
                });
                
            }
            return _instance;
        }
        
        static void destroyInstance()
        {
            if (_instance)
            {
                delete _instance;
                _instance = nullptr;
            }
        }
    private:
        typedef se::Object::TypedArrayType arrayType;
        typedef std::vector<se::Object*> objPool;
        typedef std::map<std::size_t, objPool*> fitMap;
        typedef std::map<arrayType, fitMap*> typeMap;
        
        objPool* getObjPool(arrayType type, std::size_t size);
        
        TypeArrayPool();
        ~TypeArrayPool();
        
        void clearPool();
        void dump();
    private:
        typeMap _pool;
    public:
        bool allowPush = true;
    public:
        se::Object* pop(arrayType type, std::size_t size);
        void push(arrayType type, std::size_t fitSize, se::Object* object);
    };
}
