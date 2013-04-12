package com.yctimes.autocamera;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;

import android.graphics.Bitmap;

/**
 * 条码解析
 *
 */
public class BarCodeDecoder {
	
	public native String doDecode(int[] buf, int w, int h);
	
	public native String doDecode(Bitmap bitmap, Bitmap tmpBitmap, int thresh);
	
	private void saveBWFile(Bitmap bitmap) {
	}
	
	/**
	 * key, value<br/>
	 * result, true/false<br/>
	 * info, 信息(true时为条码内容false)
	 * @param bitmap
	 * @return
	 */
	public Map<String, Object> decode(Bitmap bitmap) {
		
		Map<String, Object> retMap = new HashMap<String, Object>();
		
//		int w=bitmap.getWidth(),h=bitmap.getHeight();
//        int[] pix = new int[w * h];
//        bitmap.getPixels(pix, 0, w, 0, 0, w, h);
//        String s = doDecode(pix, w, h);
        
		Bitmap convertBitmap = bitmap.copy(Bitmap.Config.ARGB_8888, true);
		Bitmap tempBitmap = bitmap.copy(Bitmap.Config.ARGB_8888, true);
        String s = doDecode(convertBitmap, tempBitmap, 128);
        if(s.trim().equals("")) {
        	s = doDecode(convertBitmap, tempBitmap, -1);
        }
		
		if(!s.trim().equals("")) {
			retMap.put("result", true);
			String timeStamp = new SimpleDateFormat("yyyyMMdd-HHmmss", Locale.US).format(new Date());
			retMap.put("info", timeStamp + "|" + s);
		}
		else {
			retMap.put("result", false);
		}
		return retMap;
	}
	
	static {  
        System.loadLibrary("YCDecode");  
    }
}
