LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := YCDecode
LOCAL_SRC_FILES := YCDecode.cpp
LOCAL_LDFLAGS +=-ljnigraphics

include $(BUILD_SHARED_LIBRARY)


