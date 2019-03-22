package com.heqijun.jnitest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
//        tv.setText(String.format("%s%s", stringFromJNI(), messageFromJNI()));
//        tv.setText(messageFromJNI2("I am from Java"));
//        tv.setText(_test());
        tv.setText(stringFromJNI2());
    }

    /****************************Java调用C/C++**********************************************/
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    //方式一
    public native String stringFromJNI();
    public native String messageFromJNI();
    public native String messageFromJNI2(String s);

    //方式二
    public native String _test();

    /****************************C/C++调用Java**********************************************/
    public native String stringFromJNI2();
}
