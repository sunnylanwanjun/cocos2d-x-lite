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
#include "IOBuffer.h"

namespace editor {

    IOBuffer::IOBuffer (se::Object::TypedArrayType arrayType, std::size_t defaultSize)
    : _arrayType(arrayType)
    , _bufferSize(defaultSize)
    {
        _typeArray = TypeArrayPool::getInstance()->pop(_arrayType, _bufferSize);
        _typeArray->getTypedArrayData(&_buffer, &_bufferSize);
    }

    IOBuffer::~IOBuffer ()
    {
        TypeArrayPool::getInstance()->push(_arrayType, _bufferSize, _typeArray);
        _typeArray = nullptr;
    }

    //will write uint32_t in any pos,but can not out of range
    void IOBuffer::writeUint32 (std::size_t pos, uint32_t val)
    {
        if (_bufferSize < pos + sizeof(val))
        {
            CCLOG("se::IOBuffer writeUint32 has out of range");
            return;
        }
        uint32_t* buffer = (uint32_t*)(_buffer + pos);
        *buffer = val;
        return;
    }

    void IOBuffer::writeBytes (const char* bytes, std::size_t bytesLen)
    {
        checkSpace(bytesLen);
        memcpy(_buffer + _curPos, bytes, bytesLen);
        _curPos += bytesLen;
    }

    void IOBuffer::writeUint32 (uint32_t val) {
        checkSpace(sizeof(val));
        uint32_t* buffer = (uint32_t*)(_buffer + _curPos);
        *buffer = val;
        _curPos += sizeof(val);
    }

    void IOBuffer::writeFloat32 (std::size_t pos, float val)
    {
        if (_bufferSize < pos + sizeof(val))
        {
            CCLOG("se::IOBuffer writeFloat32 has out of range");
            return;
        }
        float* buffer = (float*)(_buffer + pos);
        *buffer = val;
        return;
    }

    void IOBuffer::writeFloat32 (float val)
    {
        checkSpace(sizeof(val));
        float* buffer = (float*)(_buffer + _curPos);
        *buffer = val;
        _curPos += sizeof(val);
        return;
    }

    void IOBuffer::writeUint16 (uint16_t val)
    {
        checkSpace(sizeof(val));
        uint16_t* buffer = (uint16_t*)(_buffer + _curPos);
        *buffer = val;
        _curPos += sizeof(val);
        return;
    }

    uint32_t IOBuffer::readUint32 ()
    {
        uint32_t* buffer = (uint32_t*)(_buffer + _readPos);
        _readPos += sizeof(uint32_t);
        return *buffer;
    }

    uint16_t IOBuffer::readUint16 ()
    {
        uint16_t* buffer = (uint16_t*)(_buffer + _readPos);
        _readPos += sizeof(uint16_t);
        return *buffer;
    }

    float IOBuffer::readFloat32 ()
    {
        float* buffer = (float*)(_buffer + _readPos);
        _readPos += sizeof(float);
        return *buffer;
    }

    char IOBuffer::readUint8 ()
    {
        char* buffer = (char*)(_buffer + _readPos);
        _readPos += sizeof(char);
        return *buffer;
    }

    void IOBuffer::reset ()
    {
        _curPos = 0;
        _readPos = 0;
    }

    se_object_ptr IOBuffer::getTypeArray () const
    {
        return _typeArray;
    }

    std::size_t IOBuffer::length () const
    {
        return _curPos;
    }

    std::size_t IOBuffer::getCurPos () const
    {
        return _curPos;
    }

    void IOBuffer::checkSpace (std::size_t needLen)
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
