//
//  LazyMaterial_JNI.cpp
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//


#include "LazyMaterial_JNI.h"

#if VRO_PLATFORM_ANDROID
#define VRO_METHOD(return_type, method_name) \
  JNIEXPORT return_type JNICALL              \
      Java_com_viro_core_internal_LazyMaterial_##method_name
#else
#define VRO_METHOD(return_type, method_name) \
    return_type LazyMaterial_##method_name
#endif

extern "C" {

VRO_METHOD(VRO_REF(VROLazyMaterialJNI), nativeCreateLazyMaterial)(VRO_NO_ARGS) {
    std::shared_ptr<VROLazyMaterialJNI> materialPtr = std::make_shared<VROLazyMaterialJNI>(obj);
    return VRO_REF_NEW(VROLazyMaterialJNI, materialPtr);
}

VRO_METHOD(void, nativeDestroyLazyMaterial)(VRO_ARGS
                                            VRO_REF(VROLazyMaterialJNI) nativeRef) {
    VRO_REF_DELETE(VROLazyMaterialJNI, nativeRef);
}

}  // extern "C"