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

#include "IOBuffer.h"
#include "map"
#include "vector"
#include "base/CCRef.h"

namespace editor {
    
    class IEditor {
    public:
        IEditor(){}
        virtual ~IEditor(){}
        virtual void update(float dt) = 0;
    };
    
    class EditorManager {
    public:
        static EditorManager* getInstance()
        {
            if (_instance == nullptr)
            {
                _instance = new EditorManager;
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
        
        void addTimer(IEditor* editor);
        void removeTimer(IEditor* editor);
        
        uint32_t getGLIBID()
        {
            return _glIBID;
        }
        
        uint32_t getGLVBID()
        {
            return _glVBID;
        }
        
        editor::IOBuffer vb;
        editor::IOBuffer ib;
        
        EditorManager();
        ~EditorManager();
        
        bool isUpdating = false;
        
    private:
        void update(float dt);
        void uploadVB();
        void uploadIB();
        void afterCleanupHandle();
        void afterInitHandle();
    private:
        std::map<IEditor*, bool> _updateMap;
        std::vector<IEditor*> _removeList;
        uint32_t _glIBID = 0;
        uint32_t _glVBID = 0;
        static EditorManager* _instance;
    };
}
