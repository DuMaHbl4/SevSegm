#include "SevSegm.h"

sevSegm ot(2,3,4,5,6,7,8,9,12,13,11,10); //8 анодов и 4 катода

void setup()
{
  Serial.begin(9600);
  ot.setting();
  ot.blink(2);
}

int minu;
int secu;

void loop()
{
  minu=millis()/60000%60;
  secu=millis()/1000%60;
  if(minu<10)
     ot.print(0,3);
   if(secu<10)
     ot.print(0,1);
   ot.print(secu, 0);
   ot.print(minu, 2);
}

