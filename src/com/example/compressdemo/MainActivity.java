package com.example.compressdemo;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;

public class MainActivity extends Activity {

	private Button btn_compress;
	private ImageView iv_dest;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		btn_compress = (Button) findViewById(R.id.btn_compress);
		iv_dest = (ImageView) findViewById(R.id.iv_dest);
		btn_compress.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.splash1);
				int[] pixels = new int[bitmap.getWidth()*bitmap.getHeight()];
				bitmap.getPixels(pixels, 0, bitmap.getWidth(), 0, 0, bitmap.getWidth(), bitmap.getHeight());
				byte[] raws = new byte[bitmap.getWidth()*bitmap.getHeight()*4];
				for(int i=0; i<bitmap.getWidth()*bitmap.getHeight(); i++){
					int pixel = pixels[i];
					byte alpha = (byte) ((pixel & 0xff000000) >> 24);
					byte red = (byte) ((pixel & 0x00ff0000) >> 16);
					byte green = (byte) ((pixel & 0x0000ff00) >> 8);
					byte blue = (byte) (pixel & 0x000000ff);
					raws[i*4] = alpha;
					raws[i*4+1] = red;
					raws[i*4+2] = green;
					raws[i*4+3] = blue;
				}
				byte[] dests = new Demo().test(raws, bitmap.getWidth(), bitmap.getHeight());
				Logger.d("dest size:"+dests.length);
				Bitmap bitmap2 = BitmapFactory.decodeByteArray(dests, 0, dests.length);
				iv_dest.setImageBitmap(bitmap2);
			}
		});
	}
}
