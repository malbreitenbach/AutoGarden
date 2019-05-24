package malbreitenbach.autogarden.Communication;

import static malbreitenbach.autogarden.Communication.CommandRecType.*;

/**
 * Created by Malte on 06.05.2019.
 */

public abstract class MessageRec extends Message {

    public MessageRec(byte[] bytestream)
    {
        this.inputstream = bytestream;
    }

    protected CommandRecType recType;

    protected byte getCommandID(CommandRecType type)
    {
        switch (type)
        {
            case COMREC_MAN_REPLY:
                return 0;
            case COMREC_TIMEDIFF:
                return 1;
            case COMREC_RESYNC:
                return 2;
            case COMREC_AVAILIBILITY:
                return 3;
            case COMREC_REQUEST_DATA_INFO:
                return 5;
            default:
                return 127;
        }
    }

    protected CommandRecType getCommandType(byte type)
    {
        switch (type)
        {
            case 0:
                return COMREC_MAN_REPLY;
            case 1:
                return COMREC_TIMEDIFF;
            case 2:
                return COMREC_RESYNC;
            case 3:
                return COMREC_AVAILIBILITY;
            case 5:
                return COMREC_REQUEST_DATA_INFO;
            default:
                return null;
        }
    }

    /**
     * Returnes Object suited to given bytestream
     * @param bytestream input BT bytestream
     * @return Object suited to given bytestream
     */
    public static MessageRec getRecMessage(byte[] bytestream)
    {
        if(bytestream[0] == STARTBYTE_DATA)
        {
            //Received Message is Data
            return (new MessageR4(bytestream));

        }else if(bytestream[0] == STARTBYTE_COMMAND)
        {
            //Received Message is Command
            switch (bytestream[1])
            {
                case 0:
                    return (new MessageR0(bytestream));
                case 1:
                   return (new MessageR1(bytestream));
                case 2:
                    return (new MessageR1(bytestream));
                case 3:
                    return (new MessageR3(bytestream));
                case 5:
                    return (new MessageR1(bytestream));
            }
        }
        return null;
    }

    public abstract CommandRecType getID();
    public abstract void execute();

}
