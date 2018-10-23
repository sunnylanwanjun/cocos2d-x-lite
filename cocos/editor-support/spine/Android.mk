LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := spine_static

LOCAL_MODULE_FILENAME := libspine

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
../IOBuffer.cpp \
Animation.c \
AnimationState.c \
AnimationStateData.c \
Atlas.c \
AtlasAttachmentLoader.c \
Attachment.c \
AttachmentLoader.c \
jsb/AttachmentVertices.cpp \
Bone.c \
BoneData.c \
BoundingBoxAttachment.c \
jsb/Cocos2dAttachmentLoader.cpp \
Event.c \
EventData.c \
IkConstraint.c \
IkConstraintData.c \
Json.c \
MeshAttachment.c \
PathAttachment.c \
PathConstraint.c \
PathConstraintData.c \
RegionAttachment.c \
Skeleton.c \
jsb/SkeletonAnimation.cpp \
SkeletonBinary.c \
SkeletonBounds.c \
SkeletonData.c \
SkeletonJson.c \
jsb/SkeletonRenderer.cpp \
Skin.c \
Slot.c \
SlotData.c \
TransformConstraint.c \
TransformConstraintData.c \
VertexAttachment.c \
extension.c \
jsb/spine-cocos2dx.cpp \
jsb/spine-adapter.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/.. \
						   $(LOCAL_PATH)/../..

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
					$(LOCAL_PATH)/../.. \
                    $(LOCAL_PATH)/../../../external/android/$(TARGET_ARCH_ABI)/include/v8
					
include $(BUILD_STATIC_LIBRARY)
