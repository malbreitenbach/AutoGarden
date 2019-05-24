package malbreitenbach.autogarden.Communication;

import android.util.Log;

/**
 * Created by Malte on 06.05.2019.
 */

public class MessageR0 extends MessageRec {

    /* --- Object Variables --- */
    byte r0_pumpOnOff, r0_pumpPWM, r0_PumpAuto, r0_standBy, r0_state;

    public MessageR0(byte[] bytestream)
    {
        super(bytestream);
        r0_pumpOnOff = bytestream[2];
        r0_pumpPWM = bytestream[3];
        r0_PumpAuto = bytestream[4];
        r0_standBy = bytestream[5];
        r0_state = bytestream[6];
    }

    public CommandRecType getID()
    {
        return CommandRecType.COMREC_MAN_REPLY;
    }

    @Override
    public void execute() {
        if(r0_pumpOnOff == 0)
        {
            Log.v("PUMP", "TURNED OFF");
        }
    }

}
