#include <SPI.h>		// incluye libreria interfaz SPI
#include <SD.h>			// incluye libreria para tarjetas SD
#include <FS.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <TimeLib.h>
//Declaramos el pin que encendera
int pin_dos = 2;
int pin_15 = 15;

int scanTime = 10; // in seconds
BLEScan* pBLEScan;

#define SSpin 5		// Slave Select en pin digital 10

File archivo, copia;			// objeto archivo del tipo File
int FileCount,FileCountop,FileCountH,FileCountc;

void reloj()
{
  String fecha = String(day()) + "/" + dato(month()) + "/" + dato(year()) + "," + String(hour())+ ":" + dato(minute()) + ":" + dato(second()) + ",";
  archivo.print(fecha);
  copia.print(fecha);
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    reloj();
    String datos = "NOMBRE:" + String(advertisedDevice.getName()) + "," + "MAC ADDRESS:" + advertisedDevice.getAddress().toString() + "," + "RSSI: " + String(advertisedDevice.getRSSI());
    archivo.println(datos);
    copia.println(datos);
    //Encendemos el led
    digitalWrite(pin_15, HIGH);
    //Esperamos un segundo
    delay(100);
    //Apagamos el led
    digitalWrite(pin_15, LOW);
    //Esperamos un segundo
    delay(100);
  }
};

void setup() {
  Serial.begin(115200);
  setTime(12,0,0,23,10,2024);
  pinMode(pin_dos, OUTPUT);
  pinMode(pin_15, OUTPUT);
//-------------------------------------------------------------------------------

  digitalWrite(pin_dos, HIGH);//Encendemos el led
  delay(5000);//Esperamos un segundo
  for(int j=0;j<=8;j++)
  {
  digitalWrite(pin_15, HIGH);    //Encendemos el led
  delay(100);  //Esperamos un segundo
  digitalWrite(pin_15, LOW);  //Apagamos el led
  delay(100);  //Esperamos un segundo
  }
  digitalWrite(pin_dos, LOW);  //Apagamos el led
  //Esperamos un segundo
  delay(1000);
  Serial.println("Inicializando tarjeta ...");	// texto en ventana de monitor
  if (!SD.begin(SSpin)) {			// inicializacion de tarjeta SD
    Serial.println("fallo en inicializacion !");// si falla se muestra texto correspondiente y
    return;					// se sale del setup() para finalizar el programa
  }
  String fileName = createUniqueFileName();
  String fileNamec = createUniqueFileNamec();
  archivo = SD.open(fileName, FILE_WRITE);	// apertura para lectura/escritura de archivo
  SD.mkdir("/Registros_crudos");
  copia = SD.open(fileNamec, FILE_WRITE);	// apertura para lectura/escritura de archivo
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

  Serial.println("inicializacion correcta");	// texto de inicializacion correcta
  
  delay(100); 
      
}//fin setup

void loop() {			// funcion loop() obligatoria de declarar pero no utilizada

    for(int k=0;k<=2;k++)
    {
      digitalWrite(pin_dos, HIGH);//Encendemos el led
      delay(1000);//Esperamos un segundo
      digitalWrite(pin_dos, LOW);//Apagamos el led
      delay(1000);//Esperamos un segundo
    }

    if(archivo)
    {
    Serial.println("Scanning...");
    BLEScanResults *foundDevices = pBLEScan->start(scanTime, false);
    archivo.close();
    Serial.print("Devices found: ");
    Serial.println(foundDevices->getCount());
    Serial.println("Scan done!");
    Serial.println("Guardado correctamente");	// texto de escritura correcta en monitor serie
    copia.close();

    for(int l=0;l<=8;l++)
    {
      digitalWrite(pin_dos, HIGH);//Encendemos el led
      delay(100);//Esperamos un segundo
      digitalWrite(pin_dos, LOW);//Apagamos el led
      delay(100);//Esperamos un segundo
    }
    String fileNameop = openUniqueFileName();
    archivo = SD.open(fileNameop);		// apertura de archivo prueba.txt
    Serial.println(fileNameop);
    if (archivo) {
    Serial.println("Contenido del archivo:");	// texto en monitor serie
    while (archivo.available()) {		// mientras exista contenido en el archivo
      Serial.write(archivo.read());  		// lectura de a un caracter por vez
    }	
    delay(10000);
    while(FileCountop == 12)//por cada 12 reportes de 5 minutos se crea uno solo de una hora
    {
      if(SD.mkdir("/Registros_hora"))
      {
        String fileNameH = createUniqueFileNameHora();
      File archivoH = SD.open(fileNameH,FILE_WRITE);
      if(archivoH){
        const char* archivos_unir[] = {"/reporte_1.txt", "/reporte_2.txt", "/reporte_3.txt", "/reporte_4.txt", "/reporte_5.txt", "/reporte_6.txt", "/reporte_7.txt", "/reporte_8.txt", "/reporte_9.txt", "/reporte_10.txt", "/reporte_11.txt", "/reporte_12.txt"};
        const int numero_archivos = sizeof(archivos_unir) / sizeof(archivos_unir[0]); 
        for(int i=0;i<numero_archivos;i++)
        {
          File archivo = SD.open(archivos_unir[i]);
          if(archivo)
          {
            Serial.print("Uniendo ");
            Serial.println(archivos_unir[i]);//abre los archivos para leer el contenido y unirlos en uno solo
            while(archivo.available())
            {
              archivoH.write(archivo.read());
            }
            archivo.close();
          }//fin if archivo lecturas
          else
            {
              Serial.print("Error al abrir ");
              Serial.println(archivos_unir[i]);
            }
        }//fin for
      archivoH.close();//cierra el archivo por hora
      Serial.println("Archivo de hora creado");
      }//fin if archivoH
      else
      {
        Serial.println("Error al crear archivo por hora");
      }
      }//fin if mkdir
      FileCount = 0;
      FileCountop = 0;
    }//fin while cuenta de los numeros de archivo
    Serial.println("Inicializando tarjeta ...");	// texto en ventana de monitor
  if (!SD.begin(SSpin)) {			// inicializacion de tarjeta SD
    Serial.println("fallo en inicializacion !");// si falla se muestra texto correspondiente y
    return;					// se sale del setup() para finalizar el programa
  }
  String fileName = createUniqueFileName();
  String fileNamec = createUniqueFileNamec();
  archivo = SD.open(fileName, FILE_WRITE);	// apertura para lectura/escritura de archivo
  SD.mkdir("/Registros_crudos");
  copia = SD.open(fileNamec, FILE_WRITE);	// apertura para lectura/escritura de archivo
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
  Serial.println("inicializacion correcta");	// texto de inicializacion correcta
  } //FIN DE IF SI ABRE EL ARCHIVO REINICIA EL ESCANER
    else {
      Serial.println("error en apertura de" + fileNameop + ".txt");	// texto de falla en apertura de archivo
      Serial.println("Reiniciando tarjeta ...");	// texto en ventana de monitor
    }//FIN IF ARCHIVO NO SE ABRE
  }//FIN IF ARCHIVO
  else
  {
    if (!SD.begin(SSpin)) {			// inicializacion de tarjeta SD
            Serial.println("fallo en reinicio!");// si falla se muestra texto correspondiente y
            return;					// se sale del setup() para finalizar el programa
        }
        String fileName = createUniqueFileName();
        String fileNamec = createUniqueFileNamec();
        archivo = SD.open(fileName, FILE_WRITE);	// apertura para lectura/escritura de archivo
        SD.mkdir("/Registros_crudos");
        copia = SD.open(fileNamec, FILE_WRITE);	// apertura para lectura/escritura de archivo
        BLEDevice::init("");
        pBLEScan = BLEDevice::getScan(); //create new scan
        pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
        pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
        pBLEScan->setInterval(100);
        pBLEScan->setWindow(99);  // less or equal setInterval value
        Serial.println("inicializacion correcta");	// texto de inicializacion correcta
        Serial.println("Volviendo al escaneo...");
  }//si no encuentra ningun archivo creado abre uno
    
}//FIN LOOP

String createUniqueFileName() {
  FileCount++; // Incrementa el contador
  String fileName = "/reporte_"; // Prefijo del nombre del archivo
  fileName += FileCount; // Agrega el número del contador
  fileName += ".txt"; // Sufijo del archivo
  return fileName;
}

String openUniqueFileName() {
  FileCountop++; // Incrementa el contador
  String fileNameop = "/reporte_"; // Prefijo del nombre del archivo
  fileNameop += FileCountop; // Agrega el número del contador
  fileNameop += ".txt"; // Sufijo del archivo
  return fileNameop;
}

String createUniqueFileNameHora() {
  FileCountH++; // Incrementa el contador
  String fileNameH = "/Registros_hora/reporte_Hora_"; // Prefijo del nombre del archivo
  fileNameH += FileCountH; // Agrega el número del contador
  fileNameH += ".csv"; // Sufijo del archivo
  return fileNameH;
}

String dato(int digito)
{
  String dt=String("0")+digito;
  return dt.substring(dt.length()-2);
}

String createUniqueFileNamec() {
  FileCountc++; // Incrementa el contador
  String fileNamec = "/Registros_crudos/reporte_respaldo_"; // Prefijo del nombre del archivo
  fileNamec += FileCountc; // Agrega el número del contador
  fileNamec += ".txt"; // Sufijo del archivo
  return fileNamec;
}
