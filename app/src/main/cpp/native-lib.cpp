#include "com_example_jniqq_MainActivity.h"
#include <jni.h>
#include <string>
#include <android/log.h>
#include <unistd.h>
#include <android/log.h>
#include <pthread.h>

#define TAG "JNISTUDY"
#define LOGD(...)__android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__);
#define LOGE(...)__android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__);


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jniqq_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
using namespace FMOD;


extern "C"//c的标准
JNIEXPORT void JNICALL Java_com_example_jniqq_MainActivity_voiceChange
        (JNIEnv *env, jobject, jint mode, jstring path) {

    char *content = "默认播放完毕";
    const char *path_ = env->GetStringUTFChars(path, NULL);
//    音效引擎系统 Java对象 c、指针
//
    System *system1 = 0;
    Sound *sound = 0;
    //通道 音轨 声音跑道
    Channel *channel = 0;
//    dsp
    DSP *dsp = 0;

//java 思想 初始化
//    c

    System_Create(&system1);

//  第二步：   最大音轨数 初始化标记
    system1->init(32, FMOD_INIT_NORMAL, 0);

//  第三步 创建声音 路劲、初始化标记 额外数据 声音指针
    system1->createSound(path_, FMOD_DEFAULT, 0, &sound);

//  第四步：播放 声音、分组 控制、通道
    system1->playSound(sound, 0, false, &channel);

    switch (mode) {
        case com_example_jniqq_MainActivity_MODEL_NORMAL:
            content = "原生播放完毕";
            break;
        case com_example_jniqq_MainActivity_MODEL_LUOLI:
            content = "萝莉播放完毕";
            //创建dsp类型
            system1->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            //设置pitch 调节2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 2.0f);
            //添加音效 音轨
            channel->addDSP(0, dsp);

            break;
        case com_example_jniqq_MainActivity_MODEL_DASHU:
            system1->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            //设置pitch 调节2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7f);
            //添加音效 音轨
            channel->addDSP(0, dsp);
            content = "大叔播放完毕";
            break;
        case com_example_jniqq_MainActivity_MODEL_GAOGUAI:
            content = "搞怪播放完毕";

            break;
        case com_example_jniqq_mainactivity_model_XIAOHUANGREN:
            float mFrequency;
            channel->getFrequency(&mFrequency);
            channel->setFrequency(mFrequency * 1.5f);//频率加快
            break;
        case com_example_jniqq_mainactivity_model_KONGLING:
            system1->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 200);
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 10);//回音衰减
            channel->addDSP(0, dsp);
            break;
        case com_example_jniqq_mainactivity_model_JINGSONG:
//            音调低
            system1->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            //设置pitch 调节2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7f);
            //添加音效 音轨
            channel->addDSP(0, dsp);
//回声
//颤抖
            break;
    }
//    等待播放完毕
    bool isPlayer = true;//不是一级指针
    while (isPlayer) {
        channel->isPlaying(&isPlayer);//如果真的播放完成
        usleep(1000 * 1000);
    }
    sound->release();
    system1->close();
    system1->release();
    env->ReleaseStringUTFChars(path, path_);
}

JavaVM *javaVm = nullptr;//c++ 11取代null
const char *mainActivityClassName = "com/example/jniqq/MainActivity";

const char *javaMethod = "dynamicMethod";
const char *javaMethod1 = "dynamicMethod1";

//native 真正的函数
void dynamicMethod_c(JNIEnv *jniEnv) {
    LOGD("我是动态注册的函数");
}

int dynamicMethod1_c(JNIEnv *jniEnv, jobject jobject1, jstring value) {

//    JNIEnv.
//    LOGD("%s",);
    const char *string = jniEnv->GetStringUTFChars(value, NULL);
    LOGD("我是动态注册的函数11111%s", string);
    jniEnv->ReleaseStringUTFChars(value, string);
    return 200;
}

/**
   *
    typedef struct {
          const char* name;//函数名
          const char* signature;//函数签名
          void*       fnPtr;//函数指针
      } JNINativeMethod;
   *
   */
static const JNINativeMethod jniNativeMethods[] = {
        {
                javaMethod,  "()V",                   (void *) (dynamicMethod_c)
        },
        {
                javaMethod1, "(Ljava/lang/String;)I", (int *) (dynamicMethod1_c)
        }
};

//动态注册
//相当于 java 构造函数 携带Java虚拟机 System.loadLibrary("native-lib");就直接调用
JNIEXPORT jint JNI_OnLoad(JavaVM *javaVm, void *) {//有箭头代表对了
    //as的jdk默认1.6
    ::javaVm = javaVm;//相当于this.

    JNIEnv *jniEnv = nullptr;
    int result = javaVm->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6);
    if (result != JNI_OK) {//未成功 所有c库 成功就是0
        return -1;
    }
    LOGE("初始化成功");

//    
    jclass pJclass = jniEnv->FindClass(mainActivityClassName);
    jint i = jniEnv->RegisterNatives(pJclass,
                                     jniNativeMethods,
                                     sizeof(jniNativeMethods) / sizeof(JNINativeMethod));

    return JNI_VERSION_1_6;
}

class MyContext {
public:
    JNIEnv *jniEnv = nullptr;
    jobject instance = nullptr;

};


void *myThreadTaskAction(void *pVoid) {
    LOGE("myThreadRun");
    //下载完成 告诉activity UI、 所有需要在子线程调用UI

    MyContext *myContext = static_cast<MyContext *>(pVoid);
//    JNIEnv *pEnv = myContext->jniEnv;
//    jclass pJclass = pEnv->FindClass(mainActivityClassName);//崩溃
//
//    myContext->jniEnv->GetObjectClass(myContext->instance);//崩溃

//TODO 解决方式 JVM 绑定进程了 Android进程只有一个JVM
    JNIEnv *jniEnv = nullptr;//全新的env
    jint i = ::javaVm->AttachCurrentThread(&jniEnv, nullptr);//附加一个env 子线程专用
    if (i != JNI_OK) {
        return 0;//附加失败
    }
    jclass mainActivityClass = jniEnv->GetObjectClass(myContext->instance);
//    jmethodID updateActivityUI= mainActivityClass->GetMethodID(mainActivityClass,"updateActivityUI","()V");
    jmethodID updateActivityUI = jniEnv->GetMethodID(mainActivityClass,"updateActivityUI","()V");
    //    调用
    jniEnv->CallVoidMethod(myContext->instance,updateActivityUI);

    ::javaVm->DetachCurrentThread();//必须解除
    LOGE("没毛病");
    return nullptr;
}


/**
 * jnienv 不能跨越线程 否则崩溃，可以跨越函数
 * jobject 不能跨越线程和函数 【解决方式:默认是局部引用，提升为全局引用】
 * JavaVM能够跨越线程、能够跨越函数
 */
extern "C"
JNIEXPORT
void JNICALL Java_com_example_jniqq_MainActivity_nativeThread(JNIEnv *env, jobject jobject) {
    MyContext *myContext = new MyContext();
    myContext->jniEnv = env;
//    myContext->instance = jobject ;

    myContext->instance = env->NewGlobalRef(jobject);//提升为全局引用

    pthread_t pid;
    pthread_create(&pid, nullptr, myThreadTaskAction, myContext);
    pthread_join(pid, nullptr);
}


