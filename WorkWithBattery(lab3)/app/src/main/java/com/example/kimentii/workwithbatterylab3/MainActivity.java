package com.example.kimentii.workwithbatterylab3;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.BatteryManager;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    Button createService;
    Button destroyService;
    TextView textView;
    SeekBar seekBar;

    public class Listener implements View.OnClickListener {

        @Override
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.create_button:
                    Log.d("myTag", "Create Button was pressed");
                    startService(new Intent(getApplicationContext(), MyService.class));
                    break;
                case R.id.destroy_bytton:
                    stopService(new Intent(getApplicationContext(), MyService.class));
                    break;
            }
        }
    }

    public void seekBarInit() {
        int Brightness;
        Brightness = Settings.System.getInt(getApplicationContext().getContentResolver(), Settings.System.SCREEN_BRIGHTNESS, 0);
        seekBar.setProgress(Brightness);
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                Settings.System.putInt(getApplicationContext().getContentResolver(), Settings.System.SCREEN_BRIGHTNESS, progress);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        createService = (Button) findViewById(R.id.create_button);
        destroyService = (Button) findViewById(R.id.destroy_bytton);
        Listener listener = new Listener();
        createService.setOnClickListener(listener);
        destroyService.setOnClickListener(listener);
        textView = (TextView) findViewById(R.id.battary);
        seekBar = (SeekBar) findViewById(R.id.seekBar);
        seekBarInit();
        BroadcastReceiver receiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                int status = intent.getIntExtra(BatteryManager.EXTRA_LEVEL, -1);
                textView.setText(String.valueOf(status));
            }
        };

        registerReceiver(receiver, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
        Log.d("myTag", "Application was started");
    }
}
