#include <ArduinoJson.h>
DynamicJsonDocument doc(42);


void setup() {
  Serial.begin(9600);

}

void loop() {
  JsonArray obj1 = doc.createNestedArray("obj1");
  JsonArray obj2 = doc.createNestedArray("obj2");
  obj1.add(random(300));
  obj2.add(random(300));
  obj2.add(random(300));
  serializeJson(doc, Serial);
  Serial.println(" ");
  doc.clear();
}
"Hola!"
