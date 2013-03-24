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
	
	public native String doDecode(Bitmap bitMap);
	
	/**
	 * key, value<br/>
	 * result, true/false<br/>
	 * info, 信息(true时为条码内容false)
	 * @param bitmap
	 * @return
	 */
	public Map<String, Object> decode(Bitmap bitmap) {
		Map<String, Object> retMap = new HashMap<String, Object>();
		String s = doDecode(bitmap);
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
