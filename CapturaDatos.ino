#include "motor.h"

//Bandera y evento serial que espera un \n para enviar los datos de los sensores
//conectados al Scorbot

boolean enviar = false;

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      enviar = true;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  //Se crean un arreglo de 6 Motor y se capturan los datos.
  Motor m[6];
  for (int i = 0; i < 6; i++)
  {
    m[i] = capturar(0 + i, 8 + i);
  }

  //Si se activo la bandera del evento serial, se inicia el envio de datos por serial
  if (enviar)
  {
    String volt="";
    String freq="";
    String temp="";
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
}

//Función dedicada a capturar los datos del PWM y Temperatura de los sensores correspondientes
//pinE (Sensor de Voltaje), pinT (Sensor de Temperatura).
Motor capturar(int pinE, int pinT)
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
    hz=-1;
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

  Motor m = {volt, hz, temp};
  return m;
}



