#pragma once
#include "base/Config.h"
#if USE_MIDDLEWARE > 0
#include <type_traits>
#include "cocos/bindings/jswrapper/SeApi.h"
#include "cocos/bindings/manual/jsb_conversions.h"
#include "cocos\editor-support\middleware-adapter.h"
#include "cocos\editor-support\MiddlewareManager.h"
#include "cocos\editor-support\SharedBufferManager.h"

extern se::Object* __jsb_cc_middleware_Texture2D_proto;
extern se::Class* __jsb_cc_middleware_Texture2D_class;

bool js_register_cc_middleware_Texture2D(se::Object* obj);
bool register_all_cocos2dx_editor_support(se::Object* obj);

JSB_REGISTER_OBJECT_TYPE(cc::middleware::Texture2D);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_Texture2D_getPixelsHigh);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_Texture2D_getPixelsWide);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_Texture2D_getRealTextureIndex);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_Texture2D_setPixelsHigh);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_Texture2D_setPixelsWide);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_Texture2D_setRealTextureIndex);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_Texture2D_setTexParamCallback);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_Texture2D_setTexParameters);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_Texture2D_Texture2D);

extern se::Object* __jsb_cc_middleware_SharedBufferManager_proto;
extern se::Class* __jsb_cc_middleware_SharedBufferManager_class;

bool js_register_cc_middleware_SharedBufferManager(se::Object* obj);
bool register_all_cocos2dx_editor_support(se::Object* obj);

JSB_REGISTER_OBJECT_TYPE(cc::middleware::SharedBufferManager);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_SharedBufferManager_getSharedBuffer);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_SharedBufferManager_setResizeCallback);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_SharedBufferManager_SharedBufferManager);

extern se::Object* __jsb_cc_middleware_MiddlewareManager_proto;
extern se::Class* __jsb_cc_middleware_MiddlewareManager_class;

bool js_register_cc_middleware_MiddlewareManager(se::Object* obj);
bool register_all_cocos2dx_editor_support(se::Object* obj);

JSB_REGISTER_OBJECT_TYPE(cc::middleware::MiddlewareManager);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_MiddlewareManager_getAttachInfoMgr);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_MiddlewareManager_getBufferCount);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_MiddlewareManager_getIBTypedArray);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_MiddlewareManager_getIBTypedArrayLength);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_MiddlewareManager_getRenderInfoMgr);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_MiddlewareManager_getVBTypedArray);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_MiddlewareManager_getVBTypedArrayLength);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_MiddlewareManager_render);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_MiddlewareManager_update);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_MiddlewareManager_destroyInstance);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_MiddlewareManager_generateModuleID);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_MiddlewareManager_getInstance);
SE_DECLARE_FUNC(js_cocos2dx_editor_support_MiddlewareManager_MiddlewareManager);

#endif //#if USE_MIDDLEWARE > 0
