#include <jni.h>
#include <iostream>
#include <vector>
#include <string>
#include <utility>  // for std::pair

// JNI function to be called from Java
extern "C"
JNIEXPORT void JNICALL Java_BatchProcessor_passBatchDataToCpp
  (JNIEnv *env, jobject obj, jobject courseCreditsList) {

    // Get the class of the ArrayList
    jclass arrayListClass = env->GetObjectClass(courseCreditsList);

    // Get the method ID for the 'size' method of ArrayList
    jmethodID sizeMethod = env->GetMethodID(arrayListClass, "size", "()I");
    jint size = env->CallIntMethod(courseCreditsList, sizeMethod);

    // Create a C++ vector to store pairs
    std::vector<std::pair<std::string, int>> cppVector;

    // Iterate over the ArrayList
    jmethodID getMethod = env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");
    for (int i = 0; i < size; i++) {
        jobject entryObj = env->CallObjectMethod(courseCreditsList, getMethod, i);

        // Get the class of Map.Entry
        jclass entryClass = env->GetObjectClass(entryObj);

        // Get the method ID for the 'getKey' method
        jmethodID getKeyMethod = env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
        jstring keyObj = (jstring)env->CallObjectMethod(entryObj, getKeyMethod);
        const char* key = env->GetStringUTFChars(keyObj, 0);

        // Get the method ID for the 'getValue' method
        jmethodID getValueMethod = env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");
        jobject valueObj = env->CallObjectMethod(entryObj, getValueMethod);
        jint value = env->CallIntMethod(valueObj, env->GetMethodID(env->GetObjectClass(valueObj), "intValue", "()I"));

        // Push the pair to the vector
        cppVector.push_back(std::make_pair(std::string(key), value));

        // Release resources
        env->ReleaseStringUTFChars(keyObj, key);
    }

    // Print the C++ vector content
    std::cout << "Courses and Credits:" << std::endl;
    for (const auto& pair : cppVector) {
        std::cout << "Course: " << pair.first << ", Credits: " << pair.second << std::endl;
    }
}
