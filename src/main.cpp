#include <Arduino.h>
#include <SD.h>

File myFile;

// ¿ /sd es el root ?

void writeFileSD( String name , String content){

  myFile = SD.open(name.c_str(), "w");
  if (!myFile){ 
    Serial.println("[FAIL] : File couldn't be created"); 
  } 
  if (myFile.print(content.c_str())){ 
    Serial.println("[SUCCESS] : File wrote correctly"); 
    myFile.close();
  } else{ 
    Serial.println("[FAIL] : Error writing file");
    myFile.close();
  }
}

void CreateDirectory(String DirName){
  if(SD.mkdir(DirName.c_str())){
    Serial.println("[SUCCESS] : Directory was created");
  }else{
    Serial.println("[FAIL] : directory wasn´t created"); 
  }
}

void readFileSD(String name ){

  myFile = SD.open(name.c_str(), "r");
  if (!myFile){
    Serial.println("[FAIL] : Error opening file");
  }
  Serial.println("[SUCCESS] : File content: ");
  while(myFile.available()){
    Serial.write(myFile.read());
  }
  myFile.close();
}

void removeFileSD(String name){

  if (SD.remove(name.c_str())){
    Serial.println("[SUCCESS] : File removed");
  } else{
    Serial.println("[FAIL] : File couldn't be removed");
  }
}


void setup() {
  Serial.begin(9600);
  Serial.print("Starting SD...");
  if (!SD.begin(5)) {
    Serial.println("[FAIL] : Not inicialized");
    return;
  }
  Serial.println("Success initialization");
}

void loop() {
  readFileSD("/Mati.txt");
  removeFileSD("/Mati.txt");
  delay(20000000);
}
