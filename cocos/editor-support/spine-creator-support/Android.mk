LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := spine_static

LOCAL_MODULE_FILENAME := libspine

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
../spine/Animation.c \
../spine/AnimationState.c \
../spine/AnimationStateData.c \
../spine/Atlas.c \
../spine/AtlasAttachmentLoader.c \
../spine/Attachment.c \
../spine/AttachmentLoader.c \
../spine/Bone.c \
../spine/BoneData.c \
../spine/BoundingBoxAttachment.c \
../spine/Event.c \
../spine/EventData.c \
../spine/IkConstraint.c \
../spine/IkConstraintData.c \
../spine/Json.c \
../spine/MeshAttachment.c \
../spine/PathAttachment.c \
../spine/PathConstraint.c \
../spine/PathConstraintData.c \
../spine/RegionAttachment.c \
../spine/Skeleton.c \
../spine/SkeletonBinary.c \
../spine/SkeletonBounds.c \
../spine/SkeletonData.c \
../spine/SkeletonJson.c \
../spine/Skin.c \
../spine/Slot.c \
../spine/SlotData.c \
../spine/TransformConstraint.c \
../spine/TransformConstraintData.c \
../spine/VertexAttachment.c \
../spine/extension.c \
../IOBuffer.cpp \
AttachmentVertices.cpp \
CreatorAttachmentLoader.cpp \
SpineAnimation.cpp \
SpineRenderer.cpp \
spine-cocos2dx.cpp \
spine-adapter.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/.. \
						   $(LOCAL_PATH)/../..

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
					$(LOCAL_PATH)/../.. \
                    $(LOCAL_PATH)/../../../external/android/$(TARGET_ARCH_ABI)/include/v8
					
include $(BUILD_STATIC_LIBRARY)
