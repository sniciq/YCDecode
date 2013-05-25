#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include "com_yctimes_autocamera_BarCodeDecoder.h"
#include <iostream>
#include <android/log.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include "scdcc_decoder.h"
#include "host_decoder.h"
#include "scdcc_malloc.h"
#include "scdcc_errs.h"



#define MAX_CAPACITY 1024

int cvThresholdOtsu(int ** grayArr, int width, int height);
int ** initArray(int height, int width);

jstring Java_com_yctimes_autocamera_BarCodeDecoder_doDecode(JNIEnv* env,
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

	int w = infoSource.width;
	int h = infoSource.height;

//	int bw = w / 2;
//	int bh = h / 2;
//	int xs = (w - bw) / 2;
//	int ys = (h - bh) /2;

//	int bw = w / 1.25 ;
//	int bh = h / 1.25;
//	int xs = (w - bw) / 2;
//	int ys = (h - bh) /2;

	int bw = w ;
	int bh = h;
	int xs = (w - bw) / 2;
	int ys = (h - bh) /2;

	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "w=%d, h=%d, bw=%d, bh=%d, xs=%d, ys=%d", w, h, bw, bh, xs, ys);

	//	uint8 BWImg[115200]={0};
	uint8 *BWImg = NULL ;
	uint32 w_size = bw ;
	uint32 H_size = bh ;

	BWImg = (uint8 *)malloc(w_size * H_size);
	if(BWImg == NULL)
	{
	  string str = "内存分配错误！";
	  const char* p = str.c_str();
	  return (env)->NewStringUTF(p);
	}

	uint32_t *rgbData = (uint32_t *) pixelscolor;

	//灰度化
	int ** grayArr = initArray(bw, bh);
	int alpha = (int) (0xFF << 24);
	for(int y = 0; y < bh; y++) {
		for(int x = 0; x < bw; x++) {
			int color = (int) rgbData[w * (y + ys) + x + xs];
			int red = (int) ((color & 0x00FF0000) >> 16);
			int green = (int) ((color & 0x0000FF00) >> 8);
			int blue = (int) (color & 0x000000FF);
			color = (red * 38 + green * 75 + blue * 15) >> 7;
			grayArr[x][y] = color;
		}
	}

	// OTSU获取分割阀值
	int thresh = cvThresholdOtsu(grayArr, bw, bh);
	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "thresh=%d ", thresh);

	//二值化
	int white = 0xFFFFFFFF; // 不透明白色
	int black = 0xFF000000; // 不透明黑色
	int gray;
	for (y = 0; y < bh; y++) {
		for (x = 0; x < bw; x++) {
			gray = grayArr[x][y];
			int index = (y * bw / 8) + (x / 8);
			int mm = x % 8;
			if(gray < thresh) {
				BWImg[index] |= 0x01 << (7 - mm);
			} else {
				BWImg[index]  |= 0x00 << (7 - mm);
			}
		}
	}

	unsigned char out_data[MAX_CAPACITY] = { 0 };
	int out_len = MAX_CAPACITY;
	int midlen = 10*1024;

	int re;
	unsigned char buf[10*1024]={0};

	re = SCDCC_BinaryImg_Decode(BWImg, bw, bh, buf, &midlen);
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
	for(int index = 0; index < bw; index++) {
		delete [] grayArr[index];
	}
	delete [] grayArr;
	free(BWImg);


	if(re == 0) {
		string str((char*)&out_data[2]);
		const char* p = str.c_str();
		return (env)->NewStringUTF(p);
	}
	else {
		return (env)->NewStringUTF("");
	}

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

