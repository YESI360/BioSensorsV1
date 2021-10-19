//
//CODIGO PARA EL SENSOR DE  RESPIRACION DE PECHO
//
// __   __   ______        ______     ______     __   __     ______     ______     ______
///\ \ / /  /\  == \      /\  ___\   /\  ___\   /\ "-.\ \   /\  ___\   /\  __ \   /\  == \      
//\ \ \'/   \ \  __<      \ \___  \  \ \  __\   \ \ \-.  \  \ \___  \  \ \ \/\ \  \ \  __<
// \ \__|    \ \_\ \_\     \/\_____\  \ \_____\  \ \_\\"\_\  \/\_____\  \ \_____\  \ \_\ \_\    
//  \/_/      \/_/ /_/      \/_____/   \/_____/   \/_/ \/_/   \/_____/   \/_____/   \/_/ /_/    V3
//

//
//
//CADENA DE SENSORES:
//(((SENSOR PULSO)))--->(((SENSOR PANZA)))--->(((SENSOR PECHO)))--->(((RECEPTOR))--->[[[unity]]]
//

#include <SoftwareSerial.h>
SoftwareSerial BTSerialM(10, 11); // RX | TX (cruzar en blutu) //MASTER
SoftwareSerial BTSerialS(2, 3); // RX | TX (cruzar en blutu) //SLAVE


const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int LED = 8;

int datoP = 0 ; // variables para los datos de los sensores
int datoL = 0 ;
int datoL2 = 0 ;

int indice = 0 ; //indice para el protocolito
int dataIN = 0 ; //variable para la data que ingresa por el blutu serie


void setup()
{
  //Serial.begin(9600); //monitor serie para DEBUGG
  BTSerialM.begin(9600);//incializamos modulo blutu MASTER
  BTSerialS.begin(9600);//incializamos modulo blutu SLAVE

  pinMode (LED, OUTPUT);//pin 8 como salida

  for ( int i = 0 ; i < 5 ; i++ ) { //el led parpadea 5 veces para saludar!
    digitalWrite(LED, 1 ) ;
    delay ( 20 ) ;
    digitalWrite (LED, 0 ) ;
  }

  digitalWrite (LED, LOW);//apago led por las dudas
}

void loop()
{
  sensorLanaPecho ();//funcion donde esta el sensado de la respiracion

  if (BTSerialS.available() > 0 ) {//si le esta llegando info al blutu SLAVE
    dataIN = BTSerialS.read() ;//guardo la data que le llega al blutu en la variable


    switch (indice) {// pregunto por el numero de la variable indice y segun cual sea voy al case
      case 0://arranca siempre aca
        if (dataIN == 255 ) {//si el encabezado del primer dato coincide
          indice ++;//avanzo el indice
        }
        else {//sino coincide
          indice  = 0 ;//vuelvo el indice a 0 y por ende voy al case 0
        }
        break;
      case 1:
        datoP = dataIN;//guardo el dato despues del encabezado que deberia ser el sensor de pulso
        indice++;//avanzo
        break;
      case 2 :
        if (dataIN == 254 ) {////si el encabezado del segundo dato coincide
          indice++;//avanzo el indice
        }
        else {
          indice = 0;//sino vuelvo al principio
        }
        break;
      case 3:
        datoL = dataIN; //guardo dato sensor lana panza
        indice++;//avanzo el indice
        break;
      case 4:
        if ( dataIN == 253 ) { //si me llega el ultimo dato de cierre y coincide envio todos los datos almacenados

          BTSerialM.write(255); //envio datos con sus encabezados y cierres
          BTSerialM.write(datoP);
          BTSerialM.write(254);
          BTSerialM.write(datoL);
          BTSerialM.write(253);
          BTSerialM.write(datoL2);
          BTSerialM.write(252);
          indice = 0 ;//reseteo el indice y vuelvo a leer desde el principio

          //          Serial.println(255);//imprimir en el monitor serie para DEBUGGEAR
          //          Serial.println(datoP);
          //          Serial.println(254);
          //          Serial.println(datoL);
          //          Serial.println(253);
          //          Serial.println(datoL2);
          //          Serial.println(252);

        }
        break;
    }

    delay(10);//cualquier cosa modificar un poco este delay
  }

}



void sensorLanaPecho () { //funcion donde esta el sensado

  sensorValue = analogRead(analogInPin);//lectura sensor
  outputValue = map(sensorValue, 0, 1023, 1, 10);//mapeo

  if (outputValue >= 3 )//si es mayor que el umbral
  {
    digitalWrite (LED, HIGH);//prendo led
    datoL2 = 2;//guardo dato
  }
  else if (outputValue < 3  )//si es menor que el umbral
  {
    digitalWrite (LED, LOW);//apago led
    datoL2 = 1;//guardo dato
  }
}
