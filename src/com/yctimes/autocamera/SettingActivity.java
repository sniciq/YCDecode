package com.yctimes.autocamera;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Adapter;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

public class SettingActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_setting);
		
		Spinner picSpinner = (Spinner) findViewById(R.id.picSizeSel);
		String[] m={"A型","B型","O型","AB型","其他"};
		ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item,m);
		adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);  
     
        //将adapter 添加到spinner中  
		picSpinner.setAdapter(adapter);  
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.setting, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.btn_settings_OK:
			//FIXME save setting info
			finish();
			break;
		case R.id.btn_settings_Cancel:
			finish();
			break;
		default:
			break;
		}
		return super.onOptionsItemSelected(item);
	}

}
