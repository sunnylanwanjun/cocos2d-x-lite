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
#include "scripting/js-bindings/jswrapper/Object.hpp"
#include <vector>
#include "base/ccMacros.h"

namespace editor {
    /**
     * IOBuffer write/read bytes with js array buffer.
     */
    class IOBuffer
    {
    public:
        /**
         *  @brief IOBuffer constructor.
         *  @param[in] arrayType The type array will be create.
         */
        IOBuffer(se::Object::TypedArrayType arrayType);
        /**
         *  @brief IOBuffer destructor.
         */
        ~IOBuffer();
        /**
         *  @brief Write float into buffer.
         *  @param[in] pos The buffer pos begin to write.
         *  @param[in] val The value write into buffer.
         */
        void writeFloat32(int pos,float val);
        /**
         *  @brief Write uint32 into buffer.
         *  @param[in] pos The buffer pos begin to write.
         *  @param[in] val The value write into buffer.
         */
        void writeUint32(int pos,uint32_t val);
        /**
         *  @brief Write bytes into buffer.
         *  @param[in] bytes A data pointer,the data will be copy.
         *  @param[in] bytesLen The length of the data.
         */
        void writeBytes(const char* bytes,int bytesLen);
        /**
         *  @brief Write uint32 into buffer.
         *  @param[in] val The value write into buffer.
         */
        void writeUint32(uint32_t val);
        /**
         *  @brief Write float into buffer.
         *  @param[in] val The value write into buffer.
         */
        void writeFloat32(float val);
        /**
         *  @brief Write uint16_t into buffer.
         *  @param[in] val The value write into buffer.
         */
        void writeUint16(uint16_t val);

        /**
         *  @brief Get uint32 from buffer.
         *  @return The value read from buffer.
         */
        uint32_t readUint32();
        /**
         *  @brief Get uint16 from buffer.
         *  @return The value read from buffer.
         */
        uint16_t readUint16();
        /**
         *  @brief Get float32 from buffer.
         *  @return The value read from buffer.
         */
        float readFloat32();
        /**
         *  @brief Get uint8 from buffer.
         *  @return The value read from buffer.
         */
        char readUint8();
        /**
         *  @brief Reset buffer write/read pointer.
         */
        void reset();
        /**
         *  @brief Get JS type array.
         */
        se::Object* getTypeArray() const;
        /**
         *  @brief Get length of buffer.
         */
        int length() const;
        /**
         *  @brief Get current write position.
         */
        int getCurPos() const;
    private:
        /**
         * If js array address change,will return false,or return true.
         */
        void checkSpace(int needLen);
    private:
        se::Object::TypedArrayType  _arrayType = se::Object::TypedArrayType::NONE;
        uint8_t*                    _buffer = nullptr;
        int                         _bufferSize = 40960;
        int                         _curPos = 0;
        int                         _readPos = 0;
        se::Object*                 _typeArray = nullptr;
    public:
        bool                        isNewBuffer = false;
    };
}
