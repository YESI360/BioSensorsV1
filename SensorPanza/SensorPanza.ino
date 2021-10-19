//
//CODIGO PARA EL SENSOR DE RESPIRACION DE PANZA 
//
// __   __   ______        ______     ______     __   __     ______     ______     ______
///\ \ / /  /\  == \      /\  ___\   /\  ___\   /\ "-.\ \   /\  ___\   /\  __ \   /\  == \      
//\ \ \'/   \ \  __<      \ \___  \  \ \  __\   \ \ \-.  \  \ \___  \  \ \ \/\ \  \ \  __<
// \ \__|    \ \_\ \_\     \/\_____\  \ \_____\  \ \_\\"\_\  \/\_____\  \ \_____\  \ \_\ \_\    
//  \/_/      \/_/ /_/      \/_____/   \/_____/   \/_/ \/_/   \/_____/   \/_____/   \/_/ /_/    V3
//
//CADENA DE SENSORES:
//(((SENSOR PULSO)))--->(((SENSOR PANZA)))--->(((SENSOR PECHO)))--->(((RECEPTOR))--->[[[unity]]]
//


#include <SoftwareSerial.h> //incluye libreria para crear puertos serie por software 

SoftwareSerial BTSerialM(2, 3); // RX | TX (cruzar en blutu) // MODULO MASTER
SoftwareSerial BTSerialS(10, 11); // RX | TX (cruzar en blutu)// MODULO SLAVE


const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int LED = 8; //pin LED
int datoP = 0 ; //variable dato Pulso
int datoL = 0 ; //variable dato Lana 1 (respiracion panza)

void setup()
{
  //Serial.begin(9600); //inicializamos monitor serie solo para DEBUGGEAR
  BTSerialM.begin(9600); //inicializamos el modulo blutu maestro
  BTSerialS.begin(9600); //inicializamos el modulo blutu esclavo

  pinMode (LED, OUTPUT);// pin LED como SALIDA

  for ( int i = 0 ; i < 5 ; i++ ) { //el led parpadea 5 veces para saludar!
    digitalWrite(LED, 1 ) ;
    delay ( 20 ) ;
    digitalWrite (LED, 0 ) ;
  }

  digitalWrite (LED, LOW);//apago Led por las dudas
}

void loop()
{

  sensorLanaPanza (); // funcion para sensar la respiracion

  if (BTSerialS.available() > 0 ) {  //si hay data disponible en el modulo blutu
    datoP = BTSerialS.read() ;// leo la info que llega y la guardo en la variable datoP

    BTSerialM.write(255);//envio byte encabezado dato pulso
    BTSerialM.write(datoP);//envio dato pulso
    BTSerialM.write(254);//envio byte encabezado dato lana 1
    BTSerialM.write(datoL);//envio dato lana 1
    BTSerialM.write(253);//envio byte de cierre

    //    USAR EL MONITOR SERIE PARA DEBUGGEAR
    //    Serial.println(255);
    //    Serial.println(datoP);
    //    Serial.println(254);
    //    Serial.println(datoL);
    //    Serial.println(253);

    delay(20);//cualquier perdida de datos modificar un poco este delay

  }
}

void sensorLanaPanza () { //funcion donde esta el sensado

  sensorValue = analogRead(analogInPin);//lectura sensor
  outputValue = map(sensorValue, 0, 1023, 1, 10);//mapeo

  if (outputValue >= 3 )//si es mayor que el umbral
  {
    digitalWrite (LED, HIGH);//prendo led
    datoL = 2;//guardo dato
  }
  else if (outputValue < 3  )//si es menor que el umbral
  {
    digitalWrite (LED, LOW);//apago led
    datoL = 1;//guardo dato
  }

}
