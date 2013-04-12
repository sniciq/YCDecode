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


/////////////////only test///////////////////////////
////////////////////end/////////////////////////////
#define MAX_CAPACITY 1024

int otsu(uint32_t* colors, int w, int h);

jstring Java_com_yctimes_autocamera_BarCodeDecoder_doDecode__Landroid_graphics_Bitmap_2(JNIEnv* env,
		jobject javaThis, jobject bitmap, jobject tmpbitmap, jint preThresh) {
	using namespace std;
	AndroidBitmapInfo infoSource;
	AndroidBitmapInfo tmpSource;
	void* pixelscolor;
	void* pixeltmp;

	uint8_t redColor, greenColor, blueColor;
	int ret, y, x;

	if ((ret = AndroidBitmap_getInfo(env, bitmap, &infoSource)) < 0) {
		__android_log_print(ANDROID_LOG_INFO, "JNIMsg", " AndroidBitmap_getInfo() failed 1 ! ");
		return (env)->NewStringUTF("");
	}
	if ((ret = AndroidBitmap_getInfo(env, tmpbitmap, &tmpSource)) < 0) {
		__android_log_print(ANDROID_LOG_INFO, "JNIMsg", " AndroidBitmap_getInfo() failed 1 ! ");
		return (env)->NewStringUTF("");
	}

	int w = infoSource.width;
	int h = infoSource.height;

	uint8 BWImg[115200]={0};

	if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixelscolor)) < 0) {
		__android_log_print(ANDROID_LOG_INFO, "JNIMsg", " AndroidBitmap_lockPixels() failed 1 ! ");
		return (env)->NewStringUTF("");
	}
	if ((ret = AndroidBitmap_lockPixels(env, tmpbitmap, &pixeltmp)) < 0) {
		__android_log_print(ANDROID_LOG_INFO, "JNIMsg", " AndroidBitmap_lockPixels() failed 1 ! ");
		return (env)->NewStringUTF("");
	}
//	for (y = 0; y < h; y++) {
//		uint32_t *line = (uint32_t*) pixelscolor;
//		for (x = 0; x < w; x++) {
//			uint8_t r = (uint8_t) ((line[x] >> 16) & 0xFF);
//			uint8_t g = (uint8_t) ((line[x] >> 8) & 0xFF);
//			uint8_t b = (uint8_t) ((line[x]) & 0xFF);
//			uint8 gv =  0.3 * r +  0.59 * g + 0.11* b;
//			int index = (y * w / 8) + (x / 8);
//			int mm = x % 8;
//			if(gv > 127) {
//				BWImg[index]  |= 0x01 << (7 - mm);
//			}
//			else {
//				BWImg[index] |= 0x00 << (7 - mm);
//			}
//		}
//	}

	//灰度化
	uint32_t *rgbData = (uint32_t *) pixelscolor;
	uint32_t *destData = (uint32_t *) pixeltmp;
	int alpha = (int) (0xFF << 24);
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			int color = (int) rgbData[w * y + x];
			int red = (int) ((color & 0x00FF0000) >> 16);
			int green = (int) ((color & 0x0000FF00) >> 8);
			int blue = (int) (color & 0x000000FF);
			color = (red * 38 + green * 75 + blue * 15) >> 7;
			color = alpha | (color << 16) | (color << 8) | color;
			destData[y * w + x] = 0xff000000 | (color) | (color << 8) | (color << 16);
		}
	}

	//TSU获取分割阀值
	int thresh = 128;
	if(preThresh > 0) {
		thresh = preThresh;
	}
	else {
		thresh = otsu(destData, w, h);
	}

	//二值化
	int white = 0xFFFFFFFF; // 不透明白色
	int black = 0xFF000000; // 不透明黑色
	int gray;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			gray = (int)(destData[w * y + x] & 0xFF); // 获得灰度值
			int index = (y * w / 8) + (x / 8);
			int mm = x % 8;
			if(gray < thresh) {
				BWImg[index] |= 0x00 << (7 - mm);
			} else {
				BWImg[index]  |= 0x01 << (7 - mm);
			}
		}
	}

	AndroidBitmap_unlockPixels(env,bitmap);
	AndroidBitmap_unlockPixels(env,tmpbitmap);

	char testdata[5] = "abcd";
	unsigned char out_data[MAX_CAPACITY] = { 0 };
	int out_len = MAX_CAPACITY;
	int midlen = 10*1024;

	int re;
	unsigned char buf[10*1024]={0};

	re = SCDCC_BinaryImg_Decode(BWImg, w, h, buf, &midlen);

	if(re < 0) {
		__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "SCDCC_BinaryImg_Decode fial! ret=%d ", re);
		return (env)->NewStringUTF("");
	}

	re = Host_Decoder(buf, midlen, out_data, &out_len);
	if(re < 0) {
		__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Host_Decoder fial! ret=%d ", re);
		return (env)->NewStringUTF("");
	}

	string s = "";
	if(re == 0) {
		string str((char*) out_data);
	}
	const char* p = s.c_str();
	return (env)->NewStringUTF(p);

}

jstring Java_com_yctimes_autocamera_BarCodeDecoder_doDecode___3III(JNIEnv* env,
		jobject javaThis, jintArray databuf, jint w, jint h) {
	using namespace std;

	jint *pixelbuf;
	jboolean *aa = false;
	pixelbuf = env->GetIntArrayElements(databuf, aa);

	//uint8 BWImg[w*h/8]={0};
//	uint8 *BWImg = NULL;
	uint8 BWImg[115200]={0};
//	MALLOC(BWImg, uint8, w*h/8);

	int alpha = 0xFF << 24;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			// 获得像素的颜色
//			uint8 color = pixelbuf[w * y + x];
//			uint8 red = ((color & 0x00FF0000) >> 16);
//			uint8 green = ((color & 0x0000FF00) >> 8);
//			uint8 blue = color & 0x000000FF;

			uint8 red = pixelbuf[w * y + x];
			uint8 green = pixelbuf[w * y + x + 1];
			uint8 blue = pixelbuf[w * y + x + 2];
//			color = alpha | (color << 16) | (color << 8) | color;

			uint8 gv =  0.3 * red +  0.59 * green + 0.11* blue;
			int index = (y * w / 8) + (x / 8);
			int mm = x % 8;
			if(gv > 127) {
				BWImg[index]  |= 0x01 << (7 - mm);
			}
			else {
				BWImg[index] |= 0x00 << (7 - mm);
			}
		}
	}

//	for(int i = 0; i < 640*480/8 ; i++) {
//		__android_log_print(ANDROID_LOG_INFO, "JNIMsg", " i: %d, v:%d ", i, (uint8)BWImg[i]);
//	}

	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", " w: %d, h:%d ", w, h);

	char testdata[5] = "abcd";
	unsigned char out_data[MAX_CAPACITY] = { 0 };
	int out_len = MAX_CAPACITY;
	int midlen = 10*1024;

	int re;
	unsigned char buf[10*1024]={0};

	re = SCDCC_BinaryImg_Decode(BWImg, w, h, buf, &midlen);

	if(re < 0) {
		__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "SCDCC_BinaryImg_Decode fial! ret=%d ", re);
		return (env)->NewStringUTF("");
	}

	re = Host_Decoder(buf, midlen, out_data, &out_len);
	if(re < 0) {
		__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "Host_Decoder fial! ret=%d ", re);
		return (env)->NewStringUTF("");
	}

	string s = "";
	if(re == 0) {
		string str((char*) out_data);
	}
	const char* p = s.c_str();
	return (env)->NewStringUTF(p);
}


/**
 * OTSU算法求最适分割阈值
 */
int otsu(uint32_t* rgbData, int w, int h) {
    unsigned int pixelNum[256]; // 图象灰度直方图[0, 255]
    int n, n0, n1; //  图像总点数，前景点数， 后景点数（n0 + n1 = n）
    int w0, w1; // 前景所占比例， 后景所占比例（w0 = n0 / n, w0 + w1 = 1）
    double u, u0, u1; // 总平均灰度，前景平均灰度，后景平均灰度（u = w0 * u0 + w1 * u1）
    double g, gMax; // 图像类间方差，最大类间方差（g = w0*(u0-u)^2+w1*(u1-u)^2 = w0*w1*(u0-u1)^2）
    double sum_u, sum_u0, sum_u1; // 图像灰度总和，前景灰度总和， 后景平均总和（sum_u = n * u）
    int thresh; // 阈值

    memset(pixelNum, 0, 256 * sizeof(unsigned int)); // 数组置0

    int x, y, gray;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			gray = (int) ((rgbData[w * y + x]) & 0xFF); // 获得灰度值
			pixelNum[gray]++;
		}
	}

    // 图像总点数
    n = w * h;

    // 计算总灰度
    int k;
    for (k = 0; k <= 255; k++) {
        sum_u += k * pixelNum[k];
    }

    // 遍历判断最大类间方差，得到最佳阈值
    for (k = 0; k <= 255; k++) {
        n0 += pixelNum[k]; // 图像前景点数
        if (0 == n0) { // 未获取前景，直接继续增加前景点数
            continue;
        }
        if (n == n0) { // 前景点数包括了全部时，不可能再增加，退出循环
            break;
        }
        n1 = n - n0; // 图像后景点数

        sum_u0 += k * pixelNum[k]; // 前景灰度总和
        u0 = sum_u0 / n0; // 前景平均灰度
        u1 = (sum_u - sum_u0) / n1; // 后景平均灰度

        g = n0 * n1 * (u0 - u1) * (u0 - u1); // 类间方差（少除了n^2）

        if (g > gMax) { // 大于最大类间方差时
            gMax = g; // 设置最大类间方差
            thresh = k; // 取最大类间方差时对应的灰度的k就是最佳阈值
        }
    }

    return thresh;
}
