package com.yctimes.autocamera;

import com.example.autocamera.R;

import android.app.Activity;
import android.os.Bundle;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.WindowManager;

public class MainActivity extends Activity {
	private CameraView cameraView;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
//		setContentView(R.layout.activity_main);
//		getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
//		getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
//		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
//		getActionBar().hide();
		cameraView = new CameraView(this);
//		cameraView.setOnTouchListener(new OnTouchListener() {
//			@Override
//			public boolean onTouch(View v, MotionEvent event) {
//				if(getActionBar().isShowing()) {
//					getActionBar().hide();
//				}
//				else {
//					getActionBar().show();
//				}
//				return false;
//			}
//		});
		
		setContentView(cameraView);
	}

	@Override
	public void onCreateContextMenu(ContextMenu menu, View v,
			ContextMenuInfo menuInfo) {
		getMenuInflater().inflate(R.menu.main, menu);
		super.onCreateContextMenu(menu, v, menuInfo);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	protected void onResume() {
		cameraView.resume();
		super.onResume();
	}

	@Override
	protected void onPause() {
		cameraView.pause();
		super.onPause();
	}
}
