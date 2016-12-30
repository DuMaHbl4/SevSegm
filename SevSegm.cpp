//Автор - Дмитрий Догин (связь: https://vk.com/duino_du4) 12.2016

#include "SevSegm.h"

byte numberSegment[13] = {
  0b00111111, //0   0
  0b00000110, //1   1
  0b01011011, //2   2
  0b01001111, //3   3
  0b01100110, //4   4
  0b01101101, //5   5
  0b01111101, //6   6
  0b00000111, //7   7
  0b01111111, //8   8
  0b01101111, //9   9 
  0b01000000, //-   10
  0b00000000, //    11
  0b10000000, //.   12
  };


sevSegm::sevSegm(int p0, int p1, int p2, int p3, int p4, int p5, int p6, int p7, int k0, int k1, int k2, int k3)
{
  anod[0]=p0;
  pinMode(p0, OUTPUT);
  anod[1]=p1;
  pinMode(p1, OUTPUT);
  anod[2]=p2;
  pinMode(p2, OUTPUT);
  anod[3]=p3;
  pinMode(p3, OUTPUT);
  anod[4]=p4;
  pinMode(p4, OUTPUT);
  anod[5]=p5;
  pinMode(p5, OUTPUT);
  anod[6]=p6;
  pinMode(p6, OUTPUT);
  anod[7]=p7;
  pinMode(p7, OUTPUT);
  katod[0]=k0;
  pinMode(k0, OUTPUT);
  digitalWrite(k0, LOW);
  numKat=1;
  if(k1!=0)
  {
    katod[1]=k1;
    pinMode(k1, OUTPUT);
    digitalWrite(k1, LOW);
    numKat++;
  }
  if(k2!=0)
  {
    katod[2]=k2;
    pinMode(k2, OUTPUT);
    digitalWrite(k2, LOW);
    numKat++;
  }
  if(k3!=0)
  {
    katod[3]=k3;
    pinMode(k3, OUTPUT);
    digitalWrite(k3, LOW);
    numKat++;
  }
  uint8_t i=0;
  for(i=0; i<numKat; i++) //обнуляем маску и блинк
  {
    mask[i]=0;
    sec[0]=0;
  }
  trabl=false;
}

void sevSegm::clear()
{
  for(int i=0; i<numKat; i++)
  {
    mask[i]=numberSegment[11];
  }
}

void sevSegm::blink(int p)
{
  if(p<numKat)
    sec[p]=true;
}

void sevSegm::noBlink(int p)
{
  if(p<numKat)
    sec[p]=false;
}

void sevSegm::rePrint()
{
  int i;
  for(i=0; i<numKat; i++) //выводим
  {
    for(int p=0; p<numKat; p++)
      digitalWrite(katod[p], HIGH);
    for(int l=0; l<8; l++)
      digitalWrite(anod[l],bitRead(mask[i], l));
    if(sec[i] && (1000-millis()%1000>800)) //проверка на блинк
      digitalWrite(anod[7], HIGH);
    if(trabl&&(millis()%500>300))  //если зашли зашли за границы то мигаем точками
      digitalWrite(anod[7], HIGH);
    else
    {
      if(!bitRead(mask[i],7) && !sec[i] && !millis()%1000>300)
        digitalWrite(anod[7], LOW);
    }  
    digitalWrite(katod[i], LOW);
      delay(4);
    digitalWrite(katod[i], HIGH);
  }
}

void sevSegm::setting()
{
  uint8_t buf; //переменная для копирования
  uint8_t num; //число уже записаных номеров
  uint8_t y[8]; //записаные номера
  uint8_t pic=0;
  uint8_t i;
  uint8_t j;
  uint8_t h;
  uint8_t newe[8];
  boolean a=true;; //для проверки на совпадение
  for(i=0; i<8; i++)
    y[i]=0;
  i=0;
  Serial.println("Nastroyka poriadka anodov.\nVvedite nomer izobrajenia, kotoroe sootvetstvuet izobrajeniu na semisegmentnike");
  Serial.println(" ****      ====      ====      ====      ====      ====      ====      ====");
  Serial.println("||  ||    ||  **    ||  ||    ||  ||    ||  ||    **  ||    ||  ||    ||  ||");
  Serial.println("||  ||    ||  **    ||  ||    ||  ||    ||  ||    **  ||    ||  ||    ||  ||");
  Serial.println(" ====      ====      ====      ====      ====      ====      ****      ====");
  Serial.println("||  ||    ||  ||    ||  **    ||  ||    **  ||    ||  ||    ||  ||    ||  ||");
  Serial.println("||  ||    ||  ||    ||  **    ||  ||    **  ||    ||  ||    ||  ||    ||  ||");
  Serial.println(" ==== 0    ==== 0    ==== 0    **** 0    ==== 0    ==== 0    ==== 0    ==== *");
  Serial.println("   0         1         2         3         4         5         6         7");
  for(h=0; h<8; h++)
    digitalWrite(anod[h], LOW);
  digitalWrite(anod[0], HIGH);
  while(i!=8)
  {
    a=true;
    if(Serial.available()>0)
    {
      pic=Serial.read()-48;
      if(pic<0 || pic>7)
        Serial.println("Vvedite znachenie ot 0 do 7");
      else
      {
        for(j=0; j<i; j++)
        {
          if(y[j]==pic)
            a=a*0;
        }
        if(a)
        {
          newe[i]=anod[pic];
          y[i]=pic;
          i++;
          for(h=0; h<8; h++)
          {
            digitalWrite(anod[h], LOW);
          }
          digitalWrite(anod[i], HIGH);
        }
        else
          Serial.println("Nomer zanait");
      }
    }
  }
  for(i=0; i<8; i++)
    anod[i]=newe[i];
  Serial.println("Nastroika poriadka katodov.\nVvedy nomer segmenta, kotoriy gorit, schitaia sleva napravo (->) s 0");
  Serial.println(" ****      ****      ****   ");
  Serial.println("**  **    **  **    **  **  ");
  Serial.println("**  **    **  **    **  **  ");
  Serial.println(" ****      ****      ****   ");
  Serial.println("**  **    **  **    **  **  ");
  Serial.println("**  **    **  **    **  **  ");
  Serial.println(" **** *    **** *    **** * ....... ");
  Serial.println("   0         1         2         3  ");
  for(i=0; i<numKat; i++) //зануляем используемые для анодов массивs значений, для работы с катодами
  { 
    y[i]=-1;
    newe[i]=0;
  }
  for(i=0; i<numKat; i++)
    digitalWrite(katod[i], HIGH);
  for(i=0; i<8; i++)
    digitalWrite(anod[i], HIGH);
  digitalWrite(katod[numKat-1],LOW);
  pic=-1;
  i=numKat-1;
  while(i!=255)
  {
    a=true;
    if(Serial.available()>0)
    {
      pic=Serial.read()-48;
      if(pic<0 || pic>=numKat)
        Serial.println("Vvedite znachenie ot 0 do 3");
      else
      {
        for(j=0; j<numKat; j++)
        {
          if(y[j]==pic)
            a=a*0;
        }
        if(a)
        {
          y[i]=pic;
          newe[pic]=katod[i];
          i--;
          for(j=0; j<numKat; j++)
            digitalWrite(katod[j], HIGH);
          digitalWrite(katod[i],LOW);
        }
        else
          Serial.println("Nomer zaniat");
      }
    }
  }
  for(i=0; i<numKat; i++)
    katod[i]=newe[numKat-1-i];
  Serial.print("Pri ob'iavlenii classa sevSegm neobhodimo ukazat' pini v sleduushem poriadke\n(");
  for(i=0; i<8; i++)
  {
    Serial.print(anod[i]);
    Serial.print(',');
  }
  for(i=0; i<numKat; i++)
  {
    Serial.print(katod[i]);
    if(i!=numKat-1)
      Serial.print(',');
  }
  Serial.println(')');
}

void sevSegm::print(int h, int pos)
{
  uint8_t p=pos; //запоминаем позицию
  uint8_t i=0; //переменные для счетчиков
  uint8_t j=0;
  uint8_t y[numKat]; //цифры
  boolean otr=false; //если отрицательное
  if(h==0)
    i=1;
  if(h<0)
  {
    otr=true;
    h=-h;
  }
  while(h!=0) //разбиваем число на массив из цифр
  {
    y[i]=h%10;
    h=h/10;
    i++;
  }
  for(j=pos; j<numKat; j++)
  {
    if(i!=0)
    {
      mask[j]=numberSegment[y[j-pos]];
      i--;
    }
    else
    {
      if(otr)
      {
        mask[j]=numberSegment[10];
        otr=false;
      }
    }
  }
  if(i!=0||otr==true) //определяем выходит ли за границы выводимое значение
    trabl=true;
  rePrint();
}

void sevSegm::print(float h, int pos)
{
  trabl=false;
  boolean otr=false; //если отрицательное
  uint8_t p=pos;
  uint8_t i=0; //расположение точки
  uint8_t j=0; //число цифр
  int s;
  uint8_t y[numKat];
  if(h<0)
  {
    otr=true;
    h=-h;
  }
  if(h==(int)h)  
  {
    h=h*10;
    i++;
  }
  while(h!=(int)h) //смещаем точку у дробного и считаем ее положение
  {
    h=h*10;
    i++;
  }
  s=h;
  while(j!=2) //разбираем число по цифрам и считаем разряд(на случай если h==0)
  {
    y[j]=s%10;
    s=s/10;
    j++;
  }
  while(s!=0) //разбираем число по цифрам и считаем разряд
  {
    y[j]=s%10;
    s=s/10;
    j++;
  }
  for(int k=pos; k<numKat; k++)
  {
    if(j!=0)
    {
      mask[k]=numberSegment[y[k-p]];
      if(k-p==i) //добавляем 128 если на данной маске должна стоять точка
      {
        mask[k]=128+mask[k];
      }
      j--;
    }
    else
    {
      if(otr)
      {
        mask[k]=numberSegment[10];
        otr=false;
      } 
    }
  }
  if(j!=0||otr==true)
    trabl=true;
  rePrint(); 
}
