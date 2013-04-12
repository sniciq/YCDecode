LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := OYCDecode
LOCAL_SRC_FILES := YCDecode.cpp
LOCAL_LDFLAGS +=-ljnigraphics
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)


