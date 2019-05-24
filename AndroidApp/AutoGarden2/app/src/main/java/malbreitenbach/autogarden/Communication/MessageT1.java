package malbreitenbach.autogarden.Communication;

import static malbreitenbach.autogarden.Communication.CommandType.COMMAND_READ_SENSORY;

/**
 * Created by Malte on 16.05.2019.
 */

public class MessageT1 extends MessageTra {

    @Override
    protected CommandRecType getReplyID() {
        return CommandRecType.COMREC_DATA;
    }

    @Override
    protected CommandType getID() {
        return COMMAND_READ_SENSORY;
    }

    public MessageT1(byte t1_light, byte t1_bat, byte t1_temp, byte t1_efeu, byte t1_lfeu)
    {
        super(STARTBYTE_DATA, getCommandID(COMMAND_READ_SENSORY));
        inputstream[2] = t1_light;
        inputstream[3] = t1_bat;
        inputstream[4] = t1_temp;
        inputstream[5] = t1_efeu;
        inputstream[6] = t1_lfeu;
        generateChecksum();
    }


}
