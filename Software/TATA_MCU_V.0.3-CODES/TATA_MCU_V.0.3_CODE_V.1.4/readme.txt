Changed the Over-Temperature fault mechanisim
    ALGO : if(OT detected){
            send Message without request
            then, Turn on timer 1 in CTC mode for 3 sec 
            then, at ISR for comp A, Turn off timer 1 , reset the check condition flag and again repeat the process
            }