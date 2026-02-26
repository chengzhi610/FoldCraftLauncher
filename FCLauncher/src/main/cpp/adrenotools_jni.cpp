#include <jni.h>
#include <android/log.h>
#include <fcntl.h>
#include <unistd.h>

#define LOG_TAG "adrenotools_jni"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern "C" {

JNIEXPORT void JNICALL
Java_com_tungsten_fclauncher_FCLauncher_initAdrenotools(JNIEnv *env, jclass clazz,
                                                         jstring nativeLibDir,
                                                         jstring sdcardDriverDir) {
    // 写入文件以验证是否被调用
    int fd = open("/data/data/com.tungsten.fcl.debug/adrenotools_test.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd != -1) {
        write(fd, "initAdrenotools called\n", 24);
        close(fd);
    }

    const char *native_dir = env->GetStringUTFChars(nativeLibDir, nullptr);
    const char *sdcard_dir = env->GetStringUTFChars(sdcardDriverDir, nullptr);

    LOGI("initAdrenotools called with native_dir=%s, sdcard_dir=%s", native_dir, sdcard_dir);

    env->ReleaseStringUTFChars(nativeLibDir, native_dir);
    env->ReleaseStringUTFChars(sdcardDriverDir, sdcard_dir);
}

} // extern "C"