#ifndef JSON_FILE_H
#define JSON_FILE_H

#include <ArduinoJson.h>
#include <LittleFS.h>

inline bool SaveJson(const String& path, const JsonDocument& doc) {
  File file = LittleFS.open(path, "w");
  if (!file) {
    Serial.println("Failed to open a file in writable mode.");
    return false;
  }

  // TODO: Use buffererd stream for better performance
  // (e.g. WriteBufferingStream )
  size_t size = serializeJsonPretty(doc, file);
  Serial.printf("File saved successfully: %d bytes %s\n", size, path.c_str());
  file.close();
  return true;
}

inline bool LoadJson(const String& path, JsonDocument& doc) {
  File file = LittleFS.open(path, "r");
  if (!file) {
    Serial.printf("file doesn't exist: %s\n", path.c_str());
    return false;
  }

  String jsonString = file.readString();
  Serial.println("Loaded JSON");
  Serial.println(jsonString);
  file.close();

  DeserializationError error = deserializeJson(doc, jsonString);
  if (error) {
    Serial.println("Failed to deserialize jsonString:");
    Serial.println(jsonString);
    Serial.println("Error:");
    Serial.print(error.c_str());
    return false;
  }
  return true;
}

#endif
