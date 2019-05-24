package malbreitenbach.autogarden.Communication;

/**
 * Created by Malte on 07.05.2019.
 */

public class MessageR1 extends MessageRec {

    /* --- Object Variables --- */
    int timeInS;
    byte state;

    public MessageR1(byte[] bytestream)
    {
        super(bytestream);
        timeInS = (int)(bytestream[2])<<3*8 + (int)(bytestream[3])<<2*8 + (int)(bytestream[4])<<1*8 + (int)(bytestream[5]);
        state = bytestream[6];
    }

    public CommandRecType getID()
    {
        return CommandRecType.COMREC_TIMEDIFF;
    }

    @Override
    public void execute() {

    }
}
