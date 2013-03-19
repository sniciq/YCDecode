package com.yctimes.autocamera;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;

import android.graphics.Bitmap;

/**
 * �������
 *
 */
public class BarCodeDecoder {
	
	/**
	 * key, value<br/>
	 * result, true/false<br/>
	 * info, ����(trueʱΪ�������ݣ�falseʱΪ������Ϣ)
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
