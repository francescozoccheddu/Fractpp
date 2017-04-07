#ifndef MATZ_JNIBRIDGE_HPP
#define MATZ_JNIBRIDGE_HPP

#include <jni.h>

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_francescoz_fractpp_FPPSurface_jniInit(JNIEnv *env, jobject game, jobject bridge);

JNIEXPORT void JNICALL
Java_com_francescoz_fractpp_FPPSurface_jniDispose(JNIEnv *env, jobject game, jlong obj);

JNIEXPORT void JNICALL
Java_com_francescoz_fractpp_FPPSurface_jniSetSurface(JNIEnv *env, jobject game, jlong obj, jobject surface);

};

#endif
