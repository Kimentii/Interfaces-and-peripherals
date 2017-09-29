package com.example.kimentii.workwithbatterylab3;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    Button createService;
    Button destroyService;

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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        createService = (Button) findViewById(R.id.create_button);
        destroyService = (Button) findViewById(R.id.destroy_bytton);
        Listener listener = new Listener();
        createService.setOnClickListener(listener);
        destroyService.setOnClickListener(listener);
        Log.d("myTag", "Application was started");
    }
}
