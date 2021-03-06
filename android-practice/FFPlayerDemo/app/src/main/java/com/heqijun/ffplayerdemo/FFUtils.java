package com.heqijun.ffplayerdemo;

import android.view.Surface;

public class FFUtils {

    static {
        System.loadLibrary("native-lib");
    }

    public static native String urlProtocolInfo();

    public static native String avFormatInfo();

    public static native String avCodecInfo();

    public static native String avFilterInfo();

    public static native void playVideo(String videoPath, Surface surface);
}
