/*#include <WiFi.h>
#include <WebServer.h>*/
#include <HardwareSerial.h>
#include <Wire.h>
/*#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>*/


/*Incluimos primero las librerías instaladas Ubidots y sensor PMS*/
#include "UbidotsEsp32Mqtt.h"
#include "PMS.h"
/*#include "DHT.h" /lo incluire despues */

/*Definimos que el Pin de Datos del sensor estará conectado al pin 23 del ESP32*/
#define PMSPIN 23
#define LEDPIN 15           
/*Definimos que el tipo de sensor que estamos utilizando es un DHT11*/
//#define DHTPIN ValorPorDefinir
//#define DHTTYPE DHT11     // DHT 11
/*Se actualizan algunos parámetros del DHT11 con los puntos definidos anteriormente*/ 
//DHT dht(DHTPIN, DHTTYPE);

// Definición del sensor PMS 
PMS pms(Serial2);
PMS::DATA data;


/*Definimos primero el Token que nos brinda la plataforma Ubidots para hacer la conexión*/
const char *UBIDOTS_TOKEN = "BBFF-ZmsvPRMMCPg2QFkvb4gRh47K0pilIn";
/*Definimos SSID y PASSWORD de nuestra red WiFi*/
const char *WIFI_SSID = "NET_SANCHEZ Z";      
const char *WIFI_PASS = "JdSZ_1124856559";     
/*Definimos el nombre de nuestro dispositivo, el cual aparecerá en la plataforma Ubidots*/
const char *DEVICE_LABEL = "IOT_prototipo_PM25";
/*Definimos las variables que se medirán y que serán publicadas en la plataforma Ubidots*/
const char *VARIABLE_LABEL_1 = "pm_uno"; 
const char *VARIABLE_LABEL_2 = "pm_2_5";
const char *VARIABLE_LABEL_3 = "pm_diez"; 
/*Definimos la frecuencia de publicación de 5 segundos*/
const int PUBLISH_FREQUENCY = 5000; 
/*Definimos unas variables extra, que incluye la librería, timer con millis sirve para contar*/
unsigned long timer;
uint8_t analogPin = 34; 
/*Definimos que las variables de la librería UBIDOTS trabajarán con el Token*/
Ubidots ubidots(UBIDOTS_TOKEN);

String pm1[]={"1.2","0.8","1.5","0.6","2.1","1.8","1.3","0.9","1.6","1.1","0.7","2.3","1.9","1.4","1.0","1.7","1.2","0.8","2.0","1.6","1.1","0.7","2.5","2.1","1.5","1.0","1.8","1.3","0.9"};
String pm2[]={"5.3","4.6","6.2","3.8","7.9","25.3","27.6","28.6","27.2","26.4","25.8","25.1","7.1","5.7","4.1","6.9","5.5","4.0","8.2","6.1","4.8","3.2","9.8","8.0","5.9","3.9","7.2","5.3","4.5"};
String pm10[]={"8.1","7.9","9.3","6.5","12.4","10.2","8.8","7.4","9.8","8.1","6.2","13.7","11.4","9.5","7.1","10.6","8.6","6.8","12.9","9.9","8.2","5.9","15.2","13.2","9.8","6.9","11.7","8.9","7.7"};

int ciclo=0;
int tamano=30;
//Varaibles para recibir los datos del sensor
String pm_uno;
String pm_2_5;
String pm_diez;

/*La librería nos da una función de Subscripción para mensajes de regreso desde la plataforma Ubidots hacia nuestro circuito*/
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

/*Definimos la función Setup() para iniciar nuestro circuito*/
void setup()
{
  /*Iniciamos el terminal Serial a 115200(DHT11) NOTA: a 9600 para PMS serial para consola serial2 para manejar el pms*/
  Serial.begin(9600);
  Serial2.begin(9600);  
  delay(5000);  
  /*Imprimimos el siguiente texto para asegurar el encendido del circuito*/
  pms.passiveMode();

  

  /*Iniciamos nuestro sensor*/
  //dht.begin();
  /*Incluimos un retardo de 1 segundo*/
  delay(3000);
  Serial.println("Iniciando Pin Mode");
  //identificar que hace esto - configuraciones del LED
  pinMode(LEDPIN, OUTPUT);


  delay(1000);

  /*Ahora, se incluye las funciones de conexión de la Plataforma, como la conexión a internet con las credenciales de WiFi*/
  // ubidots.setDebug(true);  //Descomentar esto para que los mensajes de depuración estén disponibles
  Serial.println("Conectando WIFI");
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  /*Se inicia la función de subscripción para los mensajes de retorno, que se definió líneas arriba*/
  ubidots.setCallback(callback);
  /*Se incluye las funciones de Setup y Reconnect, predefinidas de la librería*/
  ubidots.setup();
  ubidots.reconnect();
  timer = millis();
}

void loop()
{
  Serial.println("Iniciando VOID LOOP");
  /*Definimos que, si no se conecta a la plataforma Ubidots, se pasa la función Reconnect() para volver a establecer la conexión*/
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
  // Iniciamos la medición de la data
  Serial.println("Waking up, wait 5 seconds for stable readings...");
  pms.wakeUp();
  delay(5000);
  Serial1.println("Send read request...");
  pms.requestRead();

  Serial1.println("Reading data...");
  if (pms.read(data))
  {
    Serial.println("Dentro");
    pm_uno = data.PM_AE_UG_1_0;
    pm_2_5 = data.PM_AE_UG_2_5;
    pm_diez = data.PM_AE_UG_10_0;

    /*Definimos que el valor de particulas PM1.0, será enviado por la variable 1, a la plataforma Ubidots*/
    ubidots.add(VARIABLE_LABEL_1, pm_uno.toFloat());
    /*Definimos que el valor de particulas PM2.5, será enviado por la variable 2, a la plataforma Ubidots*/
    ubidots.add(VARIABLE_LABEL_2, pm_2_5.toFloat());
    /*Definimos que el valor de particulas PM10, será enviado por la variable 3, a la plataforma Ubidots*/
    ubidots.add(VARIABLE_LABEL_3, pm_diez.toFloat());
    /*Hacemos la publicación de los datos en el dispositivo definido*/
    ubidots.publish(DEVICE_LABEL);
    
    //También imprimimos resusltados en consola
    Serial.println("Air Quality Monitor"); 
    Serial.println("PM1.0 :" + pm_uno + "(ug/m3)"); 
    Serial.println("PM2.5 :" + pm_2_5 + "(ug/m3)"); 
    Serial.println("PM10  :" + pm_diez + "(ug/m3)");
    Serial.println("-------------------------------------");     
    timer = millis();
  } else {    
    Serial.println("No data");
    // Si no conecta el sensor, simulamos los datos
    
      pm_uno = pm1[ciclo];
      pm_2_5 = pm2[ciclo];
      pm_diez = pm10[ciclo];
      if(ciclo<30){
        ciclo = ciclo+1;  
      } else {
        ciclo = 0;
      }
      

      /*Definimos que el valor de particulas PM1.0, será enviado por la variable 1, a la plataforma Ubidots*/
        ubidots.add(VARIABLE_LABEL_1, pm_uno.toFloat());
        /*Definimos que el valor de particulas PM2.5, será enviado por la variable 2, a la plataforma Ubidots*/
        ubidots.add(VARIABLE_LABEL_2, pm_2_5.toFloat());
        /*Definimos que el valor de particulas PM10, será enviado por la variable 3, a la plataforma Ubidots*/
        ubidots.add(VARIABLE_LABEL_3, pm_diez.toFloat());
        /*Hacemos la publicación de los datos en el dispositivo definido*/
        ubidots.publish(DEVICE_LABEL);
        
        //También imprimimos resusltados en consola
        Serial.println("Air Quality Monitor"); 
        Serial.println("PM1.0 :" + pm_uno + "(ug/m3)"); 
        Serial.println("PM2.5 :" + pm_2_5 + "(ug/m3)"); 
        Serial.println("PM10  :" + pm_diez + "(ug/m3)");
        Serial.println("-------------------------------------");     
        timer = millis();  
  }
  if (pm_2_5.toFloat() >= 25)
  {    
    digitalWrite(LEDPIN, HIGH);
    delay(10000);
  }
  else
  {    
    digitalWrite(LEDPIN, LOW);
    delay(3000);
  }
   /*Agregamos un delay de 5 segundos para el envío, y que luego de este tiempo, se proceda a reiniciar el bucle*/
  
  ubidots.loop();


}
