package malbreitenbach.autogarden.Application;

import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import malbreitenbach.autogarden.Communication.BT_Background;
import malbreitenbach.autogarden.Communication.BT_SendReceive;
import malbreitenbach.autogarden.Communication.Bluetooth;
import malbreitenbach.autogarden.Communication.Message;
import malbreitenbach.autogarden.Communication.MessageT0;
import malbreitenbach.autogarden.R;

import static malbreitenbach.autogarden.Application.MainActivity.bt;

/**
 * Created by Malte on 02.05.2019.
 */

public class Tab1_main extends Fragment {

    /** --- Widgets --- **/
    ProgressBar prog_bt;
    Button but_connect, but_send, but_up;
    TextView txt_status;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View rootView = inflater.inflate(R.layout.tab1_main, container, false);

        //Update Context on bt adapter
        bt.updateContext(getContext());
        bt.updateTab(this);

        // --- Widget Initialization ---
        prog_bt = (ProgressBar)  rootView.findViewById(R.id.prog_bt);
        but_connect = (Button)  rootView.findViewById(R.id.but_connect);
        but_send = (Button)  rootView.findViewById(R.id.but_send);
        but_up = (Button)  rootView.findViewById(R.id.but_up);
        txt_status = (TextView)  rootView.findViewById(R.id.txt_status);

        //Connect ProgressBar with BT
        bt.setProgressBar(prog_bt);

        //Update Textview and Progress
        updateBluetoothStatus("Press Button to connect to Autogarden", 0);

        //Start background process;
        //(new BT_Background(this)).execute();

        // --- Button Listeners ---
        but_connect.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if(!bt.isBtConnected()) {
                            bt.connectDevice();
                        }else
                        {
                            Toast.makeText(getActivity(), "Connection with AutoGarden already established!", Toast.LENGTH_LONG).show();
                        }
                    }
                }
        );

        but_up.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if(bt.isBtConnected())
                        {
                            prog_bt.setProgress(100);
                        }else
                        {
                            prog_bt.setProgress(0);
                        }
                    }
                }
        );

        but_send.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        bt.pushMessage(new MessageT0((byte)0,(byte)1,(byte)0,(byte)0));
                    }
                }
        );

        return rootView;
    }

    public void updateBluetoothStatus(String text, int progress)
    {
        prog_bt.setProgress(progress);
        txt_status.setText(text);
    }

}
