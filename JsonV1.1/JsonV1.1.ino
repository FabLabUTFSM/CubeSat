#include <ArduinoJson.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include "Adafruit_SGP30.h"
#include <Adafruit_FXOS8700.h>
#include <Adafruit_FXAS21002C.h>
#include "Adafruit_MCP9808.h"

//pines
#define chipSelect 4
#define adlx_x A0
#define adlx_y A1
#define adlx_z A2

//Variables
//int scale = 200; mapea los datos del accelerometro No lo estoy usando, porque nadie sabe porque esta, por ultimo mapeamos despues los datos.
const int sampleSize = 10; //tomar mas muestras para reducir el error del accelerometro, puede ser 1.
#define capacity 521 //peso del json
#define SEALEVELPRESSURE_HPA 1013.25 //config sensor de presion

//declarar componentes
DynamicJsonDocument doc(capacity);
File dataFile = SD.open("Data.txt", FILE_WRITE);
Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);//objeto ACELEROMETRO-MAGNETOMETRO
Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);// OBJETO GIROSCOPIO
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808(); //OBJETO SENS. TEMPERATURA
Adafruit_BMP3XX bmp; //objeto sensor de presion
Adafruit_SGP30 sgp; //objeto sensor de gases



void setup() {
  Serial.begin(1152000);
  // prueba componentes
    //SD
  while (!Serial) {
    }
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
    //Magnetometro
  if(!accelmag.begin(ACCEL_RANGE_4G)){
        Serial.println("Couldn't find FXOS8700 detected!");
    }    
   else
    {
        Serial.println("FXOS8700 Connected!");
    }

    //    GIROSCOPIO
   if(!gyro.begin()) /* Initialise the sensor */
    {
        Serial.println("Couldn't find FXAS21002C!");
    }
   else
    {
        Serial.println("FXAS21002C connected!");
    }
    // Sensor de Temp
    if (!tempsensor.begin(0x18)) {
        dataFile.println("Couldn't find MCP9808!");
        while (1);
    }
    else{
        dataFile.println("MCP9808 Connected!");
        tempsensor.wake();   // wake up, ready to read!
    }

    //Sensor presion
    if (!bmp.begin()) {
      Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    }

    // Set up oversampling and filter initialization - Tarea pendiente
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    //bmp.setOutputDataRate(BMP3_ODR_50_HZ);

    //Sensor de gases
    if (!bmp.begin()){
      Serial.println("Error al iniciar SG30");
    }
    else {
      Serial.println("SGP30 connected!");
    }
}

void loop() {
  //configurar Json
  JsonArray Accelerometer = doc.createNestedArray("Accelerometer");
  JsonArray Magnetometer = doc.createNestedArray("Magnetometer");
  JsonArray Presion = doc.createNestedArray("Presion");
  JsonArray Gases = doc.createNestedArray("Gases");
  //Lecturas acelerometro
  Accelerometer.add(ReadAxis(adlx_x));
  Accelerometer.add(ReadAxis(adlx_y));
  Accelerometer.add(ReadAxis(adlx_z));

  //Lecturas 9DOF
  sensors_event_t aevent, mevent;  
  accelmag.getEvent(&aevent, &mevent); // realiza una medicion y la guarda internamente
  Magnetometer.add(aevent.acceleration.x);
  Magnetometer.add(aevent.acceleration.y);
  Magnetometer.add(aevent.acceleration.z);
  Magnetometer.add(mevent.magnetic.x);
  Magnetometer.add(mevent.magnetic.y);
  Magnetometer.add(mevent.magnetic.z);
  sensors_event_t event;  // Get a new sensor event 
  gyro.getEvent(&event); 
  Magnetometer.add(event.gyro.x);
  Magnetometer.add(event.gyro.y);
  Magnetometer.add(event.gyro.z);

  //Lectura temp
  doc["Temperatura"] = tempsensor.readTempC();

  //Lectura presion
  bmp.performReading();
  Presion.add(bmp.pressure / 100.0); //en hPa
  Serial.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  Presion.add(bmp.temperature);

  //Lectura gases
  sgp.IAQmeasure();
  Gases.add(sgp.TVOC);
  Gases.add(sgp.eCO2);
  sgp.IAQmeasureRaw();
  Gases.add(sgp.rawEthanol);
  Gases.add(sgp.rawH2);
  
  
  //enviar a SD
  serializeJson(doc, dataFile);
  doc.clear();
}

int ReadAxis(int axisPin)
{
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int i = 0; i < sampleSize; i++)
  {
    reading += analogRead(axisPin);
  }
  return reading/sampleSize;
}
