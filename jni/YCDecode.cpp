#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include "com_yctimes_autocamera_BarCodeDecoder.h"
#include <iostream>
#include <android/log.h>
#include <sstream>
#include <string>
#include <unistd.h>

/*
#include "scdcc_decoder.h"
#include "host_decoder.h"
#include "scdcc_malloc.h"
#include "scdcc_errs.h"
*/

/////////////////only test///////////////////////////
////////////////////end/////////////////////////////
#define MAX_CAPACITY 1024

int cvThresholdOtsu(int ** grayArr, int width, int height);
int ** initArray(int height, int width);

jstring Java_com_oyctimes_autocamera_BarCodeDecoder_doDecode(JNIEnv* env,
		jobject javaThis, jobject bitmap) {
	using namespace std;
	AndroidBitmapInfo infoSource;
	void* pixelscolor;

	uint8_t redColor, greenColor, blueColor;
	int ret, y, x;

	if ((ret = AndroidBitmap_getInfo(env, bitmap, &infoSource)) < 0) {
		__android_log_print(ANDROID_LOG_INFO, "JNIMsg", " AndroidBitmap_getInfo() failed 1 ! ");
		return (env)->NewStringUTF("");
	}
	if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixelscolor)) < 0) {
		__android_log_print(ANDROID_LOG_INFO, "JNIMsg", " AndroidBitmap_lockPixels() failed 1 ! ");
		return (env)->NewStringUTF("");
	}

	/*
	uint8 BWImg[115200]={0};

	int w = infoSource.width;
	int h = infoSource.height;
	uint32_t *rgbData = (uint32_t *) pixelscolor;

	//灰度化
	int ** grayArr = initArray(w, h);
	int alpha = (int) (0xFF << 24);
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			int color = (int) rgbData[w * y + x];
			int red = (int) ((color & 0x00FF0000) >> 16);
			int green = (int) ((color & 0x0000FF00) >> 8);
			int blue = (int) (color & 0x000000FF);
			color = (red * 38 + green * 75 + blue * 15) >> 7;
			grayArr[x][y] = color;
		}
	}

	// OTSU获取分割阀值
	int thresh = cvThresholdOtsu(grayArr, w, h);
	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "thresh=%d ", thresh);

	//二值化
	int white = 0xFFFFFFFF; // 不透明白色
	int black = 0xFF000000; // 不透明黑色
	int gray;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			gray = grayArr[x][y];
			int index = (y * w / 8) + (x / 8);
			int mm = x % 8;
			if(gray < thresh) {
				BWImg[index] |= 0x01 << (7 - mm);
			} else {
				BWImg[index]  |= 0x00 << (7 - mm);
			}
		}
	}

	char testdata[5] = "abcd";
	unsigned char out_data[MAX_CAPACITY] = { 0 };
	int out_len = MAX_CAPACITY;
	int midlen = 10*1024;

	int re;
	unsigned char buf[10*1024]={0};

	re = SCDCC_BinaryImg_Decode(BWImg, w, h, buf, &midlen);
	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "SCDCC_BinaryImg_Decode ret=%d ", re);
	if(re < 0) {
		__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "SCDCC_BinaryImg_Decode fial! ");
		return (env)->NewStringUTF("");
	}

	re = Host_Decoder(buf, midlen, out_data, &out_len);
	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Host_Decoder ret=%d ", re);
	if(re < 0) {
		__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Host_Decoder fial! ");
		return (env)->NewStringUTF("");
	}

	AndroidBitmap_unlockPixels(env,bitmap);
	for(int index = 0; index < w; index++) {
		delete [] grayArr[index];
	}
	delete [] grayArr;

	if(re == 0) {
		string str((char*) out_data);
		const char* p = str.c_str();
		return (env)->NewStringUTF(p);
	}
	else {
		return (env)->NewStringUTF("");
	}
*/
//	unsigned char out_data[]="中文abc123";

	unsigned char out_data11[]={0x61,0x62,0x63,0xC4,0xE3,0xBA,0xC3,0xA1,0xA3};//abc你好
	string ass((char*) out_data11);
	const char* pp = ass.c_str();
	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "SS:%s ", out_data11);

	unsigned char out_data[]="中文abc123";
	string str((char*) out_data);
	const char* p = str.c_str();


	return (env)->NewStringUTF(p);
}

int ** initArray(int width, int height){
   int ** gray = new int *[width];
   for (unsigned int i = 0; i < width; ++i)
   {
	  gray[i] = new int[height];
   }

   return gray;
}

int cvThresholdOtsu(int ** grayArr, int width, int height) {
    //histogram
    float histogram[256]= {0};
    for(int i=0; i<height; i++) {
    	for(int j=0; j<width; j++) {
			histogram[grayArr[j][i]]++;
		}
	}

    //normalize histogram
    int size=height  *width;
    for(int i=0; i<256; i++)
    {
        histogram[i]=histogram[i]/size;
    }

    //average pixel value
    float avgValue=0;
    for(int i=0; i<256; i++)
    {
        avgValue+=i*histogram[i];
    }

    int threshold;
    float maxVariance=0;
    float w=0,u=0;
    for(int i=0; i<256; i++)
    {
        w+=histogram[i];
        u+=i*histogram[i];

        float t=avgValue*w-u;
        float variance=t*t/(w*(1-w));
        if(variance>maxVariance)
        {
            maxVariance=variance;
            threshold=i;
        }
    }

    return threshold;
}

