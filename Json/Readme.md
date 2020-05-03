# Json

[Libreria](https://arduinojson.org/)

## Json V1

Autor: [Jose Tomas Dominguez](https://github.com/josetomas)

Este codigo simplemente genera un Json, y lo imprime a traves del puerto serial, explica como utilizar todas las funciones que necesitamos de la libreria ArduinoJson.

### Codigo Actual - Json V2
Autores: [Jose Tomas Dominguez](https://github.com/josetomas), Nicolas Carrasco

Union de Propuesta_V1, en lugar de dejar los datos en un buffer y subirlos a la SD, esta los almacena en un json y luego los sube a la SD.

### Trabajos pendientes


#### Tareas
- Unir Propuesta_V1 con esta. OK!
- Guardar datos en la tarjeta. SD OK!
- BMP: Que es OverSampling y los filtros.
- Integrar GPS OK! 
- Definir si el GPS funcionara por I2C, Software Serial o Hardware Serial.
- Averiguar que significa Set GPSECHO. - Ver ejemplo de parsing. 
- Probar en placa. 

## Referencias


- Almacenar la información en formato [Json](https://www.json.org/json-en.html), a tráves de la libreria [Arduino Json](https://arduinojson.org/). 
- [Documentacion de serializacion](https://arduinojson.org/v6/doc/serialization/)
