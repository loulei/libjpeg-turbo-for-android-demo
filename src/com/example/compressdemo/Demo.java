package com.example.compressdemo;

public class Demo {

	static{
		System.loadLibrary("demo");
	}
	
	public native  byte[] test(byte[] raws, int width, int height);
}
