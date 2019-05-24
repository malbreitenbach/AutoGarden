package malbreitenbach.autogarden.Communication;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.os.AsyncTask;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.widget.ProgressBar;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayDeque;
import java.util.Date;
import java.util.Deque;
import java.util.Set;
import java.util.UUID;

import malbreitenbach.autogarden.Application.Tab1_main;
import malbreitenbach.autogarden.Application.Tab2_manual;

import static malbreitenbach.autogarden.Application.MainActivity.bt;


/**
 * Created by Malte on 30.04.2019.
 */

public class Bluetooth {

    private final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    BluetoothSocket bluetoothSocket;
    BluetoothAdapter bluetoothAdapter;
    BluetoothDevice HC;
    String btAddress;
    ProgressDialog progress;
    boolean isBtConnected = false;
    ProgressBar prog_bt;
    OutputStream outS;
    InputStream inS;
    Date date;
    Deque<MessageTra> messageBufferOut;

    Context context;
    Tab1_main tab;
    Tab2_manual tab2;
    ConnectBT connectTask;
    BT_Background backgroundTask;
    public BT_SendReceive bt_sendReceive;

    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    public Bluetooth(String address, ProgressDialog progress, Context context, ProgressBar prog_bt)
    {
        btAddress = address;
        this.progress = progress;
        this.context = context;
        this.prog_bt = prog_bt;
        date = new Date();
        bt_sendReceive = new BT_SendReceive();
        messageBufferOut = new ArrayDeque<MessageTra>();
    }

    public void activateSensorReadings(boolean b)
    {
        if(backgroundTask != null)
        {
            backgroundTask.activateSensorReadings(b);
        }else if(!isBtConnected)
        {
            connectDevice();
            Log.v("BACKGROUNDTASK", "First starting Bluetooth");
        }else
        {
            Log.v("BACKGROUNDTASK", "GOT KILLED FROM SOMEWHERE");
        }
    }

    public void setProgressBar(ProgressBar pb)
    {
        prog_bt = pb;
    }

    public int initializeBT() {
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (!bluetoothAdapter.isEnabled()) {
            return 0; // Bluetooth deactivated
        }


        return 2; // Device not found
    }

    public Set<BluetoothDevice> findDevice() {
        //Look for HC06
        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        return pairedDevices;

        /*

        */
    }

    public boolean connectDevice() {
        Set<BluetoothDevice> pairedDevices = findDevice();
        if (pairedDevices.size() > 0) {
            // There are paired devices. Get the name and address of each paired device.
            for (BluetoothDevice device : pairedDevices) {
                if (device.getAddress().equals(btAddress)) {
                    HC = device;
                    connectTask = new ConnectBT();
                    connectTask.execute();
                    return isBtConnected;
                }
            }
        }
        return false;
    }

    public boolean isBtConnected()
    {
        return isBtConnected && bluetoothSocket.isConnected();
    }

    public void updateTab(Tab1_main tab)
    {
        this.tab = tab;
    }

    public void updateTab2(Tab2_manual tab)
    {
        this.tab2 = tab;
    }

    public void updateContext(Context c)
    {
        context = c;
    }

    public boolean pushMessage(MessageTra msg)
    {
        if(!messageBufferOut.isEmpty() && messageBufferOut.getLast().getID() == msg.getID())
        {
            //Delete last same Message
            messageBufferOut.removeLast();
        }
        messageBufferOut.addLast(msg);

        //Restart Transmitting if inactive
        if(bt_sendReceive.getStatus() == AsyncTask.Status.FINISHED || bt_sendReceive.getStatus() == AsyncTask.Status.PENDING || bt_sendReceive == null) {
            bt_sendReceive = new BT_SendReceive();
            bt_sendReceive.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
        }
        return true;
    }

    public MessageTra nextMessage()
    {
        if(!messageBufferOut.isEmpty()) {
            Log.v("BUFFER", "Pop Message (1/" + messageBufferOut.size() + ")");
            return messageBufferOut.removeFirst();
        }else
        {
            return null;
        }
    }

    public boolean nextMessageAvailable()
    {
        return !messageBufferOut.isEmpty();
    }

    public int sendMessage(Message msg)
    {
        if(isBtConnected)
        {
            try {
                outS = bluetoothSocket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
                return 0;
            }
            try {
                outS.write(msg.getBytestream());
            } catch (IOException e) {
                e.printStackTrace();
                return 1;
            }
            return 2;
            //Message successfull transmitted
        }else
        {
            return 7;
        }


    }

    public MessageRec getMessage()
    {
        if(bluetoothSocket == null)
        {
            Log.v("BT", "Socket is NULL");
            if(connectTask == null || (connectTask.getStatus() == AsyncTask.Status.FINISHED || connectTask.getStatus() == AsyncTask.Status.PENDING)) {
                connectTask = new ConnectBT();
                connectTask.execute();
            }
            return null;
        }
        /* --- Open InputStream --- */
        if(inS == null) {
            try {inS = bluetoothSocket.getInputStream();} catch (IOException e) {e.printStackTrace();}
        }

        /* --- Count number of Received Bytes --- */
        int numOfBytesReceived = 0;
        MessageRec receivedMessage;
        byte[] receivedBytestream = new byte[Message.numBytesRec];
        try {numOfBytesReceived = inS.available();} catch (IOException e) {e.printStackTrace();}

        if(numOfBytesReceived > Message.numBytesRec)
        {
           //ERROR : Buffer overrun
            int i = 5;
            try {inS.skip(numOfBytesReceived);}catch (IOException e) {e.printStackTrace();}
        }

        if(numOfBytesReceived == Message.numBytesRec)
        {
            try {inS.read(receivedBytestream);} catch (IOException e) {e.printStackTrace(); return null;}
            try {receivedMessage = MessageRec.getRecMessage(receivedBytestream);} catch (Exception e) {e.printStackTrace();return null;}
            inS = null;
            return receivedMessage;
        }

        return null; // Message is uncomplete
    }

    //Private Klasse  um Verbindung aufzubauen
    private class ConnectBT extends AsyncTask<Void, String, Void>  // UI thread
    {
        private boolean ConnectSuccess = true; //if it's here, it's almost connected

        @Override
        protected void onPreExecute()
        {

            progress = ProgressDialog.show(context, "Connecting...", "Please wait...");  //show a progress dialog
        }

        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);
            if(tab != null && values.length == 2)
            {
                    tab.updateBluetoothStatus(values[0], Integer.parseInt(values[1]));
            }
        }

        @Override
        protected Void doInBackground(Void... devices) //while the progress dialog is shown, the connection is done in background
        {
            try {Thread.sleep(500);
            } catch (InterruptedException e) {e.printStackTrace();}
            try
            {
                if (bluetoothSocket == null || !isBtConnected)
                {
                    publishProgress("Establish connection...", "33");
                    BluetoothDevice dispositivo = bluetoothAdapter.getRemoteDevice(btAddress);//connects to the device's address and checks if it's available
                    bluetoothSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);//create a RFCOMM (SPP) connection
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    bluetoothSocket.connect();//start connection
                    publishProgress("Establish connection...", "66");
                }
            }
            catch (IOException e)
            {
                ConnectSuccess = false;
                e.printStackTrace();
            }
            return null;
        }
        @Override
        protected void onPostExecute(Void result) //after the doInBackground, it checks if everything went fine
        {
            super.onPostExecute(result);

            if (!ConnectSuccess)
            {
                Log.v("BT", "Connection Failed. Is it a SPP Bluetooth? Try again.");
                isBtConnected = false;
            }
            else
            {
                Log.v("BT", "Connected.");
                isBtConnected = true;
                backgroundTask = new BT_Background(tab);
                backgroundTask.execute(); //Start status Display
            }
            progress.dismiss();
        }
    }
}
