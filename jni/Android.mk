LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := YCDecode

LOCAL_SRC_FILES := \
        scdcc_rs_decode.cpp \
        SCDCC_Symbol.cpp \
        host_decoder.cpp \
        scdcc_bitstream.cpp \
        scdcc_error_correction.cpp \
        scdcc_decoder.cpp \
        scdcc_erosion_dialation.cpp \
        scdcc_grid_analysis.cpp \
        scdcc_image_binarize.cpp \
        scdcc_image_connect_divide.cpp \
        scdcc_image_enhance.cpp \
        scdcc_linear_fit.cpp \
        scdcc_malloc.cpp \
        scdcc_read_symbol.cpp \
        YCDecode.cpp 
       
LOCAL_LDFLAGS +=-ljnigraphics
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
include $(BUILD_SHARED_LIBRARY)