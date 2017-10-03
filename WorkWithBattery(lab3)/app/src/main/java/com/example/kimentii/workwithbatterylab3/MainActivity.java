package com.example.kimentii.workwithbatterylab3;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
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

    public static final int MAX_VOLTAGE = 4200;
    public static final int MIN_VOLTAGE = 3600;

    public static final int EXTRA_MAX_VOLTAGE = 4300;
    public static final int EXTRA_MIN_VOLTAGE = 2900;

    Button createService;
    Button destroyService;
    TextView scale;
    TextView level;
    TextView plugged;
    TextView voltage;
    TextView time_left;
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
        scale = (TextView) findViewById(R.id.extraScale);
        level = (TextView) findViewById(R.id.extreLevel);
        plugged = (TextView) findViewById(R.id.extraPlagged);
        voltage = (TextView) findViewById(R.id.extraVoltage);
        seekBar = (SeekBar) findViewById(R.id.seekBar);
        time_left = (TextView) findViewById(R.id.time_left);
        seekBarInit();
        BroadcastReceiver receiver = new BroadcastReceiver() {

            long firstMeasurementVoltage;
            long secondMeasurementVoltage;
            long firstMeasurementTime;
            long secondMeasurementTime;

            @Override
            public void onReceive(Context context, Intent intent) {
                int status = intent.getIntExtra(BatteryManager.EXTRA_SCALE, -1);
                scale.setText(String.valueOf(status));
                status = intent.getIntExtra(BatteryManager.EXTRA_LEVEL, -1);
                level.setText(String.valueOf(status));
                status = intent.getIntExtra(BatteryManager.EXTRA_PLUGGED, -1);
                plugged.setText(String.valueOf(status));
                status = intent.getIntExtra(BatteryManager.ACTION_DISCHARGING, -1);
                /*if (firstMeasurementTime == 0) {
                    firstMeasurementTime = System.currentTimeMillis();
                    firstMeasurementVoltage = status;
                    time_left.setText("Calculating...");
                } else {
                    secondMeasurementTime = System.currentTimeMillis();
                    secondMeasurementVoltage = status;
                    double consumption = (double) (firstMeasurementVoltage - secondMeasurementVoltage) / (secondMeasurementTime - firstMeasurementTime);
                    long voltageLeft = secondMeasurementVoltage - MIN_VOLTAGE;
                    Log.d("myTag", "Voltage left " + voltageLeft);
                    Log.d("myTag", "time gone " + (secondMeasurementTime - firstMeasurementTime));
                    double time = voltageLeft / consumption;        //in ms
                    time /= 1000;
                    time_left.setText("Seconds left: " + time);
                    firstMeasurementTime = secondMeasurementTime;
                    firstMeasurementVoltage = secondMeasurementVoltage;
                }*/
                voltage.setText(String.valueOf(status));
            }
        };

        registerReceiver(receiver, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
        Log.d("myTag", "Application was started");
    }
}