//        LIBRERIAS Y DEFINICIONES
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include "Adafruit_SGP30.h"
#include <Adafruit_FXOS8700.h>
#include <Adafruit_FXAS21002C.h>
#include "Adafruit_MCP9808.h"
#include <SD.h>
//FALTA LA DEL GPS

File dataFile;

#define SEALEVELPRESSURE_HPA (1013.25) //presion a nivel de mar

Adafruit_SGP30 sgp; //objeto sensor de gases
Adafruit_BMP3XX bmp; //objeto sensor de presion - conexion i2c
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808(); //OBJETO SENS. TEMPERATURA

#define adlx_x A0
#define adlx_y A1
#define adlx_z A2
int scale = 200; // 200 (±200g) for ADXL377
//boolean reference_is_5V = true; //referencia de voltaje para adlx377

Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);//objeto ACELEROMETRO-MAGNETOMETRO
Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);// OBJETO GIROSCOPIO

//        VARIABLES

char buffer[256];
int n = 8;
int contador;
int contsgp;

float TEMPERATURA;
int ECO2;
int H2;
int ETA;
int TVOC;
float ACEL_X; //ACELERACION MACRO
float ACEL_Y;
float ACEL_Z;
float PRESION;
float ORIENTACION[9]; // ax,ay,az,gx,gy,gx,mx,my,mz

//    MAPEO DE FLOTANTE
float mapf(double val, double in_min, double in_max, double out_min, double out_max)
{
   return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  Serial.begin(115200);
  
// SETUP SGP30

  if (!bmp.begin()){
    dataFile.println("Error al iniciar SG30");
  }
  else {
    dataFile.println("SGP30 connected!");
  }
// SETUP BMP388 
  if (!bmp.begin()){
    dataFile.println("Error al iniciar BMP388");
  }
  else {
    dataFile.println("BMP388 connected!");
  }
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);

// SETUP MCP808
    if (!tempsensor.begin(0x18)) {
        dataFile.println("Couldn't find MCP9808!");
        while (1);
    }
    else{
        dataFile.println("MCP9808 Connected!");
        tempsensor.wake();   // wake up, ready to read!
    }
// SETUP ADLX377 
 // NO APLICA SETUP

// SETUP 9DOF
//     ACELEROMETRO
if(!accelmag.begin(ACCEL_RANGE_4G)) /* Initialise the sensor */
    {
        dataFile.println("Couldn't find FXOS8700 detected!");
    }    
    else
    {
        dataFile.println("FXOS8700 Connected!");
    }

//    GIROSCOPIO
    if(!gyro.begin()) /* Initialise the sensor */
    {
        dataFile.println("Couldn't find FXAS21002C!");
    }
    else
    {
        dataFile.println("FXAS21002C connected!");
    }
// FALTA GPS!!!!
// FIN CONFIGURACION DE SENSORES
    dataFile.print("Inicio de lectura de datos   ");
    
    }
/*  SGP30
 *   sgp.IAQmeasure() -> HACE UNA MEDICION DE TVOC Y CO2 Y GUARDA VALORES EN VARIABLES TVOC Y eCO2
 *   sgp.IAQmeasureRaw() -> Analogo pero con H2 y etanol
 *   getIAQBaseline(var1,var2) -> solicita valores de calibracion de eco2 y tvoc
 *   setIAQBaseline(var1,var2) -> asigna luego de una serire de mediciones valoresde calibracion para eco2 y tvoc
 *   
 *   BMP388
 *   bmp.performReading() -> realiza la lectura y guarda los datos 
 *   bmp.pressure -> entrega presion
 *   bmp.temperature -> entrega temperatura
 *   
 *   ADXL377
 *   LECTURA ANALOGICA
 *   
 *   termometro
 *   float -> temsensor.readTempC() -> entrega emperatura en celcius
 *   
 *   
 *   
 */

void acelerometro(){ //MAGNETOMETRO INCLUIDO
  sensors_event_t aevent, mevent;  
  accelmag.getEvent(&aevent, &mevent); // realiza una medicion y la guarda internamente
  ORIENTACION [0] = aevent.acceleration.x; // se asignan los valores a su respectiva posicion en el arreglo
  ORIENTACION [1] = aevent.acceleration.y;
  ORIENTACION [2] = aevent.acceleration.z;
  ORIENTACION [6] = mevent.magnetic.x;
  ORIENTACION [7] = mevent.magnetic.y;
  ORIENTACION [8] = mevent.magnetic.z;
}
void giroscopio(){
    sensors_event_t event;  /* Get a new sensor event */
    gyro.getEvent(&event); 
    ORIENTACION [3] = event.gyro.x;// se asignan los valores a su respectiva posicion en el arreglo
    ORIENTACION [4] = event.gyro.y;
    ORIENTACION [5] = event.gyro.z;
}
void SGP30(){
   if (sgp.IAQmeasure()) {
    TVOC= sgp.TVOC;
    ECO2 = sgp.eCO2;
  }

  /*else{  //EN CASO DE FALLO DE LA MEDICION RETORNA UN VALOR MARCADOR
    TVOC=0;
    ECO2=0;
  }*/

  if ( sgp.IAQmeasureRaw()) {
    ETA= sgp.rawEthanol;
    H2 = sgp.rawH2;
  }

 /* else{
    ETA=0;
    H2=0;
  }*/
}

void ADLX (){ //asigna el mapeo de la medicion de los pines analogos de cada eje
  ACEL_X = mapf(analogRead(adlx_x), 0, 675, -scale, scale); // 3.3/5 * 1023 =~ 675 (no se por que hace esta conversion seguro por temas del voltaje dentro de la breackboard
  ACEL_Y = mapf(analogRead(adlx_y), 0, 675, -scale, scale);
  ACEL_Z = mapf(analogRead(adlx_z), 0, 675, -scale, scale);
}

//temperatura es medicion directa no requiere funcion void -> tempsensor.readTempC();

void BMP(){
  if (bmp.performReading()){
   PRESION=bmp.pressure;
  }
  else{ //asigno valor marcador para indicar que no se realizó la lectura
    PRESION=0;
  }
}

void loop() {
  contador++;
  ADLX();
  giroscopio();
  acelerometro();
  BMP();
  
  if (contador==n){
    contador =0;
    //medicion de gps
    TEMPERATURA= tempsensor.readTempC();
    SGP30();
    int i=0;
    sprintf(buffer, "%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f%.4f,%.4f,%.4f,%.4f,%.4f,%d,%d,%d,%d\r\n", //otorga el formato de la cadena, .4f indica que se concatenara un float hasta e cuarto decimal lo demas se truncan
    ORIENTACION[i], ORIENTACION[i++], ORIENTACION[i++],ORIENTACION[i++],ORIENTACION[i++],
    ORIENTACION[i++],ORIENTACION[i++],ORIENTACION[i++],ORIENTACION[i++],
    ACEL_X,ACEL_Y, ACEL_Z, PRESION,
    TEMPERATURA,TVOC, ECO2,H2,ETA); //ojo que hasta aca es a misma linea
    dataFile=SD.open("TEST1.bin",FILE_WRITE);//abro el archivo en la sd
    dataFile.write(buffer,strlen(buffer));//imprimo enn la sd el buffer pero solo los bit usados
    //dataFile.write alog gps
    dataFile.close();
  }
  else{
    int i=0;
    sprintf(buffer, "%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f%.4f,%.4f,%.4f,%.4f\r\n", //otorga el formato de la cadena, .4f indica que se concatenara un float hasta e cuarto decimal lo demas se truncan
    ORIENTACION[i], ORIENTACION[i++], ORIENTACION[i++],ORIENTACION[i++],ORIENTACION[i++],
    ORIENTACION[i++],ORIENTACION[i++],ORIENTACION[i++],ORIENTACION[i++],
    ACEL_X,ACEL_Y, ACEL_Z, PRESION);
    dataFile=SD.open("TEST1.bin",FILE_WRITE);
    dataFile.write(buffer,strlen(buffer));//imprimo enn la sd el buffer pero solo los bit usados
    dataFile.close();
  }
}