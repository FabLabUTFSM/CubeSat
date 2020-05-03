const size_t capacity = JSON_ARRAY_SIZE(2) + 2*JSON_ARRAY_SIZE(3) + JSON_ARRAY_SIZE(9) + JSON_OBJECT_SIZE(7);
DynamicJsonDocument doc(capacity);

doc["medicion"] = 10;

JsonArray GPS = doc.createNestedArray("GPS");
GPS.add(194509);
GPS.add(71.1234);
GPS.add(71.1234);

JsonArray Accelerometer = doc.createNestedArray("Accelerometer");
Accelerometer.add(3);
Accelerometer.add(3);
Accelerometer.add(3);

JsonArray Magnetometer = doc.createNestedArray("Magnetometer");
Magnetometer.add(200);
Magnetometer.add(200);
Magnetometer.add(200);
Magnetometer.add(200);
Magnetometer.add(200);
Magnetometer.add(200);
Magnetometer.add(200);
Magnetometer.add(200);
Magnetometer.add(200);
doc["Temperatura"] = 30.5;
doc["Presion"] = 1320.2;

JsonArray Gases = doc.createNestedArray("Gases");
Gases.add(1000);
Gases.add(1000);

serializeJson(doc, Serial);
