#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <android/log.h>
#include <unistd.h>
#include "turbojpeg.h"

#define LOGTAG "demo"

#define LOGV(fmt, ...) __android_log_print(ANDROID_LOG_VERBOSE, LOGTAG, fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, LOGTAG, fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) __android_log_print(ANDROID_LOG_INFO, LOGTAG, fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) __android_log_print(ANDROID_LOG_WARN, LOGTAG, fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) __android_log_print(ANDROID_LOG_ERROR, LOGTAG, fmt, ##__VA_ARGS__)

#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

jint JNI_OnLoad(JavaVM* vm, void* reserved);
jbyteArray test(JNIEnv *env, jobject thiz, jbyteArray raws, jint width, jint height);

static const char *classPathName = "com/example/compressdemo/Demo";


static JNINativeMethod methods[] = {
		{"test", "([BII)[B", (void*)test},
};

jbyteArray test(JNIEnv *env, jobject thiz, jbyteArray raws, jint width, jint height){
	tjhandle handle;
	unsigned char *pBuffer = (unsigned char*)malloc(width*height*4);
	memset(pBuffer, 0, width*height*4);
	(*env)->GetByteArrayRegion(env, raws, 0, width*height*4, pBuffer);
//	unsigned char *pBuffer = (*env)->GetByteArrayElements(env, raws, NULL);
	unsigned char *jpeg_data;
	unsigned long jpegSize = -1;
	int ret;
	int size = width*height*4;
	jpeg_data = (unsigned char*)tjAlloc(size);
	handle = tjInitCompress();
	if(handle == NULL){
		LOGD("tjInitCompress error");
		goto test_clean_up;
	}

	ret = tjCompress2(handle, pBuffer, width, width*4, height, TJPF_ARGB, &jpeg_data, &jpegSize, TJSAMP_444, 100, TJFLAG_FASTDCT);
	if(ret == -1){
		LOGD("tjCompress2 error");
		goto test_clean_up;
	}
	LOGD("jpeg compress : %ld", jpegSize);
	jbyteArray array = (*env)->NewByteArray(env, jpegSize);
	(*env)->SetByteArrayRegion(env, array, 0, jpegSize, (jbyte*)jpeg_data);
test_clean_up:
	tjFree(jpeg_data);
	return array;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	JNIEnv* env = NULL;
	jclass clazz;
	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		LOGE("ERROR: GetEnv failed\n");
		return JNI_ERR;
	}
	clazz = (*env)->FindClass(env, classPathName);
	if (clazz == NULL) {
		LOGE("Native registration unable to find class '%s'", classPathName);
		return JNI_ERR;
	}
	if ((*env)->RegisterNatives(env, clazz, methods, NELEM(methods)) < 0) {
		LOGE("ERROR: MediaPlayer native registration failed\n");
		return JNI_ERR;
	}
	return JNI_VERSION_1_4;
}

