#include <Arduino.h>
#include <SD.h>

File myFile;

void writeFileSD( char * name , char * content){

  myFile = SD.open(name, "w");
  if (!myFile){ 
    Serial.println("File couldn't be created"); 
  } 
  if (myFile.print("Writing in file test")){ 
    Serial.println("File wrote correctly"); 
    myFile.close();
  } else{ 
    Serial.println("Error writing file");
    myFile.close();
  }
}

void readFileSD( char * name ){

  myFile = SD.open(name, "r");
  if (!myFile){
    Serial.println("Error opening file");
  }
  Serial.println("File content: ");
  while(myFile.available()){
    Serial.write(myFile.read());
  }
  myFile.close();
}

void removeFileSD( char * name){

  if (SD.remove(name)){
    Serial.println("File removed");
  } else{
    Serial.println("File couldn't be removed");
  }
}


void setup() {
  Serial.begin(9600);
  Serial.print("Iniciando SD ...");
  if (!SD.begin(5)) {
    Serial.println("No se pudo inicializar");
    return;
  }
  Serial.println("inicializacion exitosa");
  writeFileSD("/Mati.txt", "This is a test");
  readFileSD("/Mati.txt");
}

void loop() {

}