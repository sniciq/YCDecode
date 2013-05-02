package com.oyctimes.autocamera;

import com.oyctimes.autocamera.R;

import android.app.Activity;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.TextView;

public class ResultActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_result);
//		int h = (Integer) getIntent().getExtras().get("h");
//		int w = (Integer) getIntent().getExtras().get("w");
		String info = (String) getIntent().getExtras().get("info");
		TextView textView = (TextView) findViewById(R.id.resultText);
		textView.setText(info);
//		Bitmap picture = (Bitmap) getIntent().getExtras().get("picture");
		
		//取原图的1/3
//		int bw = picture.getWidth() / 2;
//		int bh = picture.getHeight() / 2;
//		int x = (picture.getWidth() - bw) / 2;
//		int y = (picture.getHeight() - bh) /2;
		
//		textView.setText(bw + " " + bh);
//		Bitmap nb = Bitmap.createBitmap(picture, x, y, bw, bh);
//		ImageView imageView = (ImageView) findViewById(R.id.resultImageView);
//		imageView.setImageBitmap(nb);
	}

}
