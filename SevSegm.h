//Автор - Дмитрий Догин (связь: https://vk.com/duino_du4) 12.2016

#ifndef SEVSEGM_H
#define SEVSEGM_H

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class sevSegm
{
  private:
    uint8_t anod[8]; //пины на которых лежат аноды
    uint8_t katod[4]; //пины на которых лежат катоды
    unsigned int mask[4]; //маски соответствующие катодам
    boolean trabl; //true если есть выход за границы
    boolean sec[4]; //катоды, на которых необходимо мигать точкой раз в секунду
	uint8_t numKat; //число катодов 
  public:
    sevSegm(int p0, int p1, int p2, int p3, int p4, int p5, int p6, int p7, int k0, int k1=0, int k2=0, int k3=0); //8 анодов и минимум 1 и максимум 4 катода
    void rePrint(); //вывод текущего изображения
    void print(int h, int pos=0); //вывод целого числа
    void print(float p, int pos=0); //вывод дробного числа
    void setting();  //настройка порядка подключенный пинов
    void blink(int p); //мигать точкой, параметр - номер катода(цифры)
    void noBlink(int p); //не мигать точкой, параметр - номер катода(цифры)
    void clear();  //очистка масок (всего экрана)
};

#endif
