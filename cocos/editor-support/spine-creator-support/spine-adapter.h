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

#include "math/Vec2.h"
#include "base/ccTypes.h"
#include "scripting/js-bindings/jswrapper/SeApi.h"
#include <functional>

namespace spine{
    /** @struct Tex2F
     * A TEXCOORD composed of 2 floats: u, y
     */
    struct Tex2F {
        GLfloat u;
        GLfloat v;
    };
    
    /** @struct V2F_T2F_C4B
     * A Vec2 with a vertex point, a tex coord point and a color 4B.
     */
    struct V2F_T2F_C4B {
    public:
        /// vertices (3F)
        cocos2d::Vec2     vertices;            // 12 bytes
        
        // tex coords (2F)
        Tex2F        texCoords;           // 8 bytes
        
        /// colors (4B)
        cocos2d::Color4B      colors;              // 4 bytes
    };
    
    struct Triangles
    {
        /**Vertex data pointer.*/
        V2F_T2F_C4B* verts;
        /**Index data pointer.*/
        unsigned short* indices;
        /**The number of vertices.*/
        int vertCount;
        /**The number of indices.*/
        int indexCount;
    };
    
    class Texture2D:public cocos2d::Ref{
    public:
        Texture2D();
        ~Texture2D();
        /**
         Extension to set the Min / Mag filter
         */
        typedef struct _TexParams {
            GLuint    minFilter;
            GLuint    magFilter;
            GLuint    wrapS;
            GLuint    wrapT;
        }TexParams;
        
        /**
         * set texture param callback
         */
        typedef std::function<void(int,GLuint,GLuint,GLuint,GLuint)> texParamCallback;
        
        /** Sets the min filter, mag filter, wrap s and wrap t texture parameters.
         If the texture size is NPOT (non power of 2), then in can only use GL_CLAMP_TO_EDGE in GL_TEXTURE_WRAP_{S,T}.
         
         @warning Calling this method could allocate additional texture memory.
         
         @since v0.8
         * @code
         * When this function bound into js or lua,the input parameter will be changed
         * In js: var setBlendFunc(var arg1, var arg2, var arg3, var arg4)
         * In lua: local setBlendFunc(local arg1, local arg2, local arg3, local arg4)
         * @endcode
         */
        void setTexParameters(const TexParams& texParams);
        
        /** Gets the width of the texture in pixels. */
        int getPixelsWide() const;
        
        /** Gets the height of the texture in pixels. */
        int getPixelsHigh() const;
        
        /** Gets real texture index */
        int getRealTextureIndex() const;
        
        /** Sets the width of the texture in pixels. */
        void setPixelsWide(int wide);
        
        /** Sets the height of the texture in pixels. */
        void setPixelsHigh(int high);
        
        /** Sets real texture index.*/
        void setRealTextureIndex(int textureIndex);
        
        /** Sets texture param callback*/
        void setTexParamCallback(const texParamCallback& callback);
    private:
        
        /** width in pixels */
        int _pixelsWide;
        
        /** height in pixels */
        int _pixelsHigh;
        
        /** js texture */
        int _realTextureIndex;
        
        texParamCallback _texParamCallback;
    };
}
