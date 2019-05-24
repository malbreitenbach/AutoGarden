package malbreitenbach.autogarden.Communication;

import static malbreitenbach.autogarden.Communication.CommandType.COMMAND_MANUAL_CONTROL;


/**
 * Created by Malte on 07.05.2019.
 */

public class MessageT0 extends MessageTra {


    @Override
    protected CommandRecType getReplyID() {
        return CommandRecType.COMREC_MAN_REPLY;
    }

    @Override
    protected CommandType getID() {
        return COMMAND_MANUAL_CONTROL;
    }

    public MessageT0(byte t0_pumpOnOff, byte t0_pumpPWM, byte t0_PumpAuto, byte t0_standBy)
    {
        super(STARTBYTE_DATA, getCommandID(COMMAND_MANUAL_CONTROL));
        inputstream[2] = t0_pumpOnOff;
        inputstream[3] = t0_pumpPWM;
        inputstream[4] = t0_PumpAuto;
        inputstream[5] = t0_standBy;
        inputstream[6] = 0x00;
        generateChecksum();
    }

}
