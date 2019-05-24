package malbreitenbach.autogarden.Application;

import android.graphics.Color;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ProgressBar;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.ToggleButton;

import malbreitenbach.autogarden.Communication.MessageT0;
import malbreitenbach.autogarden.R;
import com.jjoe64.graphview.*;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import java.util.Iterator;

import static malbreitenbach.autogarden.Application.MainActivity.bt;

/**
 * Created by Malte on 02.05.2019.
 */

public class Tab2_manual extends Fragment {

    /* --- UI elements --- */
    ProgressBar bt_circ;
    TextView bt_circ_text;
    ToggleButton but1;
    SeekBar seek1;
    TextView txt_pump;
    GraphView graph_temp, graph_light, graph_hum;

    /* --- Variables --- */
    LineGraphSeries<DataPoint> series_temp, series_light, series_hum;
    double currentTime;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View rootView = inflater.inflate(R.layout.tab2_manual, container, false);
        //TextView textView = (TextView) rootView.findViewById(R.id.section_label);
        //textView.setText(getString(R.string.section_format, getArguments().getInt(ARG_SECTION_NUMBER)));

        graph_temp = (GraphView) rootView.findViewById(R.id.graph_temp);
        graph_light = (GraphView) rootView.findViewById(R.id.graph_light);
        graph_hum = (GraphView) rootView.findViewById(R.id.graph_hum);
        bt_circ = (ProgressBar) rootView.findViewById(R.id.bt_circ);
        bt_circ_text = (TextView) rootView.findViewById(R.id.txt_show);
        but1 = (ToggleButton) rootView.findViewById(R.id.but1);
        seek1 = (SeekBar) rootView.findViewById(R.id.seek1);
        txt_pump = (TextView) rootView.findViewById(R.id.txt_field);

        but1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(but1.isChecked())
                {
                    //ON
                    bt.activateSensorReadings(true);
                }else
                {
                    //OFF
                    bt.activateSensorReadings(false);
                }
            }
        });

        seek1.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                bt.pushMessage(new MessageT0((byte)1,(byte)(progress),(byte)0,(byte)0));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                txt_pump.setText("Manual pump control (active)");
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                seekBar.setProgress(0);
                bt.pushMessage(new MessageT0((byte)0,(byte)0,(byte)0,(byte)0));
                txt_pump.setText("Manual pump control (inactive)");
            }
        });

        bt.updateTab2(this);
        setStatus(0);

        series_temp = new LineGraphSeries<DataPoint>();
        series_temp.setThickness(4);
        series_temp.setColor(Color.GREEN);
        series_light = new LineGraphSeries<DataPoint>();
        series_light.setThickness(4);
        series_light.setColor(Color.RED);
        series_hum = new LineGraphSeries<DataPoint>();
        series_hum.setThickness(4);
        series_hum.setColor(Color.GRAY);


        //graph_temp.getViewport().setYAxisBoundsManual(true);
        graph_temp.getViewport().setXAxisBoundsManual(true);
        graph_temp.addSeries(series_temp);
        graph_temp.getGridLabelRenderer().setVerticalAxisTitle("Temperature [°C]");
        graph_temp.getGridLabelRenderer().setHorizontalAxisTitle("Time [s]");
        graph_temp.getGridLabelRenderer().setTextSize(16);
        graph_temp.getGridLabelRenderer().setHighlightZeroLines(true);
        graph_temp.getViewport().setScrollable(true);


        graph_light.getViewport().setXAxisBoundsManual(true);
        graph_light.addSeries(series_light);
        graph_light.getGridLabelRenderer().setVerticalAxisTitle("Light [%]");
        graph_light.getGridLabelRenderer().setHorizontalAxisTitle("Time [s]");
        graph_light.getGridLabelRenderer().setTextSize(16);
        graph_light.getGridLabelRenderer().setHighlightZeroLines(true);
        graph_light.getViewport().setScrollable(true);


        graph_hum.getViewport().setXAxisBoundsManual(true);
        graph_hum.addSeries(series_hum);
        graph_hum.getGridLabelRenderer().setVerticalAxisTitle("Humidity [%]");
        graph_hum.getGridLabelRenderer().setHorizontalAxisTitle("Time [s]");
        graph_hum.getGridLabelRenderer().setTextSize(16);
        graph_hum.getGridLabelRenderer().setHighlightZeroLines(true);
        graph_hum.getViewport().setScrollable(true);


        currentTime = 0;

        return rootView;


    }

    public void addToLiveGraph(byte temp, byte light, byte hum)
    {
        series_temp.appendData(new DataPoint(currentTime, temp), true, 1000, false);
        series_light.appendData(new DataPoint(currentTime, light), true, 1000, false);
        series_hum.appendData(new DataPoint(currentTime, hum), true, 1000, false);

        graph_temp.getViewport().setMinX(currentTime - 30);
        graph_light.getViewport().setMinX(currentTime - 30);
        graph_hum.getViewport().setMinX(currentTime - 30);
        graph_temp.getViewport().setMinY(0.0);
        graph_light.getViewport().setMinY(0.0);
        graph_hum.getViewport().setMinY(0.0);

        currentTime += 0.3;
    }

    public void approxToLiveGraph()
    {
        /*
        double next;
        if(series_temp.getValues(currentTime-0.4,currentTime) != null) {
            Iterator<DataPoint> it = series_temp.getValues(currentTime - 0.4, currentTime);
            if (it.hasNext()) {
                double oldValue = it.next().getY();
                double currentValue = 0;
                if (it.hasNext()) {
                    currentValue = it.next().getY();
                    next = 2*currentValue - oldValue;
                }else
                {
                    return;
                }
            }else
            {
                return;
            }
        }else
        {
            return;
        }

        series_temp.appendData(new DataPoint(currentTime, next), true, 1000, false);
        //series_light.appendData(new DataPoint(currentTime, series_light.getValues(currentTime,currentTime).next().getY()), true, 1000, false);
        //series_hum.appendData(new DataPoint(currentTime, series_hum.getValues(currentTime,currentTime).next().getY()), true, 1000, false);

        graph_temp.getViewport().setMinX(currentTime - 30);
        graph_temp.getViewport().setMinY(0.0);
        currentTime += 0.1;
        */
    }

    /**
     * Update Status
     * 0: Up to date
     * 1: Transmit
     * 2: Wait
     * 3: Receive
     */
    public void setStatus(int status)
    {
        switch (status)
        {
            case 0:
                bt_circ_text.setText("Up to date");
                bt_circ.setAlpha(0f);
                break;
            case 1:
                bt_circ_text.setText("Sending Request...");
                bt_circ.setAlpha(0.3f);
                break;
            case 2:
                bt_circ_text.setText("Waiting...");
                bt_circ.setAlpha(0.6f);
                break;
            case 3:
                bt_circ_text.setText("Receiving...");
                bt_circ.setAlpha(0.9f);
                break;
            case 4:
                bt_circ_text.setText("Communication failed!");
                bt_circ.setAlpha(0f);
                break;
        }
    }
}