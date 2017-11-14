#include <Keyboard.h>
#include <OneWire.h>

#define NUMBER_OF_SENSORS 1
#define PREHEAT_TIME 3000

#define StatusLED 13
OneWire  ds(A0);

unsigned int Sample_Number = 1;
float Sample_Value[NUMBER_OF_SENSORS];

void Sensor_Init()
{
  pinMode(A0, INPUT);
}

void Take_Sample()
{
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  ds.search(addr);
  type_s = 0;
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  unsigned int raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // count remain gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw << 3;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw << 2; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw << 1; // 11 bit res, 375 ms
    // default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Sample_Value[0] = celsius;
}

void setup()
{
  Sensor_Init();
  Keyboard.begin();
  delay(PREHEAT_TIME);
}

void loop()
{
  digitalWrite(StatusLED, HIGH);
  Take_Sample();
  digitalWrite(StatusLED, LOW);
  Keyboard.print(Sample_Number);
  for (char cyc = 0; cyc < NUMBER_OF_SENSORS; cyc++)
  {
    Keyboard.print("\t");
    Keyboard.print(Sample_Value[cyc]);
  }
  Keyboard.print("\n");
  Sample_Number += 1;
}
