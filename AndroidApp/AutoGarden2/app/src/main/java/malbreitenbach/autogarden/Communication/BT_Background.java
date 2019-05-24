package malbreitenbach.autogarden.Communication;

import android.os.AsyncTask;

import malbreitenbach.autogarden.Application.Tab1_main;

import static malbreitenbach.autogarden.Application.MainActivity.bt;

/**
 * Created by Malte on 03.05.2019.
 */

//Private Klasse  um Verbindung abzufragen
public class BT_Background extends AsyncTask<Void, Integer, Void>  // UI thread
{
    Tab1_main f;
    boolean sensorReadingsOn = false;
    public BT_Background(Tab1_main f)
    {
        this.f = f;
    }

    public void activateSensorReadings(boolean b)
    {
        sensorReadingsOn = b;
    }

    @Override
    protected void onPreExecute()
    {

    }

    @Override
    protected void onProgressUpdate(Integer... values) {
        super.onProgressUpdate(values);
        if(f != null && bt != null)
        {
            switch(values[0])
            {
                case 0:
                    bt.pushMessage((new MessageT1((byte) 0x01, (byte) 0x01, (byte) 0x01, (byte) 0x01, (byte) 0x01 )));
                    break;
                case 1:
                    bt.pushMessage(new MessageT5());
                case 2:
                    bt.tab2.approxToLiveGraph();
            }
        }
        else
            this.cancel(true);
    }

    @Override
    protected Void doInBackground(Void... devices) //while the progress dialog is shown, the connection is done in background
    {
        int counter = 0;
        while(true)
        {
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            if(counter++ % 100 == 0)
            {
                //Called each 5s:
                publishProgress(1); //Check Connection
            }

            if(sensorReadingsOn) {
                //Called each 0,2s:
                if (counter++ % 3 == 0)
                {
                    publishProgress(0); //Call for new Data
                }else
                {
                    publishProgress(2); //Generate Data
                }
            }

        }

    }
    @Override
    protected void onPostExecute(Void result) //after the doInBackground, it checks if everything went fine
    {

    }
}
