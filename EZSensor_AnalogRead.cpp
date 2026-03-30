/*
 * Project myProject
 * Author: Your Name
 * Date:
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

#define EZAnalog A0
int led_on = 0;
float mode(float *x, int n);

void setup()
{
  pinMode(EZAnalog, INPUT);
  pinMode(D1, OUTPUT);
}

unsigned int sampleSize = 9;
float analogSample[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

int compare(const void *a, const void *b)
{
  return *(float *)(a) - *(float *)(b);
}

void loop()
{
  led_on ^= 1;
  digitalWrite(D1, led_on ? HIGH : LOW);

  for (unsigned int i = 0; i < sampleSize; i++)
  {
    int analogSensor = analogRead(EZAnalog);

    analogSample[i] = float(analogSensor) * (512.0 / 4095.0) * 2.54;
    delay(50);
  }

  Serial.println("Blink");
  Serial.print("Analog = ");
  for (unsigned int i = 0; i < sampleSize; i++)
  {
    Serial.print(analogSample[i]);
    Serial.print(" ");
  }
  Serial.println();

  qsort(analogSample, sampleSize, sizeof(float), compare);
  float med = mode(analogSample, sampleSize);
  Serial.print("Median = ");
  Serial.println(med);

  delay(1000);
}

// Mode function, returning the mode or median.
float mode(float *x, int n)
{
  int i = 0;
  int count = 0;
  int maxCount = 0;
  float mode = 0;
  int bimodal;
  int prevCount = 0;
  while (i < (n - 1))
  {
    prevCount = count;
    count = 0;
    while (x[i] == x[i + 1])
    {
      count++;
      i++;
    }
    if (count > prevCount && count > maxCount)
    {
      mode = x[i];
      maxCount = count;
      bimodal = 0;
    }
    if (count == 0)
    {
      i++;
    }
    if (count == maxCount)
    { // If the dataset has 2 or more modes.
      bimodal = 1;
    }
    if (mode == 0 || bimodal == 1)
    { // Return the median if there is no mode.
      mode = x[(n / 2)];
    }
    return mode;
  }
  return 0;
}