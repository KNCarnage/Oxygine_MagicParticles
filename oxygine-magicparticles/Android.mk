LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := magicparticles
LOCAL_SRC_FILES := magic_particles/android/libs/$(TARGET_ARCH_ABI)/libamagic.a
include $(PREBUILT_STATIC_LIBRARY) 


include $(CLEAR_VARS)

LOCAL_MODULE    := oxygine-magicparticles_static
LOCAL_MODULE_FILENAME := liboxygine-magicparticles
LOCAL_CPP_EXTENSION := .cpp

SRC := $(LOCAL_PATH)/src


LOCAL_SRC_FILES := \
		src/MagicEmitter.cpp \
		src/ResMagicParticles.cpp

LOCAL_C_INCLUDES := $(SRC) \
					$(OXYGINE_SRC)/ \
					$(LOCAL_PATH)/magic_particles/android/

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_C_INCLUDES)
LOCAL_WHOLE_STATIC_LIBRARIES := magicparticles
					
include $(BUILD_STATIC_LIBRARY)
