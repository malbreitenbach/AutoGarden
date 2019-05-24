package malbreitenbach.autogarden.Communication;

import static malbreitenbach.autogarden.Application.MainActivity.bt;


/**
 * Created by Malte on 07.05.2019.
 */

public class MessageR4 extends MessageRec {

        /* --- Object Variables --- */
        int timeInS;
        byte r4_light_bat, r4_temp, r4_efeu, r4_lfeu, r4_info;

        public MessageR4(byte[] bytestream)
        {
            super(bytestream);
            timeInS = (int)(bytestream[1])<<3*8 + (int)(bytestream[2])<<2*8 + (int)(bytestream[3])<<1*8 + (int)(bytestream[4]);
            r4_light_bat = bytestream[5];
            r4_temp = bytestream[6];
            r4_efeu = bytestream[7];
            r4_lfeu = bytestream[8];
            r4_info = bytestream[9];
        }

        public CommandRecType getID()
        {
            return CommandRecType.COMREC_DATA;
        }

        @Override
        public void execute() {
            bt.tab2.addToLiveGraph(r4_temp, r4_light_bat, r4_efeu);
        }

}
