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

/*
* Writes a CSV file from content
*/
void writeCSV(String content){
  //{"s":[[{"i":1,"d":25.3},...]]}
  String token;
  String id;
  String data;
  String toPrint;
  char __TOP_LIMIT__ = ':';
  char __BOTTOM_LIMIT__ = ',';
  char __BOTTOM_LIMIT_2__ = '}';
  unsigned int first;
  unsigned int last;
  
  myFile = SD.open("/Packages.txt", "a");
  content = content.substring(6);

  while(strchr(content.c_str(), __TOP_LIMIT__) != NULL){
    Serial.println(content);
    toPrint = "";
    //{"s":[[{"i":1,"d":25.3},{"i":2,"d":26.3}]]}
    first = content.indexOf(__TOP_LIMIT__);
    last  = content.indexOf(__BOTTOM_LIMIT__);
    token = content.substring(first+1,last);
    id = token;

    content = content.substring(last+1);
    first = content.indexOf(__TOP_LIMIT__);
    last  = content.indexOf(__BOTTOM_LIMIT_2__);
    token = content.substring(first+1,last);
    data = token;
    content = content.substring(last+2);

    toPrint = ("i,"+id+",d,"+data+';');
    myFile.println(toPrint);
  }
  myFile.close();
}

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

////////////////////////////////

/*
*  Updates SD datalog
*/
void dataLogUpdate(String fileName, DynamicJsonDocument doc){
  myFile = SD.open(fileName, "a");
  serializeJson(doc,myFile);
  myFile.println("");
  myFile.close();
}

void updateMemoryConfig(DynamicJsonDocument doc){
  File file = SPIFFS.open("/config.json", "w");
  serializeJson(doc, file);
  
  dataLogUpdate("/config.json", doc);
}

void updateMemoryPeripherals(DynamicJsonDocument doc){
  File file = SPIFFS.open("/peripherals.json", "w");
  serializeJson(doc, file);

  dataLogUpdate("/peripherals.json", doc);
}

void updateFailedPackages(DynamicJsonDocument doc){
  File file = SPIFFS.open("/failedPackages.json", "w");
  serializeJson(doc, file);

  dataLogUpdate("/failedPackages.json", doc);
}

////////////////////////////////


/*
/ Clones "/_id" to the SD memory
*/ 
void cloneToSD(String fileName = "/_id"){
  
  String content = readMemoryId();  
  writeFileSD(fileName, content, "w");
  readFileSD(fileName);

}

/*
* Clones all of the files in filesSPIFF to the SD memory
*/
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

/*
* Reads all of the files from filesSPIFF that are in the SD memory
*/
void readAllCopied(){

  for (String name : filesSPIFF){
    readFileSD(name);
  }

}

////////////////////////////////

void setup() {
  String Package = "{\"s\":[[{\"i\":1,\"d\":25.3},{\"i\":2,\"d\":22.3},{\"i\":3,\"d\":24}]]}";
  Serial.begin(9600);
  Serial.print("Starting SD...");
  if (!SD.begin(5)) {
    Serial.println("[FAIL] : Not inicialized");
    return;
  }
  Serial.println("Success initialization");
  delay(5000);
  
  removeFileSD("/Packages.txt");
  
  writeCSV(Package);
  //removeFileSD("/_id");
  
  /**
  removeFileSD("/counter.dat");
  removeFileSD("/Config/Config.json");
  */
  
  
}

void loop() {
  /**
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
  readFileSD( "/counter.dat");*/
  readFileSD("/Packages.txt");
  //removeFileSD("/counter.dat");
  /**
  writeFileSD("/Config/Config.json","{\"ds\":1200}", "a");
  readFileSD("/Config/Config.json");
  */
  /**
  cloneToSD();
  readFileSD("/_id");
  */
  //removeFileSD("/Config/Config.json");
  delay(5000);
}


//{"s":[[{"i":1,"d":25.3},...]]}
//i,1,d,25.3;