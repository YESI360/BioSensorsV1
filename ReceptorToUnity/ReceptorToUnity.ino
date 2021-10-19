//
//CODIGO PARA EL RECEPTOR
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
//ORDEN DE RECEPCION Y ENVIO DE DATOS :
//    DATO1,         DATO2,       DATO3
// SENSOR PULSO, SENSOR PANZA, SENSOR PECHO
// SENSOR PANZA y SENSOR PECHO (manda 1 cuando esta relajado, manda 2 cuando esta estirado)
// SENSOR PULSO (manda 1 cuando esta en reposo y 2 cuando hay un pulso)

#include <SoftwareSerial.h>
SoftwareSerial BTSerialS(2, 3); // RX | TX (cruzar en el blutu) SLAVE
int datoL = 0; // Variable para el dato del sensor LANA PANZA
int datoP = 0; //  Variable para el dato del sensor PULSO
int datoL2 = 0 ; //  Variable para el dato del sensor LANA PECHO
int contador = 0;
int indice = 0;
int dataIN = 0 ;

void setup()
{
  Serial.begin(9600);// inicializamos monitor serie a 9600
  BTSerialS.begin(9600); // inicializamos modulo blutu a 9600
}

void loop()
{


  if (BTSerialS.available() > 0) //si hay info el Blutu 1
  {
    dataIN = BTSerialS.read(); // guardamos la data que llega del sensor

    switch (indice) { // vamos chequeando el numero de indice
      case 0:
        if (dataIN == 255 ) { //si el encabezado coincide avanzamos
          indice ++;
        }
        else {
          indice  = 0 ;//sino volvemos a preguntar
        }
        break;
      case 1:
        datoP = dataIN;//guardamos el primer dato y avanzamos
        indice++;
        break;
      case 2 :
        if (dataIN == 254 ) { //si el segundo encabezado coincide avanzamos
          indice++;
        }
        else {
          indice = 0;//sino empezamos de 0
        }
        break;
      case 3:
        datoL = dataIN;//guardamos el segundo dato y avanzamos
        indice++;
        break;
      case 4:
        if ( dataIN == 253 ) {//si coincide el tercer encabezado avanzamos
          indice++;
        } else {
          indice = 0;//sino volvemos de 0
        }
        break;
      case 5:
        datoL2 = dataIN;//guardamos el tercer dato  y avanzamos
        indice++;
        break;

      case 6:
        if (dataIN == 252 ) {//si coincide el cierre imprimimos los datos para unity
          Serial.flush(); //flush para limpiar
          Serial.print(datoP);//imprimo dato 1 pulso
          Serial.print(",");//separo por coma, para poder splitear y meter en el array en unity
          Serial.print(datoL);//imprimo dato 2 panza
          Serial.print(",");//separo por coma, para poder splitear y meter en el array en unity
          Serial.print(datoL2);//imprimo dato 3 pecho
          Serial.println();//hago un salto de linea.
          indice = 0 ;//resteo el indice y volvemos a leer desde arriba
        }
        break;
    }
    ////como enviar a unity solo si hay un high ...
    //    if ( datoP or datoL or datoL2  == 2 ) {
    //      Serial.flush(); //flush para limpiar
    //      Serial.print(datoP);//imprimo dato 1 pulso
    //      Serial.print(",");//separo por coma, para poder splitear y meter en el array en unity
    //      Serial.print(datoL);//imprimo dato 2 panza
    //      Serial.print(",");//separo por coma, para poder splitear y meter en el array en unity
    //      Serial.print(datoL2);//imprimo dato 3 pecho
    //      Serial.println();//hago un salto de linea.
    //    }
  }
  delay(1);//delay general para el programa.
}





}
