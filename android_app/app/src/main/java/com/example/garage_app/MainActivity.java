package com.example.garage_app;

import android.graphics.PorterDuff;
import android.os.Bundle;
import android.widget.CompoundButton;
import android.widget.ProgressBar;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import android.view.View;
import android.content.Context;
import android.util.Log;
import android.widget.Button;
import android.widget.Toast;

import androidx.core.content.ContextCompat;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import android.app.NotificationManager;

import java.nio.charset.StandardCharsets;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";//yflj
    static String MQTTHOST = "tcp://wqtt.ru:5031";//tcp://wqtt.ru:5031
    static String USERNAME = "student";
    static String PASSWORD = "student";

    private NotificationManager notificationManager;
    private static final int NOTIFY_ID = 1;
    private static final String CHANNEL_ID ="CNANNEL_ID ";
    Button BT1;

    MqttAndroidClient client;
    private Context context;

    private TextView connectionStatusText;
    private TextView humidityText;

    private int progress = 20;
    private boolean mode = false;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        context = this;


        connect ();
        //setupPowerButton();
        setupButton1();
        setupButton2();
        setupButton3();
        levelProgress();
        seekbar();
        switchcontrol();

    }
    private void switchcontrol(){

        Switch switcher = findViewById(R.id.switch2);
        if (switcher != null) {
             mode = true;

        }
            int color = 0; if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {
                color = getResources().getColor(R.color.white, getTheme());
            }
            switcher.getThumbDrawable().setColorFilter(color, PorterDuff.Mode.SRC_ATOP); // полоска
            switcher.getTrackDrawable().setColorFilter(color, PorterDuff.Mode.SRC_ATOP); // кругляшок

        switcher.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean checked = ((Switch) v).isChecked();
                if (checked){
                   setEn1(false);
                }
                else{
                    setEn1(true);
                }
            }
        });
    }

    private void setEn1(boolean set) {
        Button button1 = findViewById(R.id.Button1);
        Button button2 = findViewById(R.id.Button2);
        Button button3 = findViewById(R.id.Button3);
        SeekBar seekBar = findViewById(R.id.seekBar);
        TextView text = findViewById(R.id.textcontroltemp);
        if (set) {
            button1.setVisibility(View.VISIBLE);
            button2.setVisibility(View.VISIBLE);
            button3.setVisibility(View.VISIBLE);
            text.setVisibility(View.INVISIBLE);
            seekBar.setVisibility(View.INVISIBLE);
        }
        else{
            button1.setVisibility(View.INVISIBLE);
            button2.setVisibility(View.INVISIBLE);
            button3.setVisibility(View.INVISIBLE);
            text.setVisibility(View.VISIBLE);
            seekBar.setVisibility(View.VISIBLE);
        }
        button1.setEnabled(set);
        button2.setEnabled(set);
        button3.setEnabled(set);
    }
    private void seekbar(){
        String topic = "leds";
        SeekBar seekBar = findViewById(R.id.seekBar);
        TextView text = findViewById(R.id.textcontroltemp);

        int color = 0; if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {
            color = getResources().getColor(R.color.white, getTheme());
        }
        seekBar.getProgressDrawable().setColorFilter(color, PorterDuff.Mode.SRC_ATOP); // полоска
        seekBar.getThumb().setColorFilter(color, PorterDuff.Mode.SRC_ATOP); // кругляшок


        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener(){

            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                text.setText(String.valueOf(progress));
                if (progress == 38) text.setText("Off");
                String sepshProgress = String.valueOf(progress);
                try {
                    client.publish(topic, sepshProgress.getBytes(), 2, true);
                }
                catch (Exception e){
                    e.printStackTrace();
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

    }



    private void levelProgress(){
        ProgressBar progressBar = findViewById(R.id.progressBar);
        progressBar.setProgress(20);


    }
    private void setupButton1(){
        String topic = "leds";
        String ON = "0";
        Button Button1 = findViewById(R.id.Button1);

        Button1.setOnClickListener(v -> {


            Button1.setEnabled(Button1.isEnabled());
            try {
                client.publish(topic, ON.getBytes(), 2, true);


            } catch (Exception e) {
                e.printStackTrace();

            }
        });
    }
    private void setupButton2(){
        String topic = "leds";
        String ON = "45";
        Button Button2 = findViewById(R.id.Button2);



        Button2.setOnClickListener(v -> {


            Button2.setEnabled(Button2.isEnabled());
            // powerImage.setImageResource(R.drawable.green_cicrle_bg);
            try {
                client.publish(topic, ON.getBytes(), 2, true);

                //Toast.makeText(getBaseContext(), "gj;hg", Toast.LENGTH_LONG).show();

            } catch (Exception e) {
                e.printStackTrace();

            }
        });
    }
//    public void sendMessage(View view) {
//        Button Button3 = findViewById(R.id.Button3);
//        Button3.getBackground().setAlpha(64);
//    }
    private void setupButton3(){
        String topic = "leds";
        String ON = "50";
        Button Button3 = findViewById(R.id.Button3);
//        TextView textView = findViewById(R.id.textView);
//        textView.setText("fgr");
        //Button3.getBackground().setAlpha(64);
        Button3.setOnClickListener(v -> {
           // Button3.getBackground().setAlpha(64);

            Button3.setEnabled(Button3.isEnabled());
            // powerImage.setImageResource(R.drawable.green_cicrle_bg);
            try {
                client.publish(topic, ON.getBytes(), 2, true);

                //Toast.makeText(getBaseContext(), "gj;hg", Toast.LENGTH_LONG).show();

            } catch (Exception e) {
                e.printStackTrace();

            }
        });
    }




    public void connect() {

        String clientId = MqttClient.generateClientId();
        client = new MqttAndroidClient(this.getApplicationContext(), MQTTHOST, clientId);
        MqttConnectOptions options = new MqttConnectOptions();
        options.setUserName(USERNAME);
        options.setPassword(PASSWORD.toCharArray());
        try {
            IMqttToken token = client.connect(options);
            token.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    // We are connecte
                    Toast.makeText(getBaseContext(), "Чайнк подключён", Toast.LENGTH_LONG).show();
                    sub();
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    // Something went wrong e.g. connection timeout or firewall problems
                    Toast.makeText(getBaseContext(), "Чайнк не подключён", Toast.LENGTH_LONG).show();
                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }

    }

    private void animation(){
        ProgressBar progressBar = findViewById(R.id.progressBar);
        long animationTHelp = 0;
        progressBar.setProgress(20);
        if (System.currentTimeMillis() - animationTHelp >= 1000){
            progressBar.setProgress(0);
            animationTHelp = System.currentTimeMillis();
        }

    }
    private void setEn(boolean set) {
        Button button1 = findViewById(R.id.Button1);
        Button button2 = findViewById(R.id.Button2);
        Button button3 = findViewById(R.id.Button3);
        TextView textView5 = findViewById(R.id.textView5);
        if (set) {
            button1.getBackground().setAlpha(255);
            button2.getBackground().setAlpha(255);
            button3.getBackground().setAlpha(255);
            //textView6.isCursorVisible();
            textView5.setVisibility(View.INVISIBLE);
        }
        else{
            button1.getBackground().setAlpha(20);
            button2.getBackground().setAlpha(20);
            button3.getBackground().setAlpha(20);
            textView5.setVisibility(View.VISIBLE);
        }
        button1.setEnabled(set);
        button2.setEnabled(set);
        button3.setEnabled(set);
    }


    private void sub() {

        //------------------------------------------------------------------------------------------------------------------
        notificationManager = (NotificationManager) getApplicationContext().getSystemService(Context.NOTIFICATION_SERVICE);
        //-----------------------------------------------------------------------------------------------------------------
       // FrameLayout powerButton = findViewById(R.id.powerButton);


        ProgressBar progressBar = findViewById(R.id.progressBar);
        TextView textView = findViewById(R.id.textView);
        TextView textView1 = findViewById(R.id.textView3);
        String temp = "temp";
        String led = "Kout";
        final String[] tempShow = new String[1];
        //String cel = "C";

        try {

            client.subscribe(led, 2);
            client.subscribe(temp, 2);

            client.setCallback(new MqttCallback() {
                @Override
                public void connectionLost(Throwable cause) {
                    Log.d(TAG, "connectionLost: " + cause.getMessage());
                    Log.d(TAG, "connectionLost: " + cause.getStackTrace());
                    connect();

                }

                @Override
                public void messageArrived(String topic/*,String topic1*/, MqttMessage message/*, MqttMessage message1*/) throws Exception {

                    ///////////////////////////////////////////led1111111111111111111
                    if (topic.equals(temp))
                    {
                        tempShow[0] = new String(message.getPayload());

                        textView.setText(tempShow[0]);
                    }
                    if (topic.equals(led)) {
//                        tempShow[0] = new String(message.getPayload());
//                        textView.setText(tempShow[0] + "t");

                        if (new String(message.getPayload()).equals("0")) {
                            //  switch1.setChecked(true);
                            progressBar.setProgress(20);
                            //animation();
                            textView1.setText("0");
                            setEn(false);
                            // powerButton.setEnabled(true);
                            //Toast.makeText(getBaseContext(), "пополните запас воды пожалуйста", Toast.LENGTH_LONG).show();


                        }
                        if (new String(message.getPayload()).equals("1")) {
                            //  switch1.setChecked(true);
                            progressBar.setProgress(40);
                           // powerButton.setEnabled(true);
                            textView1.setText("1");
                            setEn(true);

                        }
                        if (new String(message.getPayload()).equals("2")) {
                            //  switch1.setChecked(true);
                            progressBar.setProgress(60);
                            // powerButton.setEnabled(true);
                            textView1.setText("2");
                            setEn(true);

                        }
                        if (new String(message.getPayload()).equals("3")) {
                            //  switch1.setChecked(true);
                            progressBar.setProgress(80);
                            // powerButton.setEnabled(true);
                            textView1.setText("3");
                            setEn(true);

                        }
                        if (new String(message.getPayload()).equals("4")) {
                            //  switch1.setChecked(true);
                            progressBar.setProgress(100);
                            textView1.setText("4");
                            // powerButton.setEnabled(true);
                            setEn(true);

                        }

                    }
                    ////////////////////////////////////////////door///////////////////////



                }

                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {

                }
            });
        } catch (MqttException e) {
        }
    }

}
