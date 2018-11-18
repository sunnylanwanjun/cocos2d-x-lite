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
#include "IOTypeArray.h"

namespace editor {
    
    void IOTypeArray::resize (std::size_t needLen)
    {
        std::size_t hasLen = _bufferSize - _curPos;
        if (hasLen < needLen)
        {
            std::size_t addLen = needLen - hasLen + 128;
            std::size_t newLen = _bufferSize + addLen;
            
            se::Object* newTypeBuffer = TypeArrayPool::getInstance()->pop(_arrayType, newLen);
            
            uint8_t* newBuffer = nullptr;
            se::AutoHandleScope hs;
            newTypeBuffer->getTypedArrayData(&newBuffer, (size_t*)&newLen);
            memcpy(newBuffer, _buffer, _bufferSize);
            
            TypeArrayPool::getInstance()->push(_arrayType, _bufferSize, _typeArray);
            
            _typeArray = newTypeBuffer;
            _buffer = newBuffer;
            _bufferSize = newLen;
            isNewBuffer = true;
        }
    }
}
