package com.yctimes.autocamera;

import java.util.ArrayList;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;

public class SettingActivity extends Activity {
	private Spinner picSpinner;
	private ArrayAdapter<String> spinnerAdapter;
	private EditText text_takePicPeriod;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_setting);
		
		picSpinner = (Spinner) findViewById(R.id.picSizeSel);
		
		int[] orgPicSize = getIntent().getExtras().getIntArray(AppSetting.PictureSize);
		if(orgPicSize == null) {
			orgPicSize = new int[2];
			orgPicSize[0] = 0; orgPicSize[1] = 0;
		}
		
		int takePicPeriod = getIntent().getExtras().getInt(AppSetting.TakePicPeriod);
		int selectItemPosition = 0;
		ArrayList<String> supportedPicSizes = getIntent().getStringArrayListExtra("supportSizes");
		String[] m = new String[supportedPicSizes.size()];
		for(int i = 0; i < supportedPicSizes.size(); i++) {
			String s = supportedPicSizes.get(i);
			supportedPicSizes.toArray(m);
			if((orgPicSize[0] + "X" + orgPicSize[1]).equals(s)) {
				selectItemPosition = i;
			}
		}
		spinnerAdapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item,m);
		spinnerAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);  

		picSpinner.setAdapter(spinnerAdapter);  
		picSpinner.setSelection(selectItemPosition);
		
		text_takePicPeriod = (EditText) findViewById(R.id.text_takePicPeriod);
		text_takePicPeriod.setText(takePicPeriod+"");
	}
	
	public void settingOK(View v) {
		Intent output = new Intent();
		String size = spinnerAdapter.getItem(picSpinner.getSelectedItemPosition());
		int takePicPeriod = 3;
		String ss = text_takePicPeriod.getText().toString();
		if(!ss.trim().equals("")) {
			takePicPeriod = Integer.parseInt(ss);
		}
		
		output.putExtra(AppSetting.PictureSize, size);
		output.putExtra(AppSetting.TakePicPeriod, takePicPeriod);
		setResult(RESULT_OK, output);
		finish();
	}
	
	public void settingCancel(View v) {
		finish();
	}
}
