# AirQEsp32PmS5003
Codigo para configurar el sensor PMS5003 en el chip Esp32 y enviar datos a la plataforma Ubidots

NOTA: En caso de que el sensor falle, o no sea leído este prototipo enviara datos predefinidos hacia la plataforma de Ubidots.

NOTA: Para programar el chip ESP32 o usarlo cuando se encuentra conectado vía USB se recomienda NO tener conectado el chip a otra fuente de alimentación.

NOTA: Antes de conectar el ESP32 al equipo revisa los puertos incluidos por defecto (Tools > Port), para este prototipo aparecía COM1 pero al conectar el chip aparecía COM4, el cual debía ser seleccionado para poder trabajar con el chip.

El prototipo contiene 3 elementos esenciales:
-	Sensor: encargado de recibir y transmitir los datos al bróker o servidor. 
-	Broker: encargado de recibir los datos desde el sensor y también de enviar los datos hacia un sistema de procesamiento y/o visualización de datos.
-	Sistema de visualización de datos: encargado de recibir los datos y presentarlos al usuario. 

Necesitarás:
Arduino IDE
Cable mini USB

Librerías necesarias:
o	PubSubclient by Nick O’Leary: Permite realizar suscripciones y publicaciones simples a un servidor que utiliza MQTT
o	PMS library by Mariusz Kakci - PMS.h: Biblioteca Arduino para sensores Plantower PMS
o	Esp32-mqtt-main - UbidotsEsp32Mqtt.h: permite el envío de datos hacia Ubidots a través de sockets
o	HardwareSerial.h: Librería fundamental de Arduino para ESP32
o	Wire.h.: Librería que maneja conexiones WIFI.

Pasos de instalación:
1.	Descargar y ejecutar el instalador de Arduino IDE desde https://www.arduino.cc/en/software en la opción DOWNLOAD OPTIONS
2.	En Arduino IDE desde el administrador del librerías buscar e instalar PubSubclient y PMS library. Las otras dos suelen venir por defecto.
3.	Descargar Esp32- mqtt-main desde https://github.com/ubidots/esp32-mqtt/archive/refs/heads/main.zip y seguir los pasos de la siguiente guía para cargar el archivo zip https://todomaker.com/blog/esp32-ubidots-primeros-pasos/  
4.	Ir a Tools > Board > esp32 > DOIT ESP32 DEVKIT V1 (el modelo puede variar según la placa adquirida) 
5.	Conectar el chips ESP32 mediante un cable mini usb a un puerto USB del computador.
6.	Ir a tools > port > COM# (# hace referencia al número de puerto que aparece al conectar el chip). Si no aparece ningún puerto buscar una librería para conectar esp32 via USB tipo C.
7.	Copie el código anexo a este documento en Arduino IDE
8.	Presione el botón upload 
9.	Cuando se cargue el contenido, puede probarlo mediante la consola en la opción y definiendo la misma frecuenta dada en el comando Serial (ver código) para este caso 9600 
10.	Después de configurar el chip puede ser desconectado para ensamblarlo en el prototipo.
