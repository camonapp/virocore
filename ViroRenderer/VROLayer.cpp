//
//  VROLayer.cpp
//  ViroRenderer
//
//  Created by Raj Advani on 10/13/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#include "VROLayer.h"
#include "VROMath.h"
#include "VROSurface.h"
#include "VROTexture.h"
#include "VROMaterial.h"
#include "VROTextureSubstrateMetal.h"

#pragma mark - Initialization

VROLayer::VROLayer() :
    VRONode() {
    
    std::shared_ptr<VROSurface> surface = VROSurface::createSurface(1.0, 1.0);
    surface->getMaterials().front()->setLightingModel(VROLightingModel::Lambert);
        
    setGeometry(surface);
}

VROLayer::~VROLayer() {

}

#pragma mark - Layer Properties

void VROLayer::setContents(UIImage *image) {
    getMaterial()->getDiffuse().setContents(std::make_shared<VROTexture>(image));
}

void VROLayer::setContents(int width, int height, CGContextRef bitmapContext,
                           const VRORenderContext &context) {
    
    std::unique_ptr<VROTextureSubstrate> substrate = std::unique_ptr<VROTextureSubstrateMetal>(
                    new VROTextureSubstrateMetal(width, height, bitmapContext, context));
    
    getMaterial()->getDiffuse().setContents(std::make_shared<VROTexture>(VROTextureType::Quad, std::move(substrate)));
}

std::shared_ptr<VROMaterial> VROLayer::getMaterial() {
    return getGeometry()->getMaterials().front();
}

#pragma mark - Spatial Position

void VROLayer::setFrame(VRORect frame) {
    _frame = frame;
    onFrameUpdate();
}

void VROLayer::setPosition(VROVector3f point) {
    _frame.origin.x = point.x - _frame.size.width  / 2.0f;
    _frame.origin.y = point.y - _frame.size.height / 2.0f;
    _frame.origin.z = point.z;
    
    VRONode::setPosition(point);
}

void VROLayer::onFrameUpdate() {
    VRONode::setPosition({ _frame.origin.x + _frame.size.width / 2.0f,
                           _frame.origin.y + _frame.size.height / 2.0f,
                           _frame.origin.z });
    VRONode::setScale( { _frame.size.width, _frame.size.height, 1.0 });
}

VRORect VROLayer::getFrame() const {
    return _frame;
}

