#include <SPI.h>		// incluye libreria interfaz SPI
#include <SD.h>			// incluye libreria para tarjetas SD
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <ESP32Time.h>
#include <TimeLib.h>

ESP32Time rtc(0); 

//capturar la hora actual


int scanTime = 60; // in seconds
BLEScan* pBLEScan;

#define SSpin 5		// Slave Select en pin digital 10

File archivo;			// objeto archivo del tipo File

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String hora = String(hour()) + ":" + String(minute()) + ":" + String(second());
    archivo.print(hora);
    archivo.println(advertisedDevice.toString());
  }
};

void setup() {
  Serial.begin(115200);				// inicializa monitor serie a 9600 bps
  setTime(0, 0, 13, 29, 9, 2024);
  Serial.println("Inicializando tarjeta ...");	// texto en ventana de monitor
  if (!SD.begin(SSpin)) {			// inicializacion de tarjeta SD
    Serial.println("fallo en inicializacion !");// si falla se muestra texto correspondiente y
    return;					// se sale del setup() para finalizar el programa
  }
  archivo = SD.open("/prueba.txt", FILE_WRITE);	// apertura para lectura/escritura de archivo prueba.txt
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

  Serial.println("inicializacion correcta");	// texto de inicializacion correcta
  Serial.println("Scanning...");
  
}//fin setup



void loop() {			// funcion loop() obligatoria de declarar pero no utilizada
  // nada por aqui
    // put your main code here, to run repeatedly:
    if(archivo)
    {
    BLEScanResults *foundDevices = pBLEScan->start(scanTime, false);
    archivo.close();
    Serial.print("Devices found: ");
    Serial.println(foundDevices->getCount());
    Serial.println("Scan done!");
    Serial.println("Guardado correctamente");	// texto de escritura correcta en monitor serie

    archivo = SD.open("/prueba.txt");		// apertura de archivo prueba.txt
    if (archivo) {
    Serial.println("Contenido de prueba.txt:");	// texto en monitor serie
    while (archivo.available()) {		// mientras exista contenido en el archivo
      Serial.write(archivo.read());  		// lectura de a un caracter por vez
    }
    archivo.close();	
    delay(10000);
    
    setup();	
  } 
  if(!archivo) {
    Serial.println("error en apertura de prueba.txt");	// texto de falla en apertura de archivo
  }
    }

}
