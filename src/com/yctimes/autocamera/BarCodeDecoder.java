package com.yctimes.autocamera;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.graphics.Bitmap;

/**
 * 条码解析
 *
 */
public class BarCodeDecoder {
	
	private List<BarCodeDecodeOverListener> decodeOverListeners = new ArrayList<BarCodeDecodeOverListener>();
	
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

	public void decodeInThread(final Bitmap picture) {
		if(picture == null)
			return;
		
		new Thread() {
			public void run() {
				Map<String, Object> decodeMap = BarCodeDecoder.this.decode(picture);
				BarCodeDecoder.this.fireDecodeOver(decodeMap);
			}
		}.run();
	}
	
	private void fireDecodeOver(Map<String, Object> decodeMap) {
		for(BarCodeDecodeOverListener lis : decodeOverListeners) {
			lis.deCodeOver(decodeMap);
		}
	}
	
	public void addListener(BarCodeDecodeOverListener lis) {
		this.decodeOverListeners.add(lis);
	}
	
	public void removeListener(BarCodeDecodeOverListener lis) {
		this.decodeOverListeners.remove(lis);
	}
	
}
