package com.heqijun.ffplayerdemo;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private TextView tv;
    private FFVideoView ffVideoView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        tv = findViewById(R.id.sample_text);
        ffVideoView=findViewById(R.id.videoView);

    }

    public void showProtocol(View view) {
        tv.setText(FFUtils.urlProtocolInfo());
    }

    public void showFormat(View view) {
        tv.setText(FFUtils.avFormatInfo());
    }

    public void showCodec(View view) {
        tv.setText(FFUtils.avCodecInfo());
    }

    public void showFilter(View view) {
        tv.setText(FFUtils.avFilterInfo());
    }

    public void play(View view) {
        ffVideoView.playVideo("");
    }
}
