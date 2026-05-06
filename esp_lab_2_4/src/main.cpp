#include <Arduino.h>
#include <OneWire.h>

// Based on the OneWire library example

OneWire ds(15); // data wire connected to GPIO 4

void setup(void)
{
  Serial.begin(115200);
}

void loop(void)
{
  byte i;
  byte addr[8];

  if (!ds.search(addr))
  {
    Serial.println(" No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  Serial.print(" ROM =");
  for (i = 0; i < 8; i++)
  {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }
}
// #include <Arduino.h>
// #include <OneWire.h>

// OneWire ds(15);

// byte sensor1[8] = {0x28, 0x61, 0x64, 0x35, 0xC6, 0x30, 0x6A, 0x99};
// byte sensor2[8] = {0x28, 0x61, 0x64, 0x35, 0xC6, 0x0E, 0x13, 0x0C};
// byte sensor3[8] = {0x28, 0x61, 0x64, 0x35, 0xF9, 0x4D, 0x31, 0xDC};

// void setup()
// {
//   Serial.begin(115200);
// }

// float readTemp(byte addr[8])
// {
//   byte data[9];

//   ds.reset();
//   ds.select(addr);
//   ds.write(0x44);
//   delay(750);

//   ds.reset();
//   ds.select(addr);
//   ds.write(0xBE); // read scratchpad

//   for (int i = 0; i < 9; i++)
//   {
//     data[i] = ds.read();
//   }

//   int16_t raw = (data[1] << 8) | data[0];
//   return (float)raw / 16.0;
// }

// void loop()
// {
//   Serial.print("T1: ");
//   Serial.println(readTemp(sensor1));

//   Serial.print("T2: ");
//   Serial.println(readTemp(sensor2));

//   Serial.print("T3: ");
//   Serial.println(readTemp(sensor3));

//   Serial.println("------");
//   delay(2000);
// }