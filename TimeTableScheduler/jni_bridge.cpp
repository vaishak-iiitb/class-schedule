#include <jni.h>
#include "TimeTableScheduler(withRand).cpp"  // Include the C++ file containing the scheduling logic

/*
    This file bridges Java and C++ using JNI to allow Java to invoke the C++ timetable generation logic.
    The primary function defined here is the `Java_TimeTableGenerator_generateTimeTable`, which is the
    native method linked to a corresponding Java method in the `TimeTableGenerator` class.
*/



extern "C" JNIEXPORT void JNICALL Java_TimeTableGenerator_generateTimeTable
  (JNIEnv* env, jobject obj) {

    /*
    Function: Java_TimeTableGenerator_generateTimeTable
    Purpose: Serves as the JNI entry point for generating a timetable from Java.
    This function invokes the `makeTimetable` function in the included C++ file.
    If any exception occurs during execution, it is caught and reported back to Java.
    Parameters:
        - env: Pointer to the JNI environment, used for calling JNI methods and managing Java objects.
        - obj: Reference to the calling Java object (`TimeTableGenerator`).
    */

    try {
        // Call the C++ function responsible for generating the timetable
        makeTimetable();
    } catch (const std::exception& e) {

        /*
        If an exception occurs in the C++ logic, it is caught here.
        A RuntimeException is created in the Java environment to notify Java about the error.
        */

        jclass exceptionClass = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exceptionClass, e.what());  // Throw the exception with the C++ error message
    }
}
