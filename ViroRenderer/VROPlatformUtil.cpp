//
//  VROPlatformUtil.cpp
//  ViroRenderer
//
//  Created by Raj Advani on 11/7/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#include "VROPlatformUtil.h"
#include "VROLog.h"

#if VRO_PLATFORM_IOS

#import <UIKit/UIKit.h>
#import "VROImageiOS.h"

std::string VROPlatformGetPathForResource(std::string resource, std::string type) {
    NSBundle *bundle = [NSBundle bundleWithIdentifier:@"com.viro.ViroKit"];
    NSString *path = [bundle pathForResource:[NSString stringWithUTF8String:resource.c_str()]
                                      ofType:[NSString stringWithUTF8String:type.c_str()]];
    
    return std::string([path UTF8String]);
}

std::string VROPlatformLoadFileAsString(std::string path) {
    return std::string([[NSString stringWithContentsOfFile:[NSString stringWithUTF8String:path.c_str()]
                                                  encoding:NSUTF8StringEncoding
                                                     error:nil] UTF8String]);
}

std::string VROPlatformLoadResourceAsString(std::string resource, std::string type) {
    return VROPlatformLoadFileAsString(VROPlatformGetPathForResource(resource, type));
}

NSURLSessionDataTask *downloadDataWithURLSynchronous(NSURL *url,
                                                     void (^completionBlock)(NSData *data, NSError *error)) {
    
    dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
    
    NSURLSessionConfiguration *sessionConfig = [NSURLSessionConfiguration defaultSessionConfiguration];
    sessionConfig.timeoutIntervalForRequest = 30;
    
    NSURLSession *downloadSession = [NSURLSession sessionWithConfiguration: sessionConfig];
    NSURLSessionDataTask * downloadTask = [downloadSession dataTaskWithURL:url
                                                         completionHandler:^(NSData *data, NSURLResponse *response, NSError *error){
                                                             completionBlock(data, error);
                                                             dispatch_semaphore_signal(semaphore);
                                                         }];
    [downloadTask resume];
    dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
    
    return downloadTask;
}

NSURLSessionDataTask *downloadDataWithURL(NSURL *url, void (^completionBlock)(NSData *data, NSError *error)) {
    NSURLSessionConfiguration *sessionConfig = [NSURLSessionConfiguration defaultSessionConfiguration];
    sessionConfig.timeoutIntervalForRequest = 30;
    
    NSURLSession *downloadSession = [NSURLSession sessionWithConfiguration: sessionConfig];
    NSURLSessionDataTask * downloadTask = [downloadSession dataTaskWithURL:url
                                                         completionHandler:^(NSData *data, NSURLResponse *response, NSError *error){
                                                             completionBlock(data, error);
                                                         }];
    [downloadTask resume];
    return downloadTask;
}

std::string VROPlatformDownloadURLToFile(std::string url, bool *temp) {
    NSURL *URL = [NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]];
    
    __block NSString *tempFilePath;
    
    downloadDataWithURLSynchronous(URL, ^(NSData *data, NSError *error) {
        if (error.code == NSURLErrorCancelled){
            return;
        }
        
        if (!error) {
            NSString *fileName = [NSString stringWithFormat:@"%@.tmp", [[NSProcessInfo processInfo] globallyUniqueString]];
            NSURL *fileURL = [NSURL fileURLWithPath:[NSTemporaryDirectory() stringByAppendingPathComponent:fileName]];
            
            [data writeToURL:fileURL atomically:NO];
            
            *temp = true;
            tempFilePath = [fileURL path];
        }
    });
    
    return std::string([tempFilePath UTF8String]);
}

void VROPlatformDeleteFile(std::string filename) {
    NSError *deleteError = nil;
    [[NSFileManager defaultManager] removeItemAtPath:[NSString stringWithUTF8String:filename.c_str()]
                                               error:&deleteError];
}

std::shared_ptr<VROImage> VROPlatformLoadImageFromFile(std::string filename) {
    UIImage *image = [UIImage imageNamed:[NSString stringWithUTF8String:filename.c_str()]];
    return std::make_shared<VROImageiOS>(image);
}

#elif VRO_PLATFORM_ANDROID

#include "VROImageAndroid.h"

// We can hold a static reference to the JVM and to global references, but not to individual
// JNIEnv objects, as those are thread-local. Access the JNIEnv object via getJNIEnv().
// There is one JavaVM per application on Android (shared across activities).
static JavaVM *sVM = nullptr;
static jobject sActivity = nullptr;
static jobject sJavaAssetMgr = nullptr;
static jobject sPlatformUtil = nullptr;
static AAssetManager *sAssetMgr = nullptr;

// Get the JNI Environment for the current thread. If the JavaVM is not yet attached to the
// current thread, attach it
void getJNIEnv(JNIEnv **jenv) {
    if (sVM->GetEnv((void **) jenv, JNI_VERSION_1_6) == JNI_EDETACHED) {
        sVM->AttachCurrentThread(jenv, nullptr);
    }
}

void VROPlatformSetEnv(JNIEnv *env, jobject activity, jobject assetManager, jobject platformUtil) {
    env->GetJavaVM(&sVM);
    sActivity = env->NewGlobalRef(activity);
    sJavaAssetMgr = env->NewGlobalRef(assetManager);
    sPlatformUtil = env->NewGlobalRef(platformUtil);
    sAssetMgr = AAssetManager_fromJava(env, assetManager);
}

void VROPlatformSetEnv(JNIEnv *env) {
    env->GetJavaVM(&sVM);
}

JNIEnv *VROPlatformGetJNIEnv() {
    JNIEnv *env;
    getJNIEnv(&env);

    return env;
}

jobject VROPlatformGetJavaAssetManager() {
    return sJavaAssetMgr;
}

AAssetManager *VROPlatformGetAssetManager() {
    return sAssetMgr;
}

void VROPlatformReleaseEnv() {
    JNIEnv *env;
    getJNIEnv(&env);

    env->DeleteGlobalRef(sActivity);
    env->DeleteGlobalRef(sJavaAssetMgr);
    env->DeleteGlobalRef(sPlatformUtil);

    sActivity = nullptr;
    sJavaAssetMgr = nullptr;
    sAssetMgr = nullptr;
}

std::string VROPlatformLoadFileAsString(std::string path) {
    pabort();
    return "";
}

std::string VROPlatformLoadResourceAsString(std::string resource, std::string type) {
    std::string assetName = resource + "." + type;

    AAsset *asset = AAssetManager_open(sAssetMgr, assetName.c_str(), AASSET_MODE_BUFFER);
    size_t length = AAsset_getLength(asset);
    
    char *buffer = (char *)malloc(length + 1);
    AAsset_read(asset, buffer, length);
    buffer[length] = 0;
    
    std::string str(buffer);
    AAsset_close(asset);
    free(buffer);

    return str;
}

std::string VROPlatformDownloadURLToFile(std::string url, bool *temp) {
    JNIEnv *env = VROPlatformGetJNIEnv();
    jstring jurl = env->NewStringUTF(url.c_str());

    jclass cls = env->GetObjectClass(sPlatformUtil);
    jmethodID jmethod = env->GetMethodID(cls, "downloadURLToTempFile", "(Ljava/lang/String;)Ljava/lang/String;");
    jstring jpath = (jstring) env->CallObjectMethod(sPlatformUtil, jmethod, jurl);

    const char *path = env->GetStringUTFChars(jpath, 0);
    std::string spath(path);

    pinfo("Path to file is %s", path);

    env->ReleaseStringUTFChars(jpath, path);
    env->DeleteLocalRef(jpath);
    env->DeleteLocalRef(cls);

    *temp = true;
    return spath;
}

void VROPlatformDeleteFile(std::string filename) {
    JNIEnv *env = VROPlatformGetJNIEnv();
    jstring jfilename = env->NewStringUTF(filename.c_str());

    jclass cls = env->GetObjectClass(sPlatformUtil);
    jmethodID jmethod = env->GetMethodID(cls, "deleteFile", "(Ljava/lang/String;)V");
    env->CallVoidMethod(sPlatformUtil, jmethod, jfilename);

    env->DeleteLocalRef(jfilename);
    env->DeleteLocalRef(cls);
}

std::string VROPlatformCopyAssetToFile(std::string asset) {
    JNIEnv *env = VROPlatformGetJNIEnv();
    jstring jasset = env->NewStringUTF(asset.c_str());

    jclass cls = env->GetObjectClass(sPlatformUtil);
    jmethodID jmethod = env->GetMethodID(cls, "copyAssetToFile", "(Ljava/lang/String;)Ljava/lang/String;");
    jstring jpath = (jstring) env->CallObjectMethod(sPlatformUtil, jmethod, jasset);

    const char *path = env->GetStringUTFChars(jpath, 0);
    std::string spath(path);

    pinfo("Path to file is %s", path);

    env->ReleaseStringUTFChars(jpath, path);
    env->DeleteLocalRef(jpath);
    env->DeleteLocalRef(cls);

    return spath;
}

std::shared_ptr<VROImage> VROPlatformLoadImageFromFile(std::string filename) {
    jobject bitmap = VROPlatformLoadBitmapFromFile(filename);
    return std::make_shared<VROImageAndroid>(bitmap);
}

std::shared_ptr<VROImage> VROPlatformLoadImageFromAsset(std::string asset) {
    jobject bitmap = VROPlatformLoadBitmapFromAsset(asset);
    return std::make_shared<VROImageAndroid>(bitmap);
}

jobject VROPlatformLoadBitmapFromAsset(std::string asset) {
    JNIEnv *env;
    getJNIEnv(&env);

    jclass cls = env->GetObjectClass(sPlatformUtil);
    jmethodID jmethod = env->GetMethodID(cls, "loadBitmapFromAsset", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");

    jstring string = env->NewStringUTF(asset.c_str());
    jobject jbitmap = env->CallObjectMethod(sPlatformUtil, jmethod, string);

    env->DeleteLocalRef(string);
    env->DeleteLocalRef(cls);
    return jbitmap;
}

jobject VROPlatformLoadBitmapFromFile(std::string path) {
    JNIEnv *env;
    getJNIEnv(&env);

    jclass cls = env->GetObjectClass(sPlatformUtil);
    jmethodID jmethod = env->GetMethodID(cls, "loadBitmapFromFile", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");

    jstring string = env->NewStringUTF(path.c_str());
    jobject jbitmap = env->CallObjectMethod(sPlatformUtil, jmethod, string);

    env->DeleteLocalRef(string);
    env->DeleteLocalRef(cls);
    return jbitmap;
}

void *VROPlatformConvertBitmap(jobject jbitmap, int *bitmapLength, int *width, int *height) {
    JNIEnv *env;
    getJNIEnv(&env);

    AndroidBitmapInfo bitmapInfo;
    AndroidBitmap_getInfo(env, jbitmap, &bitmapInfo);

    passert (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888);

    *width = bitmapInfo.width;
    *height = bitmapInfo.height;
    *bitmapLength = bitmapInfo.height * bitmapInfo.stride;

    void *bitmapData;
    AndroidBitmap_lockPixels(env, jbitmap, &bitmapData);

    void *safeData = malloc(*bitmapLength);
    memcpy(safeData, bitmapData, *bitmapLength);

    AndroidBitmap_unlockPixels(env, jbitmap);

    env->DeleteLocalRef(jbitmap);
    return safeData;
}

jobject VROPlatformCreateVideoSink(int textureId) {
    JNIEnv *env;
    getJNIEnv(&env);

    jclass cls = env->GetObjectClass(sActivity);
    jmethodID jmethod = env->GetMethodID(cls, "createVideoSink", "(I)Landroid/view/Surface;");
    jobject jsurface = env->CallObjectMethod(sActivity, jmethod, textureId);

    env->DeleteLocalRef(cls);
    return jsurface;
}

void VROPlatformDestroyVideoSink(int textureId) {
    JNIEnv *env;
    getJNIEnv(&env);

    jclass cls = env->GetObjectClass(sActivity);
    jmethodID jmethod = env->GetMethodID(cls, "destroyVideoSink", "(I)V");
    env->CallVoidMethod(sActivity, jmethod, textureId);

    env->DeleteLocalRef(cls);
}

int VROPlatformGetAudioSampleRate() {
    JNIEnv *env;
    getJNIEnv(&env);

    jclass cls = env->GetObjectClass(sPlatformUtil);
    jmethodID jmethod = env->GetMethodID(cls, "getAudioSampleRate", "()I");
    jint sampleRate = env->CallIntMethod(sPlatformUtil, jmethod);

    env->DeleteLocalRef(cls);
    return sampleRate;
}

int VROPlatformGetAudioBufferSize() {
    JNIEnv *env;
    getJNIEnv(&env);

    jclass cls = env->GetObjectClass(sPlatformUtil);
    jmethodID jmethod = env->GetMethodID(cls, "getAudioBufferSize", "()I");
    jint bufferSize = env->CallIntMethod(sPlatformUtil, jmethod);

    env->DeleteLocalRef(cls);
    return bufferSize;
}

#endif
