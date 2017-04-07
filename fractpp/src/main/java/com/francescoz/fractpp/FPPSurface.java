package com.francescoz.fractpp;

import android.content.Context;
import android.graphics.PixelFormat;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

public class FPPSurface {

    private final SurfaceView surfaceView;
    private final long jniObj;
    private boolean disposed;

    public FPPSurface(Context context, FPPConfig config, FPPBridge bridge) {
        System.loadLibrary("lib-fpp");
        jniObj = jniInit(bridge);
        surfaceView = new SurfaceView(context);
        if (config.alphaPrecision.isTranslucent()) {
            surfaceView.setZOrderOnTop(true);
            surfaceView.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        }
        surfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder holder) {

            }

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {

            }
        });
    }

    private static native long jniInit(FPPBridge bridge);

    private static native void jniSetSurface(long obj, Surface surface);

    private static native void jniDispose(long obj);

    public void ensureDisposed() {
        if (!disposed)
            dispose();
    }

    public boolean isDisposed() {
        return disposed;
    }

    public View getView() {
        return surfaceView;
    }

    public void dispose() {
        if (disposed)
            throw new IllegalStateException("Surface is already disposed");
        disposed = true;

    }

}
