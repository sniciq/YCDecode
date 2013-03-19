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
	
	/**
	 * key, value<br/>
	 * result, true/false<br/>
	 * info, 内容(true时为条码内容，false时为错误信息)
	 * @param bitmap
	 * @return
	 */
	public Map<String, Object> decode(Bitmap bitmap) {
		Map<String, Object> retMap = new HashMap<String, Object>();
		retMap.put("result", true);
		String timeStamp = new SimpleDateFormat("yyyyMMdd-HHmmss", Locale.US).format(new Date());
		retMap.put("info", timeStamp + "aaaaaaaaaaaaaaaaaavvvvvvvvvvvvvvvvvvvvv");
		return retMap;
	}

}
