#include "motor.h"

Motor m[7]; //Se crean un arreglo de 7 Motor;

//Bandera y evento serial que espera un \n para enviar los datos de los sensores
//conectados al Scorbot

boolean enviar = true;
boolean slidebase = false;
int to = 150;
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
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);
  pinMode(A13, INPUT);
  pinMode(A14, INPUT);
  for (int i = 0; i < 7; i++)
  {
    m[i] = {0, 0, 0, 0, 0, 0, 0};
  }
}

void loop() {
  for (int i = 0; i < 7; i++)
  {
    m[i] = capturar((0 + i), (8 + i), m[i].aB, m[i].aM, m[i].freqA, m[i].voltA);
  }

  //Si se activo la bandera del evento serial, se inicia el envio de datos por serial
  if (enviar && !slidebase)
  {
    //enviar = false;
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
    Serial.println(s);
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
Motor capturar(int pinT, int pinE, float aB, float aM, float freqA, float voltA)
{
  unsigned long high, low, cycle;
  float dutyCycle1, dutyCycle2, voltage, temp, hz, volt;

  if (pinE == 8)
  {
    high = pulseIn(A8, HIGH, to);
    low = pulseIn(A8, LOW, to);
  }
  if (pinE == 9)
  {
    high = pulseIn(A9, HIGH, to);
    low = pulseIn(A9, LOW, to);
  }
  if (pinE == 10)
  {
    high = pulseIn(A10, HIGH, to);
    low = pulseIn(A10, LOW, to);
  }
  if (pinE == 11)
  {
    high = pulseIn(A11, HIGH, to);
    low = pulseIn(A11, LOW, to);
  }
  if (pinE == 12)
  {
    high = pulseIn(A12, HIGH, to);
    low = pulseIn(A12, LOW, to);
  }
  if (pinE == 13)
  {
    high = pulseIn(A13, HIGH, to);
    low = pulseIn(A13, LOW, to);
  }
  if (pinE == 14)
  {
    high = pulseIn(A14, HIGH, to);
    low = pulseIn(A14, LOW, to);
  }

  voltage = (analogRead(pinT) * 0.004882814); //Volt Temperatura
  cycle = high + low;
  dutyCycle1 =  (float)high / float(cycle);
  dutyCycle2 =  (float)low / float(cycle);


  if (cycle != 0)
  {
    hz = 1000000 / (float)cycle;
    freqA = hz;
  }
  else
  {
    hz = freqA;
  }


  if (high == 0 || low == 0) {
    volt = voltA;
  }

  else
  {
    if (dutyCycle1 == dutyCycle2)
    {
      volt = 0;
      voltA = volt;
    }
    else
    {
      if (dutyCycle1 > dutyCycle2)
      {
        volt = (12 * (dutyCycle1 - dutyCycle2));
        voltA = volt;
      }
      else
      {
        volt = -1 * (12 * (dutyCycle2 - dutyCycle1));
        voltA = volt;
      }
    }
  }
  temp = (voltage - 0.5) * 100.0;
  if ((aB == 0) && (aM == 0))
  {
    aB = temp;
    Motor m = {volt, hz, temp, voltA, freqA, aB, aM};
    return m;
  }
  if (((abs(temp - aB)) < 2.0))
  {
    Motor m = {volt, hz, temp, voltA, freqA, aB, aM};
    return m;
  }
  else
  {
    if (((abs(temp - aM)) < 2.0))
    {
      Motor m = {volt, hz, temp, voltA, freqA, aB, aM};
      return m;
    }
    else
    {
      Motor m = {volt, hz, temp, voltA, freqA, aB, aM};
      return m;
    }
  }

  Motor m = {volt, hz, temp, voltA, freqA, aB, aM};
  return m;
}



