#include "Motor.h"
#include "Filter.h"

Motor m[7]; //Se crean un arreglo de 7 Motor;

//Bandera y evento serial que espera un \n para enviar los datos de los sensores
//conectados al Scorbot

boolean enviar = false;
boolean slidebase = false;

ExponentialFilter<float> ADCFilter1(20, 0);
ExponentialFilter<float> ADCFilter2(20, 0);
ExponentialFilter<float> ADCFilter3(20, 0);
ExponentialFilter<float> ADCFilter4(20, 0);
ExponentialFilter<float> ADCFilter5(20, 0);
ExponentialFilter<float> ADCFilter6(20, 0);
ExponentialFilter<float> ADCFilter7(20, 0);

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
  float temp, voltaje;
  for (int i = 0; i < 7; i++)
  {
    m[i] = {0, 0, 0};
    voltaje = (analogRead(i) * 0.004882814);
    temp = (voltaje - 0.5) * 100.0;
    if (i==0)
      ADCFilter1.SetCurrent(temp);
    if (i==1)
      ADCFilter2.SetCurrent(temp);
    if (i==2)
      ADCFilter3.SetCurrent(temp);
    if (i==3)
      ADCFilter4.SetCurrent(temp);
    if (i==4)
      ADCFilter5.SetCurrent(temp);
    if (i==5)
      ADCFilter6.SetCurrent(temp);
    if (i==6)
      ADCFilter7.SetCurrent(temp);
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
  if (pinT==0)
  {
    ADCFilter1.Filter(temp);
    temp=ADCFilter7.Current();
  }
  if (pinT==1)
  {
    ADCFilter2.Filter(temp);
    temp=ADCFilter7.Current();
  }
  if (pinT==2)
  {
    ADCFilter3.Filter(temp);
    temp=ADCFilter7.Current();
  }
  if (pinT==3)
  {
    ADCFilter4.Filter(temp);
    temp=ADCFilter7.Current();
  }
  if (pinT==4)
  {
    ADCFilter5.Filter(temp);
    temp=ADCFilter7.Current();
  }
  if (pinT==5)
  {
    ADCFilter6.Filter(temp);
    temp=ADCFilter7.Current();
  }
  if (pinT==6)
  {
    ADCFilter7.Filter(temp);
    temp=ADCFilter7.Current();
  }
    
  if ((aB == 0) && (aM == 0))
  {
    aB = temp;
    Motor m = {temp, aB, aM};
    return m;
  }
  if (((abs(temp - aB)) < 2.0))
  {
    aB = temp;
    Motor m = {temp, aB, aM};
    return m;
  }
  else
  {
    if (((abs(temp - aM)) < 2.0))
    {
      aM=aB;
      aB=temp;
      Motor m = {temp, aB, aM};
      return m;
    }
    else
    {
      aM=temp;
      temp=aB;
      Motor m = {temp, aB, aM};
      return m;
    }
  }
}



