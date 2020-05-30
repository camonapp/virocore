//
//  VROTimingFunction.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/22/15.
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

#ifndef VROTIMINGFUNCTION_H_
#define VROTIMINGFUNCTION_H_

#include <memory>

enum class VROTimingFunctionType {
    Linear,
    EaseIn,
    EaseOut,
    EaseInEaseOut,
    Bounce,
    PowerDecel,

    CubicIn,
    CubicOut,
    CubicInOut,
    CubicOutIn,
    CubicBackIn,
    CubicBackOut,
    CuarticBackIn,
    CuarticBackOut,
    ElasticInSmall,
    ElasticOutSmall,
    ElasticInBig,
    ElasticOutBig
};

class VROTimingFunction {
    
public:
    
    static std::unique_ptr<VROTimingFunction> forType(VROTimingFunctionType type);

    VROTimingFunction() {}
    virtual ~VROTimingFunction() {}

    virtual float getT(float t, float from, float to) = 0;
    
};

#endif /* VROTIMINGFUNCTION_H_ */
