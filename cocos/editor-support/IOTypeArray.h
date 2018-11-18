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

#include "TypeArrayPool.h"
#include "IOBuffer.h"
#include "base/ccMacros.h"

namespace editor {
    class IOTypeArray: public IOBuffer {
    public:
        IOTypeArray (se::Object::TypedArrayType arrayType, std::size_t defaultSize)
        : _arrayType(arrayType)
        {
            _bufferSize = defaultSize;
            _typeArray = TypeArrayPool::getInstance()->pop(_arrayType, _bufferSize);
            se::AutoHandleScope hs;
            _typeArray->getTypedArrayData(&_buffer, &_bufferSize);
        }
        
        ~IOTypeArray()
        {
            TypeArrayPool::getInstance()->push(_arrayType, _bufferSize, _typeArray);
            _typeArray = nullptr;
            _buffer = nullptr;
        }
        
        inline se_object_ptr getTypeArray () const
        {
            return _typeArray;
        }
    
        virtual void resize(std::size_t newLen, bool needCopy = false) override;
    private:
        se::Object::TypedArrayType  _arrayType = se::Object::TypedArrayType::NONE;
        se::Object*                 _typeArray = nullptr;
    };
}
