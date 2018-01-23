#include "motor.h"

Motor m[7]; //Se crean un arreglo de 7 Motor;

//Bandera y evento serial que espera un \n para enviar los datos de los sensores
//conectados al Scorbot

boolean enviar = false;
boolean slidebase = false;

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '0') {
      enviar = true;
      slidebase = false;
    }
    if (inChar == '1') {
      enviar = true;
      slidebase = true;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < 7; i++)
  {
    m[i] = {0, 0, 0, 0, 0};
  }
}

void loop() {
  for (int i = 0; i < 7; i++)
  {
    m[i] = capturar(0 + i, 8 + i, m[i].aB, m[i].aM);
  }

  //Si se activo la bandera del evento serial, se inicia el envio de datos por serial
  if (enviar && !slidebase)
  {
    String volt = "";
    String freq = "";
    String temp = "";
    //voltaje() + freq() + temp()
    for (int i = 0; i < 6; i++)
    {
      if (i == 5)
      {
        volt = volt + m[i].volt + ";";
        freq = freq + m[i].freq + ";";
        temp = temp + m[i].temp;
      }
      else
      {
        volt = volt + m[i].volt + ",";
        freq = freq + m[i].freq + ",";
        temp = temp + m[i].temp + ",";
      }
    }
    String s = volt + freq + temp + "#\r";
    Serial.print(s);
    enviar = false;
  }
  if (enviar && slidebase)
  {
    String volt = "";
    String freq = "";
    String temp = "";
    //voltaje() + freq() + temp()
    for (int i = 0; i < 7; i++)
    {
      if (i == 6)
      {
        volt = volt + m[i].volt + ";";
        freq = freq + m[i].freq + ";";
        temp = temp + m[i].temp;
      }
      else
      {
        volt = volt + m[i].volt + ",";
        freq = freq + m[i].freq + ",";
        temp = temp + m[i].temp + ",";
      }
    }
    String s = volt + freq + temp + "#\r";
    Serial.print(s);
    enviar = false;
  }
}

//Función dedicada a capturar los datos del PWM y Temperatura de los sensores correspondientes
//pinE (Sensor de Voltaje), pinT (Sensor de Temperatura).
Motor capturar(int pinT, int pinE, float aB, float aM)
{
  unsigned long high, low, cycle;
  float dutyCycle1, dutyCycle2, voltage, temp, hz, volt;

  high = pulseIn(pinE, HIGH, 100);
  low = pulseIn(pinE, LOW, 100);
  voltage = (analogRead(pinT) * 0.004882814);
  cycle = high + low;
  dutyCycle1 =  (float)high / float(cycle);
  dutyCycle2 =  (float)low / float(cycle);


  if (cycle != 0)
  {
    hz = 1000000 / (float)cycle;
  }
  else
  {
    hz = -1;
  }


  if (high == 0 || low == 0) {
    volt = 12;
  }

  else
  {
    if (dutyCycle1 == dutyCycle2)
    {
      volt = 0;
    }
    else
    {
      if (dutyCycle1 > dutyCycle2)
      {
        volt = (12 * (dutyCycle1 - dutyCycle2));
      }
      else
      {
        volt = -1 * (12 * (dutyCycle2 - dutyCycle1));
      }
    }
  }
  temp = (voltage - 0.5) * 100.0;
  if ((aB==0) && (aM==0))
  {
    aB = temp;
    Motor m = {volt, hz, temp, aB, aM};
    return m;
  }
  if (((abs(temp-aB))<2.0))
  { 
    Motor m = {volt, hz, temp, temp, aM};
    return m;
  }
  else
  {
    if (((abs(temp-aM))<2.0))
    {
      Motor m = {volt, hz, temp, temp, aM};
      return m;
    }
    else
    {
      Motor m = {volt, hz, aB, aB, temp};
      return m;
    }
  }

  Motor m = {volt, hz, temp, aB, aM};
  return m;
}



