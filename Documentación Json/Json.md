# Bibligrafía 

[Libreria](https://arduinojson.org/)

[Documentacion de serializacion](https://arduinojson.org/v6/doc/serialization/)

# Desarrollo Json

## V1

Json sugerido Joseto: {"medicion": i, "GPS":[Time GMT, Lat, Long], "Accelerometer": [x,y,z], "Magnetometer": [O1,O2,O3,O4,O5,O6,O7,O8,O9], "Temperatura":temp, "Presion": presion, "Gases": [VOC,co2]}

Ejemplo:

{
"medicion": 
10, 
"GPS":[
194509.000, 
71.1234, 
71.1234
],
"Accelerometer": [
3,
3,
3], 
"Magnetometer": [
200,
200,
200,
200,
200,
200,
200,
200,
200],
"Temperatura":
 30.5,
"Presion": 
1320.20 , 
"Gases": [
1000,
1000
]
}

Peso del JSON medido a traves de [ArduinoJson Assistant](https://arduinojson.org/v6/assistant/) 

El cual tiene un peso de 450 bytes en el microcontrolador STM32. 

