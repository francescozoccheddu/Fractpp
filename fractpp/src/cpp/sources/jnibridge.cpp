#include <jnibridge.hpp>
#include <debug.hpp>
#include <coordinator.hpp>

#define FPP_LOG_TAG "JNI Bridge"

JNIEXPORT jlong JNICALL
Java_com_francescoz_fractpp_FPPSurface_jniInit(JNIEnv *env, jobject game, jobject bridge) {
    FPP_LOGD("Init");
    FPP::Coordinator *coordinator = new FPP::Coordinator(bridge);
    return reinterpret_cast<jlong> (coordinator);
}

JNIEXPORT void JNICALL
Java_com_francescoz_fractpp_FPPSurface_jniDispose(JNIEnv *env, jobject game, jlong obj) {
    FPP_LOGD("Dispose");
    FPP::Coordinator *coordinator = reinterpret_cast<FPP::Coordinator *> (obj);
    coordinator->destroy();
    delete coordinator;
}

JNIEXPORT void JNICALL
Java_com_francescoz_fractpp_FPPSurface_jniSetSurface(JNIEnv *env, jobject game, jlong obj,
                                                     jobject surface) {
    FPP_LOGD(surface ? "Set surface" : "Set null surface");
    FPP::Coordinator *coordinator = reinterpret_cast<FPP::Coordinator *> (obj);
    coordinator->setSurface(surface);
}

