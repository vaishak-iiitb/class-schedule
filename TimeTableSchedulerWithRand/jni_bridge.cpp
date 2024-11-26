#include <jni.h>
#include "TimeTableScheduler(withRand).cpp"

extern "C" JNIEXPORT void JNICALL Java_TimeTableGenerator_generateTimeTable
  (JNIEnv* env, jobject obj) {
    try {
        makeTimetable();
    } catch (const std::exception& e) {
        jclass exceptionClass = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exceptionClass, e.what());
    }
}
