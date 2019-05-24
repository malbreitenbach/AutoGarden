package malbreitenbach.autogarden.Communication;

import android.os.AsyncTask;
import android.util.Log;

import java.util.Date;

import malbreitenbach.autogarden.Application.Tab1_main;

import static malbreitenbach.autogarden.Application.MainActivity.bt;

/**
 * Created by Malte on 03.05.2019.
 */

public class BT_SendReceive extends AsyncTask<Void, Integer, MessageRec>
{

    @Override
    protected void onPreExecute()
    {
        //Log.v("TASK", "New Task started");
    }

    @Override
    protected MessageRec doInBackground(Void[] devices) //while the progress dialog is shown, the connection is done in background
    {
        publishProgress(1);
        //Log.v("TASK", "Task starts Background Task");

        //If new Message in Buffer
        MessageTra messageToBeTransmitted = bt.nextMessage();

        if(messageToBeTransmitted != null)
        {
            bt.sendMessage(messageToBeTransmitted);
        }else
        {
            return null;
        }

        //Message transmitted
        publishProgress(2);
        //Start Receiving
        long startTime = new Date().getTime();
        MessageRec receivedMessage;

        while((receivedMessage = bt.getMessage()) == null)
        {
            if(this.isCancelled())
            {
                Log.v("BT", "Task was cancelled externally");
                try {Thread.sleep(500);} catch (InterruptedException e) {e.printStackTrace();}
                return null;
            }
            if((new Date()).getTime() > startTime + 250)
            {
                //TimeOut
                Log.v("BT", "Timeout for Receiving");
                try {Thread.sleep(500);} catch (InterruptedException e) {e.printStackTrace();}
                return null;
            }
            try {Thread.sleep(1);} catch (InterruptedException e) {e.printStackTrace();}
        }
        Log.v("BT", "## Message received  \r\n" +
                "## Type: "+ receivedMessage.getID().toString() + " \r\n" +
                "## Response time = " + ((new Date()).getTime() - startTime) + "ms");
        publishProgress(3);
        CommandRecType requiredRecType = messageToBeTransmitted.getReplyID();
        CommandRecType recType = receivedMessage.getID();
        if(!recType.equals(requiredRecType)) {
            Log.v("BT", "MessageID not as expected");
            try {Thread.sleep(500);} catch (InterruptedException e) {e.printStackTrace();}
            return null; //MessageID is not as expected
        }

        //Log.v("TASK", "Task ends Background Task");
        return receivedMessage;
    }

    @Override
    protected void onProgressUpdate(Integer... values) {
        super.onProgressUpdate(values);
        bt.tab2.setStatus(values[0]);
    }

    @Override
    protected void onPostExecute(MessageRec result) //after the doInBackground, it checks if everything went fine
    {
        if(result != null) {
            //Log.v("BT", "Received Message: " + result);
            result.execute();
            bt.tab2.setStatus(0);

            //Start new Transfer
            if(bt.nextMessageAvailable()) {
                Log.v("BT", "Next Message Available, will be transmitted immediately...");
                bt.bt_sendReceive = new BT_SendReceive();
                bt.bt_sendReceive.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
                return;
            }
        }else
        {
            Log.v("BT", "Message Receive failed!");
            bt.tab2.setStatus(4);
            bt.tab.updateBluetoothStatus("Disconnected",0);
            //Start new Transfer
            if(bt.nextMessageAvailable()) {
                Log.v("BT", "Next Message Available, will be transmitted immediately...");
                bt.bt_sendReceive = new BT_SendReceive();
                bt.bt_sendReceive.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
                return;
            }
        }
        //Log.v("TASK", "Task ended.");
    }
}