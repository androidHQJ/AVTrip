#include <jni.h>
#include <string>

#define CLASS_PATH "com/heqijun/jnitest/MainActivity"
#define CLASS_PATH2 "com/heqijun/jnitest/Student"

/****************************Java调用C/C++ 方式一**********************************************/
extern "C" JNIEXPORT jstring JNICALL
Java_com_heqijun_jnitest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_heqijun_jnitest_MainActivity_messageFromJNI(JNIEnv *env, jobject instance) {
    // TODO
    return env->NewStringUTF(",哈哈哈");
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_heqijun_jnitest_MainActivity_messageFromJNI2(JNIEnv *env, jobject instance, jstring s_) {
    const char *s = env->GetStringUTFChars(s_, 0);
    env->ReleaseStringUTFChars(s_, s);//释放资源
    return env->NewStringUTF(s);
}

/****************************Java调用C/C++ 方式二：JNI_OnLoad 机制**********************************************/

//2.实现一个对应native的函数
extern "C"
JNIEXPORT jstring JNICALL
test_register(JNIEnv *env, jobject instance) {
    return env->NewStringUTF("this is a test of register");
}

//3.定义一个方法映射表
static JNINativeMethod  g_methods[]={
        {
                "_test",
                "()Ljava/lang/String;",
                (reinterpret_cast<void**>(test_register))
        }
};

jint JNI_OnLoad(JavaVM* vm, void* reserved){
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        abort();
    }
    jclass clazz=env-> FindClass(CLASS_PATH);
    env->RegisterNatives(clazz,g_methods, sizeof(g_methods)/ sizeof(g_methods[0]));

    return JNI_VERSION_1_6;
}

/******************C/C++调用Java***************************/
extern "C"
JNIEXPORT jstring JNICALL
Java_com_heqijun_jnitest_MainActivity_stringFromJNI2(JNIEnv *env, jobject instance) {
    // TODO
    //step1:
    jclass  clazz=env->FindClass(CLASS_PATH2);
    //step2:
    jmethodID method_init_id=env->GetMethodID(clazz,"<init>","()V");//获取构造方法
    jmethodID method_set_id=env->GetMethodID(clazz,"setYear","(I)V");//获取set方法
    jmethodID method_get_id=env->GetMethodID(clazz,"getYear","()I");//获取get方法

    //step3:
    jobject  obj=env->NewObject(clazz,method_init_id);

    //step4:
    env->CallVoidMethod(obj,method_set_id,18);
    int year=env->CallIntMethod(obj,method_get_id);

    char temp[]={0, };
    sprintf(temp,"%d",year);

    std::string hello = "Hello from C++,year=";
    hello.append(temp);

    return env->NewStringUTF(hello.c_str());
}