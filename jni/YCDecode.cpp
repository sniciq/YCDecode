#include <jni.h>
#include <android/bitmap.h>
#include "com_yctimes_autocamera_BarCodeDecoder.h"
#include <iostream>
#include <android/log.h>
#include <sstream>
#include <string>


jstring Java_com_yctimes_autocamera_BarCodeDecoder_doDecode (JNIEnv* env, jobject javaThis, jobject bitmap) {
	using namespace std;

	AndroidBitmapInfo infoSource;
	int ret;
	ret = AndroidBitmap_getInfo(env, bitmap, &infoSource);
	if(ret < 0) {
		return (env)->NewStringUTF("AndroidBitmap_getInfo() failed 1 !");
	}

	uint32_t width = infoSource.width;
	uint32_t height = infoSource.height;
	//color image :: width is %d; height is %d; stride is %d; format is %d;flags is%d
	uint32_t stride = infoSource.stride;
	uint32_t format = infoSource.format;
	uint32_t flags = infoSource.flags;

	string s = "color image , width is ";

//		int width = 100;

	stringstream ss;
	string ws;
	ss << width;
	ss >> ws;
	s += ws;

	ss.clear();
	ss << height;
	ss >> ws;
	s += ",height is";
	s += ws;

	ss.clear();
	ss << stride;
	ss >> ws;
	s += ",stride is";
	s += ws;

	ss.clear();
	ss << format;
	ss >> ws;
	s += ",format is";
	s += ws;

	ss.clear();
	ss << flags;
	ss >> ws;
	s += ",flags is";
	s += ws;

	const char* p = s.c_str();
	jstring encoding = (env)->NewStringUTF(p);
	return encoding;
}

