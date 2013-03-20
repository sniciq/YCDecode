#include <jni.h>
#include <android/bitmap.h>
#include "com_yctimes_autocamera_BarCodeDecoder.h"
#include <iostream>

jstring Java_com_yctimes_autocamera_BarCodeDecoder_doDecode (JNIEnv* env, jobject thiz, jobject bitmap) {
	jstring encoding = (env)->NewStringUTF("AAAAAAAAAAAA");
	return encoding;
}

int main() {
	using namespace std;
	cout << "aaaaaaaaaaaa";
}
