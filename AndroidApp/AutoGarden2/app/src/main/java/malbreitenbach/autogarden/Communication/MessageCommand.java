package malbreitenbach.autogarden.Communication;

/**
 * Created by Malte on 03.05.2019.
 */

public class MessageCommand extends Message {

    private CommandType comType;

    /*
    public MessageCommand(byte[] inputstream)
    {
        super(inputstream);
    }
    */

    public MessageCommand(CommandType type)
    {
        super();
        comType = type;
        inputstream[0] = 0x55 | 0x01; //Command
        switch (type)
        {
            case COMMAND_RESYNC:
                inputstream[1] = 4;
                break;
            case COMMAND_AVAILABILITY_REQUEST:
                inputstream[1] = 5;
                break;
            case COMMAND_REQUEST_DATA_INFO:
                inputstream[1] = 7;
                break;
        }

        inputstream[2] = 0x42;
        inputstream[3] = 0x42;
        inputstream[4] = 0x42;
        inputstream[5] = 0x42;
        inputstream[6] = 0x42;
        generateChecksum();
    }

}
