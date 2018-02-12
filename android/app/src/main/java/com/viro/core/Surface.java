/*
 * Copyright (c) 2017-present, ViroMedia, Inc.
 * All rights reserved.
 */

/*
 * Java JNI wrapper for linking the following classes below across the bridge.
 *
 * Java JNI Wrapper     : com.viro.renderer.jni.SurfaceJni.java
 * Cpp JNI wrapper      : Surface_JNI.cpp
 * Cpp Object           : VROSurface.cpp
 */
package com.viro.core;

/**
 * Surface represents a one-sided plane defined by a width and height.
 */
public class Surface extends Geometry {

    private float mWidth, mHeight;

    /**
     * Construct a new Surface with the given width and height.
     *
     * @param width  The extent of the Surface along its horizontal axis (X).
     * @param height The extent of the Surface along its vertical axis (Y).
     */
    public Surface(float width, float height) {
        this(width, height, 0, 0, 1, 1);
    }


    /**
     * Construct a new Surface with custom texture coordinates. Texture coordinates
     * specify how to tile a {@link Texture} across the Surface geometry.
     *<p>
     * Texture coordinates are represented by a 2D UV axis(analogous to an x,y axis).
     * Texture coords are specified like percentages - the left edge of a texture is u = 0 and the right edge of the texture is u = 1.0.
     * The top of the texture is v = 0 and the bottom is v = 1.0.
     * <p>
     * Specifying greater than 1 on the U or V axis will cause the tile to repeat if the Surface {@link Material materials} {@link Texture texture}
     * {@link Texture#setWrapS(Texture.WrapMode)} and {@link Texture#setWrapT(Texture.WrapMode)}
     * is set to Texture.WrapMode.REPEAT. Specifying less than 1 on the U or V axis will render that texture partially over the entire surface.
     *
     * <p>
     * For example, specifying u0,v0 as (0,0) and (u1,v2) as (2,2) will tile the Texture twice over the width and height of the surface, effectively
     * displaying 4 textures on the surface. Doing the opposite, setting (u1,v1) as (0.5, 0.5) will display a quarter of the texture
     * over the entire surface.
     * <p>
     * @param width The extent of the Surface along its horizontal axis (X).
     * @param height The extent of the Surface along its vertical axis (Y).
     * @param u0 The texture coord that specifies the start {@link Texture} left edge.
     * @param v0 The texture coord that specifies the start {@link Texture} top edge.
     * @param u1 The texture coord that specifies the end {@link Texture} left edge.
     * @param v1 The texture coord that specifies the end {@link Texture} top edge.
     */
    public Surface(float width, float height, float u0, float v0, float u1, float v1) {
        mWidth = width;
        mHeight = height;
        mNativeRef = nativeCreateSurface(width, height, u0, v0, u1, v1);
    }

    /**
     * @hide
     */
    public Surface(float width, float height, float u0, float v0, float u1, float v1,
                   Surface oldSurface) {
        mNativeRef = nativeCreateSurfaceFromSurface(width, height, u0, v0, u1, v1,
                oldSurface.mNativeRef);
    }

    protected void finalize() throws Throwable {
        try {
            dispose();
        } finally {
            super.finalize();
        }
    }

    /**
     * Release native resources associated with this Surface.
     */
    public void dispose(){
        if (mNativeRef != 0) {
            nativeDestroySurface(mNativeRef);
            mNativeRef = 0;
        }
    }

    /**
     * Set the width of this Surface.
     *
     * @param width The extent of the Surface along its horizontal axis.
     */
    public void setWidth(float width) {
        mWidth = width;
        nativeSetWidth(mNativeRef, width);
    }

    /**
     * Get the width of this Surface.
     *
     * @return The extent of this Surface along its horizontal axis.
     */
    public float getWidth() {
        return mWidth;
    }

    /**
     * Set the height of this Surface.
     *
     * @param height The extent of the Surface along its vertical axis.
     */
    public void setHeight(float height) {
        mHeight = height;
        nativeSetHeight(mNativeRef, height);
    }

    /**
     * Get the height of this surface.
     *
     * @return The extent of this Surface along its vertical axis.
     */
    public float getHeight() {
        return mHeight;
    }

    private native long nativeCreateSurface(float width, float height, float u0, float v0, float u1, float v1);
    private native long nativeCreateSurfaceFromSurface(float width, float height,
                                                       float u0, float v0, float u1, float v1,
                                                       long oldSurfaceRef);
    private native void nativeDestroySurface(long surfaceRef);
    private native void nativeSetWidth(long surfaceRef, float width);
    private native void nativeSetHeight(long surfaceRef, float height);
    private native void nativeSetVideoTexture(long surfaceRef, long textureRef);
    private native void nativeSetImageTexture(long surfaceRef, long textureRef);
    private native void nativeSetMaterial(long surfaceRef, long materialRef);
    private native void nativeClearMaterial(long surfaceRef);

    /**
     * @hide
     * @param texture
     */
    public void setVideoTexture(VideoTexture texture){
        nativeSetVideoTexture(mNativeRef, texture.mNativeRef);
    }
    /**
     * @hide
     * @param texture
     */
    public void setImageTexture(Texture texture) {
        nativeSetImageTexture(mNativeRef, texture.mNativeRef);
    }
    /**
     * @hide
     * @param material
     */
    public void setMaterial(Material material) {
        nativeSetMaterial(mNativeRef, material.mNativeRef);
    }
    /**
     * @hide
     */
    public void clearMaterial() {
        nativeClearMaterial(mNativeRef);
    }

}
