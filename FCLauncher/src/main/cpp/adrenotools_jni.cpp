#include <jni.h>
#include <string>
#include <adrenotools/driver.h>
#include <android/log.h>
#include <fcntl.h>
#include <unistd.h>

#define LOG_TAG "adrenotools_jni"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

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

    const char *tmpDir = getenv("TMPDIR");
    if (!tmpDir) tmpDir = "/data/local/tmp";

    void *handle = adrenotools_open_libvulkan(
        RTLD_NOW,
        1,                      // ADRENOTOOLS_DRIVER_CUSTOM
        tmpDir,
        native_dir,             // hookLibDir
        sdcard_dir,             // customDriverDir
        "libvulkan_turnip.so",  // customDriverName
        nullptr,
        nullptr
    );

    if (handle) {
        LOGI("adrenotools_open_libvulkan SUCCESS");
    } else {
        LOGE("adrenotools_open_libvulkan FAILED");
    }

    env->ReleaseStringUTFChars(nativeLibDir, native_dir);
    env->ReleaseStringUTFChars(sdcardDriverDir, sdcard_dir);
}

} // extern "C" test