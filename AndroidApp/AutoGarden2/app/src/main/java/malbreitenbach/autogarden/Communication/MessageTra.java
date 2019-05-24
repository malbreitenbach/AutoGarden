package malbreitenbach.autogarden.Communication;

/**
 * Created by Malte on 07.05.2019.
 */

public abstract class MessageTra extends Message {

    protected MessageTra(byte startbyte, byte command)
    {
        inputstream = new byte[numBytesTra];
        inputstream[0] = startbyte;
        inputstream[1] = command;
    }
    protected abstract CommandRecType getReplyID();
    protected abstract CommandType getID();

    protected static byte getCommandID(CommandType type)
    {
        switch (type)
        {
            case COMMAND_MANUAL_CONTROL:
                return 0;
            case COMMAND_READ_SENSORY:
                return 1;
            case COMMAND_UPDATE_TIME:
                return 2;
            case COMMAND_DATA_REQUEST:
                return 3;
            case COMMAND_RESYNC:
                return 4;
            case COMMAND_AVAILABILITY_REQUEST:
                return 5;
            case COMMAND_CHECK_DATA_AVAILABILITY:
                return 6;
            case COMMAND_REQUEST_DATA_INFO:
                return 7;
            default:
                return 127;
        }
    }

}
