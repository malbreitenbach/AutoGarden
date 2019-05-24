package malbreitenbach.autogarden.Communication;

import static malbreitenbach.autogarden.Communication.CommandType.COMMAND_AVAILABILITY_REQUEST;

/**
 * Created by Malte on 17.05.2019.
 */

public class MessageT5 extends MessageTra {


    @Override
    protected CommandRecType getReplyID() {
        return CommandRecType.COMREC_AVAILIBILITY;
    }

    @Override
    protected CommandType getID() {
        return COMMAND_AVAILABILITY_REQUEST;
    }

    public MessageT5()
    {
        super(STARTBYTE_DATA, getCommandID(COMMAND_AVAILABILITY_REQUEST));
        inputstream[2] = 0x11;
        inputstream[3] = 0x00;
        inputstream[4] = 0x00;
        inputstream[5] = 0x00;
        inputstream[6] = 0x00;
        inputstream[7] = 0x00;
        generateChecksum();
    }

}
