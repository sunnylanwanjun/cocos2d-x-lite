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
#include "base/ccMacros.h"
#include <cstddef>
#include <cstdint>
#include <string>

namespace editor {
    
    /**
     * IOBuffer write/read bytes with js array buffer.
     */
    class IOBuffer
    {
    public:
        IOBuffer (std::size_t defaultSize)
        : _bufferSize(defaultSize)
        {
            _buffer = new uint8_t[_bufferSize];
        }
        
        IOBuffer ()
        {
            
        }
        
        virtual ~IOBuffer ()
        {
            if (_buffer)
            {
                delete[] _buffer;
                _buffer = nullptr;
            }
        }
        
        inline void writeUint32 (std::size_t pos, uint32_t val)
        {
            if (_bufferSize < pos + sizeof(val)) return;
            uint32_t* buffer = (uint32_t*)(_buffer + pos);
            *buffer = val;
        }
        
        inline void writeFloat32 (std::size_t pos, float val)
        {
            if (_bufferSize < pos + sizeof(val)) return;
            float* buffer = (float*)(_buffer + pos);
            *buffer = val;
        }
        
        inline void writeBytes (const char* bytes, std::size_t bytesLen)
        {
            if (_bufferSize < _curPos + bytesLen) return;
            memcpy(_buffer + _curPos, bytes, bytesLen);
            _curPos += bytesLen;
        }
        
        inline void writeUint32 (uint32_t val) {
            if (_bufferSize < _curPos + sizeof(val)) return;
            uint32_t* buffer = (uint32_t*)(_buffer + _curPos);
            *buffer = val;
            _curPos += sizeof(val);
        }
        
        inline void writeFloat32 (float val)
        {
            if (_bufferSize < _curPos + sizeof(val)) return;
            float* buffer = (float*)(_buffer + _curPos);
            *buffer = val;
            _curPos += sizeof(val);
        }
        
        inline void writeUint16 (uint16_t val)
        {
            if (_bufferSize < _curPos + sizeof(val)) return;
            uint16_t* buffer = (uint16_t*)(_buffer + _curPos);
            *buffer = val;
            _curPos += sizeof(val);
        }
        
        inline uint32_t readUint32 ()
        {
            uint32_t* buffer = (uint32_t*)(_buffer + _readPos);
            _readPos += sizeof(uint32_t);
            return *buffer;
        }
        
        inline uint16_t readUint16 ()
        {
            uint16_t* buffer = (uint16_t*)(_buffer + _readPos);
            _readPos += sizeof(uint16_t);
            return *buffer;
        }
        
        inline float readFloat32 ()
        {
            float* buffer = (float*)(_buffer + _readPos);
            _readPos += sizeof(float);
            return *buffer;
        }
        
        inline char readUint8 ()
        {
            char* buffer = (char*)(_buffer + _readPos);
            _readPos += sizeof(char);
            return *buffer;
        }
        
        inline void reset ()
        {
            _curPos = 0;
            _readPos = 0;
        }
        
        inline std::size_t length() const
        {
            return _curPos;
        }

        inline std::size_t getCurPos() const
        {
            return _curPos;
        }

        inline uint8_t* getBuffer() const
        {
            return _buffer;
        }
    protected:
        virtual void resize(std::size_t needLen);
    protected:
        uint8_t*                    _buffer = nullptr;
        std::size_t                 _bufferSize = 0;
        std::size_t                 _curPos = 0;
        std::size_t                 _readPos = 0;
    };
}
