package com.yctimes.autocamera;

import java.util.HashMap;
import java.util.Map;

import android.graphics.Bitmap;

/**
 * 条码解析
 *
 */
public class BarCodeDecoder {
	
	public native String doDecode(Bitmap bitmap);
	
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
		
		Bitmap convertBitmap = bitmap.copy(Bitmap.Config.ARGB_8888, true);
        String s = doDecode(convertBitmap);
		
		if(!s.trim().equals("")) {
			retMap.put("result", true);			
			retMap.put("info", s);
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
