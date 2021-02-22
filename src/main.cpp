#include <Arduino.h>
#include <SD.h>

File myFile;
enum TypeOfFile {
  txt = 1,
  json,
  dat
};


// ¿ /sd es el root ? R: yes 

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

  myFile = SD.open(FileName.c_str(), mode);
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

void writeJSON(String content){
  
}


void setup() {
  String Package = "{\"s\":[[{\"i\":1,\"d\":25.3}]]}"
  Serial.begin(9600);
  Serial.print("Starting SD...");
  if (!SD.begin(5)) {
    Serial.println("[FAIL] : Not inicialized");
    return;
  }
  Serial.println("Success initialization");
  writeFileSD( "/counter.dat" , "0", "w");
}

void loop() {
  File MyFileCount = SD.open("/counter.dat", "r");
  String character;
  int fileCount;
  
  while(myFile.available()){
    character = myFile.read();
  }
  
  
  fileCount = atoi(character.c_str()); 
  
  fileCount += 1;
  character = (String)(fileCount);
  MyFileCount.close();
  writeFileSD( "/counter.dat" , character, "w");
  readFileSD( "/counter.dat");
  //removeFileSD("/counter.dat");

  writeFileSD("/Config/Config.json","{\"ds\":1200}", "a");
  readFileSD("/Config/Config.json");
  //removeFileSD("/Config/Config.json");
  delay(5000);
}


//{"s":[[{"i":1,"d":25.3},...]]}
i,1,d,25.3;