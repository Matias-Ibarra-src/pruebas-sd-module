#include <Arduino.h>
#include <SD.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"

File myFile;


enum TypeOfFile {
  txt = 1,
  json,
  dat
};

const char *filesSPIFF[] = {
  "/_id", 
  "/environment.json", 
  "/config.json", 
  "/failedPackages.json"
}; 


float DummyRead(){
  float numero;
  numero = (float)(rand() % 65);
  return numero;
}

int ParseType(String Type){
  if(!strcmp(Type.c_str(),".txt"))return txt;
  if(!strcmp(Type.c_str(),".json"))return json;
  if(!strcmp(Type.c_str(),".dat"))return dat;
  return 0;
}

void writeFileSD( String FileName , String content, String mode){
  
  //int Type = ParseType(Types);
  char *typeStr = strchr(FileName.c_str(), '.');
  int Type = ParseType(typeStr);
  
  int counter;

  myFile = SD.open(FileName.c_str(), mode.c_str());
  if (!myFile){ 
    Serial.println("[FAIL] : File couldn't be created"); 
  } 
  
  switch (Type){
    case 1:
      if (myFile.println(content.c_str())){ 
        Serial.println("[SUCCESS] : File wrote correctly"); 
        myFile.close();
      } else{ 
        Serial.println("[FAIL] : Error writing file");
        myFile.close();
      }
    break;

    case 2:
      if (myFile.println(content.c_str())){ 
        Serial.println("[SUCCESS] : File wrote correctly"); 
        myFile.close();
      } else{
        Serial.println("[FAIL] : Error writing file");
        myFile.close();
      }
    break;

    case 3:
      counter = atoi(content.c_str());
      if (myFile.print(counter)){ 
        Serial.println("[SUCCESS] : File wrote correctly"); 
        myFile.close();
      } else{ 
        Serial.println("[FAIL] : Error writing file");
        myFile.close();
      }
    break;
  }
  myFile.close();
}

void CreateDirectory(String DirName){
  if(SD.mkdir(DirName.c_str())){
    Serial.println("[SUCCESS] : Directory was created");
  }else{
    Serial.println("[FAIL] : directory wasn´t created"); 
  }
}

void readFileSD(String FileName){

  myFile = SD.open(FileName.c_str(), "r");
  if (!myFile){
    Serial.println("[FAIL] : Error opening file");
  }
  Serial.print("[SUCCESS] : File content: ");
  while(myFile.available()){
    Serial.write(myFile.read());
  }
  Serial.print("\n");
  myFile.close();
}

void removeFileSD(String FileName){

  if (SD.remove(FileName.c_str())){
    Serial.println("[SUCCESS] : File removed");
  } else{
    Serial.println("[FAIL] : File couldn't be removed");
  }
}

void writeCSV(String content){
  const size_t size = strlen(content.c_str()) * sizeof(int);
  myFile = SD.open("/Packages.csv", "a");
  DynamicJsonDocument doc(size);
  deserializeJson(doc, content);
  char buffer[256];
  
  JsonArray sensors = doc["s"];
  for(JsonArray array : sensors){
    for (JsonObject objects : array){
      const char *id = objects["i"];
      const char *data = objects["d"]; 
      Serial.println(id);
      Serial.println(data);
      Serial.println("1");
      sprintf(buffer, "%s%c%s%c", id, ',', data, ';');
      Serial.println("2");
      myFile.println(buffer);
      Serial.println("3");
    }
  }
  Serial.println("4");
  myFile.close();
}

// INDIVIDUAL PARA PRUEBAS ///

String readMemoryId(){
  File file = SPIFFS.open("/_id");
  return file.readString();
}

String readMemoryEnvironment(){
  File file = SPIFFS.open("/environment.json");
  return file.readString();
}

String readMemoryConfig(){
  File file = SPIFFS.open("/config.json");
  return file.readString();
}

String readMemoryPeripherals(){
  File file = SPIFFS.open("/peripherals.json");
  return file.readString();
}


void cloneToSD(String fileName){
  
  String content = readMemoryId();  
  writeFileSD(fileName, content, "w");
  readFileSD(fileName);

}

void cloneAllToSD(){

  String content;
  char mode[] = "w";
  
  content = readMemoryId();
  writeFileSD(filesSPIFF[0], content, mode);

  content = readMemoryEnvironment();
  writeFileSD(filesSPIFF[1], content, mode);

  content = readMemoryConfig();
  writeFileSD(filesSPIFF[2], content, mode);

  content = readMemoryPeripherals();
  writeFileSD(filesSPIFF[3], content, mode);
}

void readAllCopied(){

  for (String name : filesSPIFF){
    readFileSD(name);
  }

}

////////////////////////////////




void setup() {
  String Package = "{\"s\":[[{\"i\":1,\"d\":25.3}]]}";
  Serial.begin(9600);
  Serial.print("Starting SD...");
  if (!SD.begin(5)) {
    Serial.println("[FAIL] : Not inicialized");
    return;
  }
  Serial.println("Success initialization");
  delay(5000);
  writeCSV(Package);
  Serial.println("0");
}

void loop() {
  myFile = SD.open("/counter.dat", "r");
  String character;
  int fileCount;
  while(myFile.available()){
    character = myFile.read();
  }
  
  fileCount = atoi(character.c_str()); 
  fileCount += 1;
  character = (String)(fileCount);
  myFile.close();
  writeFileSD( "/counter.dat" , character, "w");
  readFileSD( "/counter.dat");
  readFileSD("/Packages.csv");
  //removeFileSD("/counter.dat");

  writeFileSD("/Config/Config.json","{\"ds\":1200}", "a");
  readFileSD("/Config/Config.json");
  //removeFileSD("/Config/Config.json");
  delay(5000);
}


//{"s":[[{"i":1,"d":25.3},...]]}
//i,1,d,25.3;