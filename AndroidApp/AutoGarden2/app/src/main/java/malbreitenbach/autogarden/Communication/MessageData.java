package malbreitenbach.autogarden.Communication;

/**
 * Created by Malte on 03.05.2019.
 */

public class MessageData extends Message {

    private CommandType comType;
    /*
    private byte t0_pumpOnOf, t0_pumpPWM, t0_PumpAuto,	t0_standBy;
    private byte t1_light, t1_bat, t1_temp, t1_efeu, t1_lfeu;
    private byte tx_timeInS_3, tx_timeInS_2, tx_timeInS_1, tx_timeInS_0; */

    public MessageData(byte[] inputstream)
    {
        super();
    }

    /**
     *
     * @param type  Type of Data (t0 / t1 / tx)
     * @param b2    t0_pumpOnOf / t1_light / tx_timeInS_3
     * @param b3    t0_pumpPWM / t1_bat / tx_timeInS_2
     * @param b4    t0_PumpAuto / t1_temp / tx_timeInS_1
     * @param b5    t0_standBy / t1_efeu / tx_timeInS_0
     * @param b6    - / t1_lfeu / -
     */
    public MessageData(CommandType type, byte b2, byte b3, byte b4, byte b5, byte b6)
    {
        super();
        comType = type;
        inputstream[0] = 0x55 | 0x00; //Data
        inputstream[1] = getCommandID(type);
        inputstream[2] = b2;
        inputstream[3] = b3;
        inputstream[4] = b4;
        inputstream[5] = b5;
        inputstream[6] = b6;
        generateChecksum();
    }

    private byte getCommandID(CommandType type)
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
            case COMMAND_CHECK_DATA_AVAILABILITY:
                return 6;
            default:
                return 127;
        }
    }
}
