LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := dragonbones_static

LOCAL_MODULE_FILENAME := libdragonbones

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
../dragonBones/animation/Animation.cpp \
../dragonBones/animation/AnimationState.cpp \
../dragonBones/animation/BaseTimelineState.cpp \
../dragonBones/animation/TimelineState.cpp \
../dragonBones/animation/WorldClock.cpp \
../dragonBones/armature/Armature.cpp \
../dragonBones/armature/Bone.cpp \
../dragonBones/armature/Constraint.cpp \
../dragonBones/armature/DeformVertices.cpp \
../dragonBones/armature/Slot.cpp \
../dragonBones/armature/TransformObject.cpp \
../dragonBones/core/BaseObject.cpp \
../dragonBones/core/DragonBones.cpp \
../dragonBones/event/EventObject.cpp \
../dragonBones/factory/BaseFactory.cpp \
../dragonBones/geom/Point.cpp \
../dragonBones/geom/Transform.cpp \
../dragonBones/model/AnimationConfig.cpp \
../dragonBones/model/AnimationData.cpp \
../dragonBones/model/ArmatureData.cpp \
../dragonBones/model/BoundingBoxData.cpp \
../dragonBones/model/CanvasData.cpp \
../dragonBones/model/ConstraintData.cpp \
../dragonBones/model/DisplayData.cpp \
../dragonBones/model/DragonBonesData.cpp \
../dragonBones/model/SkinData.cpp \
../dragonBones/model/TextureAtlasData.cpp \
../dragonBones/model/UserData.cpp \
../dragonBones/parser/DataParser.cpp \
../dragonBones/parser/BinaryDataParser.cpp \
../dragonBones/parser/JSONDataParser.cpp \
CCArmatureDisplay.cpp \
CCFactory.cpp \
CCSlot.cpp \
CCTextureAtlasData.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/.. \
						   $(LOCAL_PATH)/../..

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
					$(LOCAL_PATH)/../.. \
                    $(LOCAL_PATH)/../../../external/android/$(TARGET_ARCH_ABI)/include/v8 \
					$(LOCAL_PATH)/../../../external/sources/

include $(BUILD_STATIC_LIBRARY)
