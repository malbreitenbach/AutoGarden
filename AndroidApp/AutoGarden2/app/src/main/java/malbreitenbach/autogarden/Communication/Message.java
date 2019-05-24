package malbreitenbach.autogarden.Communication;

import android.util.Log;

import java.util.Arrays;

/**
 * Created by Malte on 03.05.2019.
 */

abstract public class Message {

    final static byte numBytesRec = 11;
    final static byte numBytesTra = 8;

    final static byte STARTBYTE_DATA = 0x54;
    final static byte STARTBYTE_COMMAND = 0x55;

    protected byte[] inputstream;

    //public Message(byte[] inputstream)
    {
        this.inputstream = inputstream;
    }

    //public Message()
    {
        this.inputstream = new byte[numBytesTra];
    }

    /*
    public static Message getMessage(byte[] inputstream) throws Exception
    {
        //Check StartByte
        byte startbyte = inputstream[0];

        if((startbyte & 0xFE) == (0x55 & 0xFE))
        {
            //Startbyte Correct, generate Object
            Message msg;
            if((startbyte & 0x01) == 0) //is Data
            {
                msg = new MessageData(inputstream);
            }else //is Command
            {
                msg = new MessageCommand(inputstream);
            }

            //Check Checksum
            if(msg.checkChecksum()) {
                return msg;
            } else
            {
                throw new MessageCorruptedException();
            }
        }else
        {
            throw new MessageCorruptedException();
        }
    }
    */

    /*
    public static Message getRecMessage(byte[] inputstream) throws Exception
    {
        //Check StartByte
        byte startbyte = inputstream[0];

        if((startbyte & 0xFE) == (0x55 & 0xFE))
        {
            //Startbyte Correct, generate Object
            Message msg;
            if((startbyte & 0x01) == 0) //is Data
            {

            }else //is Command
            {

            }

            //Check Checksum
            if(msg.checkChecksum()) {
                return msg;
            } else
            {
                throw new MessageCorruptedException();
            }
        }else
        {
            throw new MessageCorruptedException();
        }
    }
    */

    public boolean checkChecksum()
    {
        byte sum = 0;
        for(int i = 0; i < inputstream.length - 1; i++)
        {
            sum += inputstream[i];
        }

        return (sum == inputstream[inputstream.length-1]);
    }

    public void generateChecksum()
    {
        byte sum = 0;
        for(int i = 0; i < inputstream.length - 1; i++)
        {
            sum += inputstream[i];
        }

        inputstream[inputstream.length-1] = sum;
    }

    public byte[] getBytestream()
    {
        return inputstream;
    }

    public void printBytestream()
    {
        Log.v("DEBUG", "PRINT BYTESTREAM: ");
        Log.v("DEBUG", Arrays.toString(inputstream));
        Log.v("DEBUG", "Checksum: " + checkChecksum());
    }

    public String toString()
    {
        return Arrays.toString(inputstream) + ", Checksum: " + checkChecksum();
    }



}
