package unibo.btclient;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.util.UUID;

import unibo.btclient.utils.C;
import unibo.btlib.BluetoothChannel;
import unibo.btlib.BluetoothUtils;
import unibo.btlib.ConnectToBluetoothServerTask;
import unibo.btlib.ConnectionTask;
import unibo.btlib.RealBluetoothChannel;
import unibo.btlib.exceptions.BluetoothDeviceNotFound;


public class MainActivity extends AppCompatActivity {
    private final int D2 =  40;  //40
    private final int DeltaD = 4;
    private BluetoothChannel btChannel;
    private int perc = 0;
    SeekBar seekBar;

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        seekBar = findViewById(R.id.seek_bar);
        seekBar.setKeyProgressIncrement(20);

        final BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();

        if(btAdapter != null && !btAdapter.isEnabled()){
            startActivityForResult(new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE), C.bluetooth.ENABLE_BT_REQUEST);
        }

        initUI();
    }

    private void initUI() {
        findViewById(R.id.connectBtn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    connectToBTServer();
                } catch (BluetoothDeviceNotFound bluetoothDeviceNotFound) {
                    bluetoothDeviceNotFound.printStackTrace();
                }
            }
        });

        findViewById(R.id.btn_controlla_diga).setOnClickListener(l ->{
            seekBar.setVisibility(View.VISIBLE);
            seekBar.setProgress(perc);
            btChannel.sendMessage("START");
        });

        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                btChannel.sendMessage(String.valueOf("MANUAL;"+seekBar.getProgress()));
                Toast.makeText(getApplicationContext(), String.valueOf("MANUAL; value: "+seekBar.getProgress()), Toast.LENGTH_LONG).show();

            }
        });


    }

    @Override
    protected void onStop() {
        super.onStop();

        btChannel.close();
    }

    @Override
    protected void onActivityResult(final int requestCode, final int resultCode, @Nullable final Intent data) {
        if(requestCode == C.bluetooth.ENABLE_BT_REQUEST && resultCode == RESULT_OK){
            Log.d(C.APP_LOG_TAG, "Bluetooth enabled!");
        }

        if(requestCode == C.bluetooth.ENABLE_BT_REQUEST && resultCode == RESULT_CANCELED){
            Log.d(C.APP_LOG_TAG, "Bluetooth not enabled!");
        }
    }

    private void connectToBTServer() throws BluetoothDeviceNotFound {
        final BluetoothDevice serverDevice = BluetoothUtils.getPairedDeviceByName(C.bluetooth.BT_DEVICE_ACTING_AS_SERVER_NAME);

        // !!! UTILIZZARE IL CORRETTO VALORE DI UUID
        final UUID uuid = BluetoothUtils.getEmbeddedDeviceDefaultUuid();
        //final UUID uuid = BluetoothUtils.generateUuidFromString(C.bluetooth.BT_SERVER_UUID);

        new ConnectToBluetoothServerTask(serverDevice, uuid, new ConnectionTask.EventListener() {
            @Override
            public void onConnectionActive(final BluetoothChannel channel) {

                ((Button)findViewById(R.id.connectBtn)).setText("Connected.");


                findViewById(R.id.connectBtn).setEnabled(false);

                btChannel = channel;
                btChannel.registerListener(new RealBluetoothChannel.Listener() {
                    @Override
                    public void onMessageReceived(String receivedMessage) {
                        /*((TextView) findViewById(R.id.chatLabel)).append(String.format("> [RECEIVED from %s] %s\n",
                                btChannel.getRemoteDeviceName(),
                                receivedMessage));*/

                        if(receivedMessage.contains("NORMALE")){
                            ((TextView)findViewById(R.id.statusLabel)).setText("NORMALE");
                            ((TextView)findViewById(R.id.statusLabel)).setTextColor(Color.parseColor("#FF000000"));
                            ((TextView)findViewById(R.id.livDiga)).setText("0 %");
                            findViewById(R.id.btn_controlla_diga).setEnabled(false);
                            findViewById(R.id.seek_bar).setVisibility(View.INVISIBLE);
                            btChannel.sendMessage("STOP");


                        }
                        else if(receivedMessage.contains("PREALLARME")){
                            String value = receivedMessage.split(";")[1];
                            ((TextView)findViewById(R.id.statusLabel)).setText("PRE ALLARME");
                            ((TextView)findViewById(R.id.lastRelLabel)).setText(value);
                            findViewById(R.id.btn_controlla_diga).setEnabled(false);
                            findViewById(R.id.seek_bar).setVisibility(View.INVISIBLE);
                            btChannel.sendMessage("STOP");


                            ((TextView)findViewById(R.id.statusLabel)).setTextColor(Color.rgb(255,165,0));
                        }
                        else if(receivedMessage.contains("ALLARME")){
                            String value = receivedMessage.split(";")[1];
                            ((TextView)findViewById(R.id.statusLabel)).setText("ALLARME");
                            ((TextView)findViewById(R.id.lastRelLabel)).setText(value);
                            perc = getPercentage(Float.parseFloat(value));
                            ((TextView)findViewById(R.id.livDiga)).setText(perc + "%");
                            findViewById(R.id.btn_controlla_diga).setEnabled(true);
                            ((TextView)findViewById(R.id.statusLabel)).setTextColor(Color.rgb(255,0,0));
                        }
                    }

                    @Override
                    public void onMessageSent(String sentMessage) {
                        /*((TextView) findViewById(R.id.chatLabel)).append(String.format("> [SENT to %s] %s\n",
                                btChannel.getRemoteDeviceName(),
                                sentMessage));*/
                    }
                });
            }

            @Override
            public void onConnectionCanceled() {
                ((TextView) findViewById(R.id.statusLabel)).setText(String.format("Status : unable to connect, device %s not found!",
                        C.bluetooth.BT_DEVICE_ACTING_AS_SERVER_NAME));
            }
        }).execute();
    }

    private int getPercentage(float D) {

        if (D > D2 - DeltaD && D <= D2)
            return 20;
        if (D > D2 - (2 * DeltaD) && D <= D2 - DeltaD)
            return 40;
        if (D > D2 - (3 * DeltaD) && D <= D2 - (2 * DeltaD))
            return 60;
        if (D > D2 - (4 * DeltaD) && D <= D2 - (3 * DeltaD))
            return 80;
        if (D <= D2 * (4 * DeltaD))
            return 100;

        return 0;

    }
}
