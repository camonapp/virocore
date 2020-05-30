//
//  VROTimingFunction.cpp
//  ViroRenderer
//
//  Created by Raj Advani on 1/14/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "VROTimingFunction.h"
#include "VROTimingFunctionEaseIn.h"
#include "VROTimingFunctionEaseOut.h"
#include "VROTimingFunctionEaseInEaseOut.h"
#include "VROTimingFunctionBounce.h"
#include "VROTimingFunctionPowerDeceleration.h"
#include "VROTimingFunctionLinear.h"
#include "VROTimingFunctionCubicIn.h"
#include "VROTimingFunctionCubicOut.h"
#include "VROTimingFunctionCubicInOut.h"
#include "VROTimingFunctionCubicOutIn.h"
#include "VROTimingFunctionCubicBackIn.h"
#include "VROTimingFunctionCubicBackOut.h"
#include "VROTimingFunctionCuarticBackIn.h"
#include "VROTimingFunctionCuarticBackOut.h"
#include "VROTimingFunctionElasticInSmall.h"
#include "VROTimingFunctionElasticInBig.h"
#include "VROTimingFunctionElasticOutBig.h"
#include "VROTimingFunctionElasticOutSmall.h"

#include "VROLog.h"

std::unique_ptr<VROTimingFunction> VROTimingFunction::forType(VROTimingFunctionType type) {
    switch (type) {
        case VROTimingFunctionType::Linear:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionLinear());
        case VROTimingFunctionType::EaseIn:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionEaseIn());
        case VROTimingFunctionType::EaseOut:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionEaseOut());
        case VROTimingFunctionType::EaseInEaseOut:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionEaseInEaseOut());
        case VROTimingFunctionType::Bounce:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionBounce());
        case VROTimingFunctionType::PowerDecel:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionPowerDeceleration());
        case VROTimingFunctionType::CubicIn:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionCubicIn());
        case VROTimingFunctionType::CubicOut:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionCubicOut());
        case VROTimingFunctionType::CubicInOut:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionCubicInOut());
        case VROTimingFunctionType::CubicOutIn:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionCubicOutIn());
        case VROTimingFunctionType::CubicBackIn:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionCubicBackIn());
        case VROTimingFunctionType::CubicBackOut:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionCubicBackOut());
        case VROTimingFunctionType::CuarticBackIn:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionCuarticBackIn());
        case VROTimingFunctionType::CuarticBackOut:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionCuarticBackOut());
        case VROTimingFunctionType::ElasticInSmall:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionElasticInSmall());
        case VROTimingFunctionType::ElasticOutSmall:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionElasticOutSmall());
        case VROTimingFunctionType::ElasticInBig:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionElasticInBig());
        case VROTimingFunctionType::ElasticOutBig:
            return std::unique_ptr<VROTimingFunction>(new VROTimingFunctionElasticOutBig());
        default:
            pabort();
    }
}

