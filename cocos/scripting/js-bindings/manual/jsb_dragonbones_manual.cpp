/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos.com

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated engine source code (the "Software"), a limited,
 worldwide, royalty-free, non-assignable, revocable and non-exclusive license
 to use Cocos Creator solely to develop games on your target platforms. You shall
 not use Cocos Creator software for developing other software or tools that's
 used for developing games. You are not granted to publish, distribute,
 sublicense, and/or sell copies of Cocos Creator.

 The software or tools in this License Agreement are licensed, not sold.
 Xiamen Yaji Software Co., Ltd. reserves all rights not expressly granted to you.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "jsb_dragonbones_manual.hpp"
#include "cocos/scripting/js-bindings/jswrapper/SeApi.h"
#include "cocos/scripting/js-bindings/manual/jsb_conversions.hpp"
#include "cocos/scripting/js-bindings/manual/jsb_global.h"
#include "cocos/scripting/js-bindings/manual/jsb_helper.hpp"
#include "cocos/scripting/js-bindings/auto/jsb_cocos2dx_dragonbones_auto.hpp"
#include "cocos/editor-support/dragonBones-creator-support/CCDragonBonesHeaders.h"

#include "cocos2d.h"

using namespace cocos2d;

static bool js_cocos2dx_dragonbones_Armature_getAnimation(se::State& s)
{
    if (s.args().size() == 0)
    {
        dragonBones::Armature* cobj = (dragonBones::Armature*)s.nativeThisObject();
        dragonBones::Animation* ret = cobj->getAnimation();
        bool ok = native_ptr_to_rooted_seval<dragonBones::Animation>(ret, __jsb_dragonBones_Animation_class, &s.rval());
        SE_PRECONDITION2(ok, false, "Convert dragonBones::Animation to se::Value failed!");
        return true;
    }

    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)s.args().size(), 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_dragonbones_Armature_getAnimation)

static bool js_cocos2dx_dragonbones_Armature_getArmatureData(se::State& s)
{
    if (s.args().size() == 0)
    {
        dragonBones::Armature* cobj = (dragonBones::Armature*)s.nativeThisObject();
        const dragonBones::ArmatureData* ret = cobj->getArmatureData();
        bool ok = native_ptr_to_rooted_seval<dragonBones::ArmatureData>((dragonBones::ArmatureData*)ret, __jsb_dragonBones_ArmatureData_class, &s.rval());
        SE_PRECONDITION2(ok, false, "Convert dragonBones::ArmatureData to se::Value failed!");
        return true;
    }

    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)s.args().size(), 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_dragonbones_Armature_getArmatureData)

static bool js_cocos2dx_dragonbones_Armature_getDisplay(se::State& s)
{
    if (s.args().size() == 0)
    {
        dragonBones::Armature* cobj = (dragonBones::Armature*)s.nativeThisObject();
        dragonBones::CCArmatureDisplay* ret = dynamic_cast<dragonBones::CCArmatureDisplay*>(cobj->getDisplay());
        if (ret != nullptr)
        {
            bool ok = native_ptr_to_seval<dragonBones::CCArmatureDisplay>(ret, __jsb_dragonBones_CCArmatureDisplay_class, &s.rval());
            SE_PRECONDITION2(ok, false, "Convert dragonBones::Animation to se::Value failed!");
        }
        else
        {
            s.rval().setNull();
        }
        return true;
    }

    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)s.args().size(), 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_dragonbones_Armature_getDisplay)

static bool js_cocos2dx_dragonbones_Armature_getSlots(se::State& s)
{
	dragonBones::Armature* cobj = (dragonBones::Armature*)s.nativeThisObject();
	const auto& result = cobj->getSlots();
	se::HandleObject arr(se::Object::createArrayObject(result.size()));

	uint32_t i = 0;
	se::Value tmp;
	bool ok = true;
	for (const auto& slot : result)
	{
		if (!native_ptr_to_rooted_seval<dragonBones::Slot>(slot, &tmp))
		{
			ok = false;
			break;
		}
		arr->setArrayElement(i, tmp);
		++i;
	}
	if (ok)
		s.rval().setObject(arr);

	SE_PRECONDITION2(ok, false, "Convert getSlots to se::Value failed!");
	return true;
}
SE_BIND_FUNC(js_cocos2dx_dragonbones_Armature_getSlots)

static bool js_cocos2dx_dragonbones_CCArmatureDisplay_getAnimation(se::State& s)
{
    if (s.args().size() == 0)
    {
        dragonBones::CCArmatureDisplay* cobj = (dragonBones::CCArmatureDisplay*)s.nativeThisObject();
        dragonBones::Animation* ret = cobj->getAnimation();
        bool ok = native_ptr_to_rooted_seval<dragonBones::Animation>(ret, __jsb_dragonBones_Animation_class, &s.rval());
        SE_PRECONDITION2(ok, false, "Convert dragonBones::Animation to se::Value failed!");
        return true;
    }

    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)s.args().size(), 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_dragonbones_CCArmatureDisplay_getAnimation)

static bool js_cocos2dx_dragonbones_AnimationState_getAnimationData(se::State& s)
{
    if (s.args().size() == 0)
    {
        dragonBones::AnimationState* cobj = (dragonBones::AnimationState*)s.nativeThisObject();
        const dragonBones::AnimationData* ret = cobj->getAnimationData();
        bool ok = native_ptr_to_rooted_seval<dragonBones::AnimationData>((dragonBones::AnimationData*)ret, __jsb_dragonBones_AnimationData_class, &s.rval());
        SE_PRECONDITION2(ok, false, "Convert dragonBones::AnimationData to se::Value failed!");
        return true;
    }

    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)s.args().size(), 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_dragonbones_AnimationState_getAnimationData)

static bool js_cocos2dx_dragonbones_ArmatureData_get_animations(se::State& s)
{
    dragonBones::ArmatureData* cobj = (dragonBones::ArmatureData*)s.nativeThisObject();
    se::HandleObject retObj(se::Object::createPlainObject());
    bool ok = false;
    se::Value tmp;
    for (const auto& e : cobj->animations)
    {
        if (!e.first.empty())
        {
            ok = native_ptr_to_rooted_seval<dragonBones::AnimationData>(e.second, __jsb_dragonBones_AnimationData_class, &tmp);
            SE_PRECONDITION2(ok, false, "Convert dragonBones::AnimationData to se::Value failed!");
            retObj->setProperty(e.first.c_str(), tmp);
        }
    }
    s.rval().setObject(retObj);
    return true;
}
SE_BIND_PROP_GET(js_cocos2dx_dragonbones_ArmatureData_get_animations)

static bool js_cocos2dx_dragonbones_ArmatureData_get_bones(se::State& s)
{
    dragonBones::ArmatureData* cobj = (dragonBones::ArmatureData*)s.nativeThisObject();
    se::HandleObject retObj(se::Object::createPlainObject());
    bool ok = false;
    se::Value tmp;
    for (const auto& e : cobj->bones)
    {
        if (!e.first.empty())
        {
            ok = native_ptr_to_rooted_seval<dragonBones::BoneData>(e.second, __jsb_dragonBones_BoneData_class, &tmp);
            SE_PRECONDITION2(ok, false, "Convert dragonBones::AnimationData to se::Value failed!");
            retObj->setProperty(e.first.c_str(), tmp);
        }
    }
    s.rval().setObject(retObj);
    return true;
}
SE_BIND_PROP_GET(js_cocos2dx_dragonbones_ArmatureData_get_bones)

static bool js_cocos2dx_dragonbones_ArmatureData_get_skins(se::State& s)
{
    dragonBones::ArmatureData* cobj = (dragonBones::ArmatureData*)s.nativeThisObject();
    se::HandleObject retObj(se::Object::createPlainObject());
    bool ok = false;
    se::Value tmp;
    for (const auto& e : cobj->skins)
    {
        if (!e.first.empty())
        {
            ok = native_ptr_to_rooted_seval<dragonBones::SkinData>(e.second, __jsb_dragonBones_SkinData_class, &tmp);
            SE_PRECONDITION2(ok, false, "Convert dragonBones::AnimationData to se::Value failed!");
            retObj->setProperty(e.first.c_str(), tmp);
        }
    }
    s.rval().setObject(retObj);
    return true;
}
SE_BIND_PROP_GET(js_cocos2dx_dragonbones_ArmatureData_get_skins)

static bool js_cocos2dx_dragonbones_ArmatureData_get_slots(se::State& s)
{
    dragonBones::ArmatureData* cobj = (dragonBones::ArmatureData*)s.nativeThisObject();
    se::HandleObject retObj(se::Object::createPlainObject());
    bool ok = false;
    se::Value tmp;
    for (const auto& e : cobj->slots)
    {
        if (!e.first.empty())
        {
            ok = native_ptr_to_rooted_seval<dragonBones::SlotData>(e.second, __jsb_dragonBones_SlotData_class, &tmp);
            SE_PRECONDITION2(ok, false, "Convert dragonBones::AnimationData to se::Value failed!");
            retObj->setProperty(e.first.c_str(), tmp);
        }
    }
    s.rval().setObject(retObj);
    return true;
}
SE_BIND_PROP_GET(js_cocos2dx_dragonbones_ArmatureData_get_slots)

static bool js_cocos2dx_dragonbones_DragonBonesData_get_armatureNames(se::State& s)
{
    dragonBones::DragonBonesData* cobj = (dragonBones::DragonBonesData*)s.nativeThisObject();

    const auto& ret = cobj->getArmatureNames();
    bool ok = std_vector_string_to_seval(ret, &s.rval());
    SE_PRECONDITION2(ok, false, "Convert ArmatureNames to se::Value failed!");
    return true;
}
SE_BIND_PROP_GET(js_cocos2dx_dragonbones_DragonBonesData_get_armatureNames)

static bool js_cocos2dx_dragonbones_WorldClock_add(se::State& s)
{
    const auto& args = s.args();
    int argc = (int)args.size();

    if (argc == 1)
    {
        dragonBones::WorldClock* cobj = (dragonBones::WorldClock *)s.nativeThisObject();
        const se::Value& arg0Val = args[0];
        if (!arg0Val.isNull())
        {
            dragonBones::IAnimatable* arg0 = nullptr;
            bool ok = arg0Val.isObject();
            SE_PRECONDITION2(ok, false, "WorldClock_add argument isn't object");

            void* animateable = nullptr;
            ok = seval_to_native_ptr(arg0Val, &animateable);
            SE_PRECONDITION2(ok, false, "WorldClock_add argument get native ptr failed!");

            auto armatureObj = static_cast<dragonBones::Armature*>(animateable);
            arg0 = dynamic_cast<dragonBones::Armature*>(armatureObj);
            if (arg0 == nullptr)
            {
                auto clockObj = static_cast<dragonBones::WorldClock*>(animateable);
                arg0 = dynamic_cast<dragonBones::WorldClock*>(clockObj);
            }

            cobj->add(arg0);
        }

        return true;
    }

    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_dragonbones_WorldClock_add)

static bool js_cocos2dx_dragonbones_WorldClock_remove(se::State& s)
{
    const auto& args = s.args();
    int argc = (int)args.size();

    if (argc == 1)
    {
        dragonBones::WorldClock* cobj = (dragonBones::WorldClock *)s.nativeThisObject();
        const se::Value& arg0Val = args[0];
        if (!arg0Val.isNull())
        {
            dragonBones::IAnimatable* arg0 = nullptr;
            bool ok = arg0Val.isObject();
            SE_PRECONDITION2(ok, false, "WorldClock_remove argument isn't object");

            void* animateable = nullptr;
            ok = seval_to_native_ptr(arg0Val, &animateable);
            SE_PRECONDITION2(ok, false, "WorldClock_remove argument get native ptr failed!");

            auto armatureObj = static_cast<dragonBones::Armature*>(animateable);
            arg0 = dynamic_cast<dragonBones::Armature*>(armatureObj);
            if (arg0 == nullptr)
            {
                auto clockObj = static_cast<dragonBones::WorldClock*>(animateable);
                arg0 = dynamic_cast<dragonBones::WorldClock*>(clockObj);
            }

            cobj->remove(arg0);
        }

        return true;
    }

    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_dragonbones_WorldClock_remove)

static bool js_cocos2dx_dragonbones_CCFactory_getFactory(se::State& s)
{
    if (s.args().size() == 0)
    {
        auto ret = dragonBones::CCFactory::getFactory();
        bool ok = native_ptr_to_rooted_seval<dragonBones::CCFactory>(ret, __jsb_dragonBones_CCFactory_class, &s.rval());
        SE_PRECONDITION2(ok, false, "Convert dragonBones::CCFactory to se::Value failed!");
        return true;
    }

    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)s.args().size(), 0);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_dragonbones_CCFactory_getFactory)

static bool js_cocos2dx_dragonbones_TransformObject_getGlobal(se::State& s)
{
    dragonBones::TransformObject* cobj = (dragonBones::TransformObject *)s.nativeThisObject();
    bool isReturnCached = false;
    bool ok = native_ptr_to_rooted_seval<dragonBones::Transform>(&cobj->global, __jsb_dragonBones_Transform_class, &s.rval(), &isReturnCached);
    SE_PRECONDITION2(ok, false, "Convert dragonBones::Transform to se::Value failed!");
    if (!isReturnCached)
    {
        s.thisObject()->attachObject(s.rval().toObject());
    }
    return true;
}
SE_BIND_PROP_GET(js_cocos2dx_dragonbones_TransformObject_getGlobal)

static bool js_cocos2dx_dragonbones_TransformObject_getOrigin(se::State& s)
{
    dragonBones::TransformObject* cobj = (dragonBones::TransformObject *)s.nativeThisObject();
    bool isReturnCached = false;
    bool ok = native_ptr_to_rooted_seval<dragonBones::Transform>(&cobj->origin, __jsb_dragonBones_Transform_class, &s.rval(), &isReturnCached);
    SE_PRECONDITION2(ok, false, "Convert dragonBones::Transform to se::Value failed!");
    if (!isReturnCached)
    {
        s.thisObject()->attachObject(s.rval().toObject());
    }
    return true;
}
SE_BIND_PROP_GET(js_cocos2dx_dragonbones_TransformObject_getOrigin)

static bool js_cocos2dx_dragonbones_TransformObject_getOffset(se::State& s)
{
    dragonBones::TransformObject* cobj = (dragonBones::TransformObject *)s.nativeThisObject();
    bool isReturnCached = false;
    bool ok = native_ptr_to_rooted_seval<dragonBones::Transform>(&cobj->offset, __jsb_dragonBones_Transform_class, &s.rval(), &isReturnCached);
    SE_PRECONDITION2(ok, false, "Convert dragonBones::Transform to se::Value failed!");
    if (!isReturnCached)
    {
        s.thisObject()->attachObject(s.rval().toObject());
    }
    return true;
}
SE_BIND_PROP_GET(js_cocos2dx_dragonbones_TransformObject_getOffset)

static bool js_cocos2dx_dragonbones_Slot_getRawDisplay(se::State& s)
{
    CCLOGERROR("%s","Slot getRawDisplay is not support in native!");
    return false;
}
SE_BIND_FUNC(js_cocos2dx_dragonbones_Slot_getRawDisplay)

static bool js_cocos2dx_dragonbones_Slot_getDisplay(se::State& s)
{
    dragonBones::Slot* cobj = (dragonBones::Slot *)s.nativeThisObject();
    dragonBones::CCArmatureDisplay* ret = static_cast<dragonBones::CCArmatureDisplay*>(cobj->getDisplay());
    bool ok = native_ptr_to_seval<dragonBones::DBCCSprite>(ret, __jsb_dragonBones_DBCCSprite_class, &s.rval());
    SE_PRECONDITION2(ok, false, "Convert dragonBones::DBCCSprite to se::Value failed!");
    return true;
}
SE_BIND_FUNC(js_cocos2dx_dragonbones_Slot_getDisplay)

static bool js_cocos2dx_dragonbones_Slot_getMeshDisplay(se::State& s)
{
    CCLOGERROR("%s","Slot getMeshDisplay is not support in native!");
    return false;
}
SE_BIND_FUNC(js_cocos2dx_dragonbones_Slot_getMeshDisplay)

static bool js_cocos2dx_dragonbones_Slot_setDisplay(se::State& s)
{
    const auto& args = s.args();
    int argc = (int)args.size();

    if (argc == 2)
    {
        dragonBones::Slot* cobj = (dragonBones::Slot *)s.nativeThisObject();
        dragonBones::DBCCSprite* dbSprite = nullptr;
        bool ok = seval_to_native_ptr(args[0], &dbSprite);
        SE_PRECONDITION2(ok, false, "Convert se::Value to dragonBones::DBCCSprite failed!");
        dragonBones::DisplayType type;
        ok = seval_to_int32(args[1], (int32_t *)&type);
        SE_PRECONDITION2(ok, false, "Convert se::Value to dragonBones::DisplayType failed!");
        cobj->setDisplay(dbSprite, type);
        return true;
    }

    SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_dragonbones_Slot_setDisplay)

static bool js_cocos2dx_dragonbones_Slot_get_name(se::State& s)
{
	dragonBones::Slot* cobj = (dragonBones::Slot*)s.nativeThisObject();
	SE_PRECONDITION2(cobj, false, "js_cocos2dx_dragonbones_Slot_get_name : Invalid Native Object");

	CC_UNUSED bool ok = true;
	se::Value jsret;
	ok &= std_string_to_seval(cobj->name, &jsret);
	s.rval() = jsret;
	return true;
}
SE_BIND_PROP_GET(js_cocos2dx_dragonbones_Slot_get_name)

static bool js_cocos2dx_dragonbones_BaseFactory_replaceSlotDisplay(se::State& s)
{
	dragonBones::BaseFactory* cobj = (dragonBones::BaseFactory*)s.nativeThisObject();
	SE_PRECONDITION2(cobj, false, "js_cocos2dx_dragonbones_BaseFactory_replaceSlotDisplay : Invalid Native Object");
	const auto& args = s.args();
	size_t argc = args.size();
	CC_UNUSED bool ok = true;
	if (argc == 5) {
		std::string arg0;
		std::string arg1;
		std::string arg2;
		std::string arg3;
		dragonBones::Slot* arg4 = nullptr;
		ok &= seval_to_std_string(args[0], &arg0);
		ok &= seval_to_std_string(args[1], &arg1);
		ok &= seval_to_std_string(args[2], &arg2);
		ok &= seval_to_std_string(args[3], &arg3);
		ok &= seval_to_native_ptr(args[4], &arg4);
		SE_PRECONDITION2(ok, false, "js_cocos2dx_dragonbones_BaseFactory_replaceSlotDisplay : Error processing arguments");
		cobj->replaceSlotDisplay(arg0, arg1, arg2, arg3, *arg4);
		return true;
	}
	if (argc == 6) {
		std::string arg0;
		std::string arg1;
		std::string arg2;
		std::string arg3;
		dragonBones::Slot* arg4 = nullptr;
		int arg5 = -1;
		ok &= seval_to_std_string(args[0], &arg0);
		ok &= seval_to_std_string(args[1], &arg1);
		ok &= seval_to_std_string(args[2], &arg2);
		ok &= seval_to_std_string(args[3], &arg3);
		ok &= seval_to_native_ptr(args[4], &arg4);
		ok &= seval_to_int32(args[5], &arg5);
		SE_PRECONDITION2(ok, false, "js_cocos2dx_dragonbones_BaseFactory_replaceSlotDisplay : Error processing arguments");
		cobj->replaceSlotDisplay(arg0, arg1, arg2, arg3, *arg4, arg5);
		return true;
	}
	SE_REPORT_ERROR("wrong number of arguments: %d, was expecting %d", (int)argc, 6);
	return false;
}
SE_BIND_FUNC(js_cocos2dx_dragonbones_BaseFactory_replaceSlotDisplay)

static bool js_cocos2dx_dragonbones_CCFactory_parseTextureAtlasData(se::State& s)
{
    CC_UNUSED bool ok = true;
    dragonBones::CCFactory* cobj = (dragonBones::CCFactory*)s.nativeThisObject();
    SE_PRECONDITION2(cobj, false, "js_cocos2dx_dragonbones_CCFactory_parseTextureAtlasData : Invalid Native Object");
    const auto& args = s.args();
    size_t argc = args.size();

    if (argc == 2) {
        std::string arg0;
        ok &= seval_to_std_string(args[0], &arg0);
        if (!ok) { ok = true; return false; }

        if (se::Value(args[1]).isString()) {
            std::string arg1;
            ok &= seval_to_std_string(args[1], &arg1);
            if (!ok) { ok = true; return false; }
            dragonBones::TextureAtlasData* result = cobj->parseTextureAtlasData(arg0, arg1);
            ok &= native_ptr_to_rooted_seval<dragonBones::TextureAtlasData>((dragonBones::TextureAtlasData*)result, &s.rval());
        }
        else if (se::Value(args[1]).isObject()) {
            cocos2d::Texture2D *arg1;
            ok &= seval_to_native_ptr(args[1], &arg1);
            if (!ok) { ok = true; return false; }
            dragonBones::TextureAtlasData* result = cobj->parseTextureAtlasData(arg0, arg1);
            ok &= native_ptr_to_rooted_seval<dragonBones::TextureAtlasData>((dragonBones::TextureAtlasData*)result, &s.rval());
        }
        SE_PRECONDITION2(ok, false, "js_cocos2dx_dragonbones_CCFactory_parseTextureAtlasData : Error processing arguments");
        return true;
    }

    if (argc == 3) {
        std::string arg0;
        ok &= seval_to_std_string(args[0], &arg0);
        if (!ok) { ok = true; return false; }
        std::string arg2;
        ok &= seval_to_std_string(args[2], &arg2);
        if (!ok) { ok = true; return false; }

        if (se::Value(args[1]).isString()) {
            std::string arg1;
            ok &= seval_to_std_string(args[1], &arg1);
            if (!ok) { ok = true; return false; }
            dragonBones::TextureAtlasData* result = cobj->parseTextureAtlasData(arg0, arg1, arg2);
            ok &= native_ptr_to_rooted_seval<dragonBones::TextureAtlasData>((dragonBones::TextureAtlasData*)result, &s.rval());
        }
        else if (se::Value(args[1]).isObject()) {
            cocos2d::Texture2D *arg1;
            ok &= seval_to_native_ptr(args[1], &arg1);
            if (!ok) { ok = true; return false; }
            dragonBones::TextureAtlasData* result = cobj->parseTextureAtlasData(arg0, arg1, arg2);
            ok &= native_ptr_to_rooted_seval<dragonBones::TextureAtlasData>((dragonBones::TextureAtlasData*)result, &s.rval());
        }
        SE_PRECONDITION2(ok, false, "js_cocos2dx_dragonbones_CCFactory_parseTextureAtlasData : Error processing arguments");
        return true;
    }

    if (argc == 4) {
        std::string arg0;
        ok &= seval_to_std_string(args[0], &arg0);
        if (!ok) { ok = true; return false; }
        std::string arg2;
        ok &= seval_to_std_string(args[2], &arg2);
        if (!ok) { ok = true; return false; }
        float arg3 = 0;
        ok &= seval_to_float(args[3], &arg3);
        if (!ok) { ok = true; return false; }

        if (se::Value(args[1]).isString()) {
            std::string arg1;
            ok &= seval_to_std_string(args[1], &arg1);
            if (!ok) { ok = true; return false; }
            dragonBones::TextureAtlasData* result = cobj->parseTextureAtlasData(arg0, arg1, arg2, arg3);
            ok &= native_ptr_to_rooted_seval<dragonBones::TextureAtlasData>((dragonBones::TextureAtlasData*)result, &s.rval());
        }
        else if (se::Value(args[1]).isObject()) {
            cocos2d::Texture2D *arg1;
            ok &= seval_to_native_ptr(args[1], &arg1);
            if (!ok) { ok = true; return false; }
            dragonBones::TextureAtlasData* result = cobj->parseTextureAtlasData(arg0, arg1, arg2, arg3);
            ok &= native_ptr_to_rooted_seval<dragonBones::TextureAtlasData>((dragonBones::TextureAtlasData*)result, &s.rval());
        }
        SE_PRECONDITION2(ok, false, "js_cocos2dx_dragonbones_CCFactory_parseTextureAtlasData : Error processing arguments");
        return true;
    }

    SE_REPORT_ERROR("wrong number of arguments: %d", (int)argc);
    return false;
}
SE_BIND_FUNC(js_cocos2dx_dragonbones_CCFactory_parseTextureAtlasData)

bool register_all_dragonbones_manual(se::Object* obj)
{
    __jsb_dragonBones_Armature_proto->defineFunction("getAnimation", _SE(js_cocos2dx_dragonbones_Armature_getAnimation));
    __jsb_dragonBones_Armature_proto->defineFunction("getArmatureData", _SE(js_cocos2dx_dragonbones_Armature_getArmatureData));
    __jsb_dragonBones_Armature_proto->defineFunction("getDisplay", _SE(js_cocos2dx_dragonbones_Armature_getDisplay));
    __jsb_dragonBones_Armature_proto->defineFunction("getSlots", _SE(js_cocos2dx_dragonbones_Armature_getSlots));

    __jsb_dragonBones_CCArmatureDisplay_proto->defineFunction("getAnimation", _SE(js_cocos2dx_dragonbones_CCArmatureDisplay_getAnimation));
    __jsb_dragonBones_AnimationState_proto->defineFunction("getAnimationData", _SE(js_cocos2dx_dragonbones_AnimationState_getAnimationData));

    __jsb_dragonBones_ArmatureData_proto->defineProperty("animations", _SE(js_cocos2dx_dragonbones_ArmatureData_get_animations), nullptr);
    __jsb_dragonBones_ArmatureData_proto->defineProperty("bones", _SE(js_cocos2dx_dragonbones_ArmatureData_get_bones), nullptr);
    __jsb_dragonBones_ArmatureData_proto->defineProperty("skins", _SE(js_cocos2dx_dragonbones_ArmatureData_get_skins), nullptr);
    __jsb_dragonBones_ArmatureData_proto->defineProperty("slots", _SE(js_cocos2dx_dragonbones_ArmatureData_get_slots), nullptr);

    __jsb_dragonBones_DragonBonesData_proto->defineProperty("armatureNames", _SE(js_cocos2dx_dragonbones_DragonBonesData_get_armatureNames), nullptr);

    __jsb_dragonBones_BaseFactory_proto->defineFunction("replaceSlotDisplay", _SE(js_cocos2dx_dragonbones_BaseFactory_replaceSlotDisplay));

    __jsb_dragonBones_CCFactory_proto->defineFunction("parseTextureAtlasData", _SE(js_cocos2dx_dragonbones_CCFactory_parseTextureAtlasData));

    se::Object* global = se::ScriptEngine::getInstance()->getGlobalObject();
    se::Value dragonBonesVal, worldClockVal;
    global->getProperty("dragonBones", &dragonBonesVal);
    dragonBonesVal.toObject()->getProperty("WorldClock", &worldClockVal);

    se::Object* globalClock = se::Object::createObjectWithClass(__jsb_dragonBones_WorldClock_class);
    globalClock->setPrivateData(&dragonBones::WorldClock::clock);
    worldClockVal.toObject()->setProperty("clock", se::Value(globalClock));

    __jsb_dragonBones_WorldClock_proto->defineFunction("add", _SE(js_cocos2dx_dragonbones_WorldClock_add));
    __jsb_dragonBones_WorldClock_proto->defineFunction("remove", _SE(js_cocos2dx_dragonbones_WorldClock_remove));

    se::Value factoryVal;
    dragonBonesVal.toObject()->getProperty("CCFactory", &factoryVal);
    factoryVal.toObject()->defineFunction("getFactory", _SE(js_cocos2dx_dragonbones_CCFactory_getFactory));

    __jsb_dragonBones_TransformObject_proto->defineProperty("global", _SE(js_cocos2dx_dragonbones_TransformObject_getGlobal), nullptr);
    __jsb_dragonBones_TransformObject_proto->defineProperty("origin", _SE(js_cocos2dx_dragonbones_TransformObject_getOrigin), nullptr);
    __jsb_dragonBones_TransformObject_proto->defineProperty("offset", _SE(js_cocos2dx_dragonbones_TransformObject_getOffset), nullptr);

    __jsb_dragonBones_Slot_proto->defineProperty("name", _SE(js_cocos2dx_dragonbones_Slot_get_name), nullptr);
    __jsb_dragonBones_Slot_proto->defineFunction("getDisplay", _SE(js_cocos2dx_dragonbones_Slot_getDisplay));
    __jsb_dragonBones_Slot_proto->defineFunction("setDisplay", _SE(js_cocos2dx_dragonbones_Slot_setDisplay));

    dragonBones::BaseObject::setObjectRecycleOrDestroyCallback([](dragonBones::BaseObject* obj, int type){

        std::string typeName = typeid(*obj).name();

        se::Object* seObj = nullptr;

        auto iter = se::NativePtrToObjectMap::find(obj);
        if (iter != se::NativePtrToObjectMap::end())
        {
            // Save se::Object pointer for being used in cleanup method.
            seObj = iter->second;
            // Unmap native and js object since native object was destroyed.
            // Otherwise, it may trigger 'assertion' in se::Object::setPrivateData later
            // since native obj is already released and the new native object may be assigned with
            // the same address.
            se::NativePtrToObjectMap::erase(iter);
        }
        else
        {
            // CCLOG("Didn't find %s, %p in map", typeName, obj);
            // assert(false);
            return;
        }

        std::string typeNameStr = typeName;
        auto cleanup = [seObj, typeNameStr](){

            auto se = se::ScriptEngine::getInstance();
            if (!se->isValid() || se->isInCleanup())
                return;

            se::AutoHandleScope hs;
            se->clearException();

            // The mapping of native object & se::Object was cleared in above code.
            // The private data (native object) may be a different object associated with other se::Object.
            // Therefore, don't clear the mapping again.
            seObj->clearPrivateData(false);
            seObj->unroot();
            seObj->decRef();
        };

        if (!se::ScriptEngine::getInstance()->isGarbageCollecting())
        {
            cleanup();
        }
        else
        {
            CleanupTask::pushTaskToAutoReleasePool(cleanup);
        }
    });

    se::ScriptEngine::getInstance()->addAfterCleanupHook([](){

        // Destroy CCFactory singlton.
        dragonBones::CCFactory::destroyInstance();

        // World clock is a static variable and needs to be cleared and reset.
        dragonBones::WorldClock::clock.clear();
        dragonBones::WorldClock::clock.time = 0.0f;
        dragonBones::WorldClock::clock.timeScale = 1.0f;

        // Copy the dragonbones object vector since vector element will be deleted in BaseObject destructor.
        std::vector<dragonBones::BaseObject*> allDragonBonesObjects = dragonBones::BaseObject::getAllObjects();
        SE_LOGD("Starting to cleanup dragonbones object, count: %d\n", (int)allDragonBonesObjects.size());
        // Clear dragonBones::Armature objects those are not in the pool because
        // dragonBones::Armature controls life cycle for lots of other objects,
        // so it needs to be disposed first.
        for (auto dbObj : allDragonBonesObjects)
        {
            if (dynamic_cast<dragonBones::Armature*>(dbObj) != nullptr && !dbObj->isInPool())
            {
//                SE_LOGD("1. Force delete not in pool DragonBones Armature object: %s, %p\n", typeid(*dbObj).name(), dbObj);
                delete dbObj;
            }
        }

        // After disposing dragonBones::Armature objects, there will be lots of other kinds of objects returned to pool.
        // Therefore, we clean object pool here.
        dragonBones::BaseObject::clearPool(0);

        // Copy the dragonbones object vector again.
        allDragonBonesObjects = dragonBones::BaseObject::getAllObjects();
        SE_LOGD("After first cleanup, dragonbones object remained count: %d\n", (int)allDragonBonesObjects.size());
        // Check again whether there are some objects still in pool since the releationship of dragonbones objects is really complex.
        for (auto dbObj : allDragonBonesObjects)
        {
            if (!dbObj->isInPool())
            {
//                SE_LOGD("2. Force delete not in pool DragonBones object: %s, %p\n", typeid(*dbObj).name(), dbObj);
                delete dbObj;
            }
        }

        // Clear pool again.
        dragonBones::BaseObject::clearPool(0);

        // Don't need to use copy operator since we only print leak object below.
        auto& refAllDragonBonesObjects = dragonBones::BaseObject::getAllObjects();
        SE_LOGD("After second cleanup, dragonbones object remained count: %d\n", (int)refAllDragonBonesObjects.size());

        // Print leak objects
        for (auto dbObj : refAllDragonBonesObjects)
        {
            SE_LOGD("Leak dragonbones object: %s, %p\n", typeid(*dbObj).name(), dbObj);
        }

        // If there're leak objects, clear vector should be done for restarting game.
        refAllDragonBonesObjects.clear();
    });

    se::ScriptEngine::getInstance()->clearException();

    return true;
}
