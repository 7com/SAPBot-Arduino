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
    m[i] = {0, 0, 0};
  }
}

void loop() {
  for (int i = 0; i < 7; i++)
  {
    m[i] = capturar((0 + i),m[i].aB, m[i].aM);
  }

  //Si se activo la bandera del evento serial, se inicia el envio de datos por serial
  if (enviar && !slidebase)
  {
    String temp = "";
    //temp()
    for (int i = 0; i < 6; i++)
    {
      if (i == 5)
      {
        temp = temp + m[i].temp;
      }
      else
      {
        temp = temp + m[i].temp + ",";
      }
    }
    String s = temp + "#\r";
    Serial.print(s);
    enviar = false;
  }
  if (enviar && slidebase)
  {
    String temp = "";
    for (int i = 0; i < 7; i++)
    {
      if (i == 6)
      {
        temp = temp + m[i].temp;
      }
      else
      {
        temp = temp + m[i].temp + ",";
      }
    }
    String s = temp + "#\r";
    Serial.print(s);
    enviar = false;
  }
}

//Función dedicada a capturar los datos de Temperatura de los sensores correspondientes
//pinT (Sensor de Temperatura).
Motor capturar(int pinT, float aB, float aM)
{
  float temp, voltaje;
  voltaje = (analogRead(pinT) * 0.004882814);
  temp = (voltaje - 0.5) * 100.0;
  if ((aB == 0) && (aM == 0))
  {
    aB = temp;
    Motor m = {temp, aB, aM};
    return m;
  }
  if (((abs(temp - aB)) < 2.0))
  {
    Motor m = {temp, aB, aM};
    return m;
  }
  else
  {
    if (((abs(temp - aM)) < 2.0))
    {
      Motor m = {temp, aB, aM};
      return m;
    }
    else
    {
      Motor m = {temp, aB, aM};
      return m;
    }
  }

  Motor m = {temp, aB, aM};
  return m;
}



