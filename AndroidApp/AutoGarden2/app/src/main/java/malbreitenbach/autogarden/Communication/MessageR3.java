package malbreitenbach.autogarden.Communication;

import java.util.Calendar;
import java.util.Date;

import static malbreitenbach.autogarden.Application.MainActivity.bt;

/**
 * Created by Malte on 17.05.2019.
 */

public class MessageR3 extends MessageRec {

    /* --- Object Variables --- */
    boolean available;

    public MessageR3(byte[] bytestream)
    {
        super(bytestream);
        available = (bytestream[2] == 0x11);
    }

    public CommandRecType getID()
    {
        return CommandRecType.COMREC_AVAILIBILITY;
    }

    @Override
    public void execute() {
        if(available)
            bt.tab.updateBluetoothStatus("Connected (last checked: " + (Calendar.getInstance()).get(Calendar.HOUR_OF_DAY) + ":" + (Calendar.getInstance()).get(Calendar.MINUTE)%60 + ")",100);
        else
            bt.tab.updateBluetoothStatus("Disconnected",0);
    }
}
