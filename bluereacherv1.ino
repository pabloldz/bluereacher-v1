#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <SPI.h>		// incluye libreria interfaz SPI
#include <SD.h>			// incluye libreria para tarjetas SD

#define SSpin 5		// Slave Select en pin digital 10

File archivo;			// objeto archivo del tipo File

int scanTime = 30; // in seconds
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    archivo.println(advertisedDevice.toString().c_str());
  } 
};

void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando tarjeta ...");	// texto en ventana de monitor
  if (!SD.begin(SSpin)) {			// inicializacion de tarjeta SD
    Serial.println("fallo en inicializacion !");// si falla se muestra texto correspondiente y
    return;					// se sale del setup() para finalizar el programa
  }//fin if
  Serial.println("inicializacion correcta");	// texto de inicializacion correcta
  archivo = SD.open("/prueba.txt", FILE_WRITE);	// apertura para lectura/escritura de archivo prueba.txt
  Serial.println("Scanning...");
 if (archivo) {
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); //create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);  // less or equal setInterval value
 }
  else {
    Serial.println("error en apertura de prueba.txt");	// texto de falla en apertura de archivo
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults *foundDevices = pBLEScan->start(scanTime, false);
  archivo.close();
  Serial.print("Dispositivos encontrados: ");
  Serial.println(foundDevices->getCount());	
  Serial.println("Escaneo terminado!!!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}
