#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2,3);

int izqA = 6;            // define el pin 6 para motor izquierdo hacia adelante
int izqB = 7;            // define el pin 7 para motor izquierdo hacia atras 
int derA = 8;            // define el pin 8 para motor izquierdo hacia adelante 
int derB = 9;            // define el pin 9 para motor izquierdo hacia atras 
String estadoAUX = "115"; // inicia detenido

int pecho = 4;            // define el pin 4 como (pecho) para el Ultrasonido
int ptrig = 5;            // define el pin 5 como (ptrig) para el Ultrasonido
int duracion, distancia;  // para Calcular distacia

//Definimos seguidores de linea
int ls = 10; //Sensor Izquierdo
int rs = 11; //Sensor Derecho
int leftValue; //Definimos los valores para la izquierda
int rightValue; //Definimos los valores para la derecha

void setup() {
  Serial.begin(9600);    // inicia el puerto serial para comunicacion con el Bluetooth
  BTSerial.begin(9600);
  
  pinMode(derA, OUTPUT);
  pinMode(derB, OUTPUT);
  pinMode(izqA, OUTPUT);
  pinMode(izqB, OUTPUT);
  
  pinMode(pecho, INPUT);   // define el pin 4 como entrada (pecho) 
  pinMode(ptrig,OUTPUT);   // define el pin 5 como salida  (ptrig) 
  pinMode(13,OUTPUT);

  pinMode(ls, INPUT);     // define el pin 10 como entrada (ls)
  pinMode(rs, INPUT);     // define el pin 11 como entrada (rs)
}

void loop() {
  int velociadad = 255;            // Velocidad de los motores (0-255)
  if(BTSerial.available()){        // lee el bluetooth y almacena en estado
      estadoAUX =BTSerial.read();
  }
  if(estadoAUX == "102"){           // Boton desplazar al Frente
     Adelante(velociadad);
  }
  if(estadoAUX == "108"){          // Boton IZQ 
     Izq(velociadad);
  }
  if(estadoAUX == "115"){         // Boton Parar
    Parar();
  }
  if(estadoAUX == "114"){          // Boton DER
    Der(velociadad);
  } 

  if(estadoAUX == "98"){          // Boton Reversa
    Atras(velociadad);
  }

  if(estadoAUX == "49"){          // Boton Seguidor de linea
    SeguidorLineaControlador();
  }
}

//Funcion para parar
void Parar() {
    analogWrite(derB, 0);     
    analogWrite(izqB, 0); 
    analogWrite(derA, 0);    
    analogWrite(izqA, 0); 
    Serial.println("Parar"); 
}

//Funcion para desplazar hacia adelante
void Adelante(int vel) {
    analogWrite(derB, 0);     
    analogWrite(izqB, 0); 
    analogWrite(derA, vel);  
    analogWrite(izqA, vel); 
    Serial.print("Adelante: ");
    Serial.println(vel);     
}
//Funcion para desplazar hacia Atras
void Atras(int vel) {
    analogWrite(derA, 0);    
    analogWrite(izqA, 0);
    analogWrite(derB, vel);  
    analogWrite(izqB, vel);
    Serial.print("ATRAS: "); 
    Serial.println(vel); 
}
//Funcion para girar hacia la Izquierda
void Izq(int vel) {
    analogWrite(derB, 0);     
    analogWrite(izqB, 0); 
    analogWrite(derA, 0);  
    analogWrite(izqA, vel); 
    Serial.print("IZQ: "); 
    Serial.println(vel);
}
//Funcion para girar hacia la Derecha
void Der(int vel) {
    analogWrite(derB, 0);     
    analogWrite(izqB, 0);
    analogWrite(izqA, 0);
    analogWrite(derA, vel);
    Serial.print("DER: "); 
    Serial.println(vel); 
}

//Función seguidor de linea
void SeguidorLinea(){
        leftValue = digitalRead(ls);
        rightValue = digitalRead(rs);
        delay(10);
        int velocidad = 130;            // Velocidad de los motores (0-130)
        if (rightValue == 0 && leftValue == 0) {
            Adelante(velocidad);
        }
        if (rightValue == 1 && leftValue == 0) {
           Izq(velocidad);
         }
         if (rightValue == 0 && leftValue == 1) {
          Der(velocidad);
         }
         if (rightValue == 1 && leftValue == 1) {
           Parar();
         }
  }

  
//Funcion para el seguidor de linea
void SeguidorLineaControlador(){
     digitalWrite(ptrig, HIGH);   // genera el pulso de trigger por 10us
     delay(0.01);
     digitalWrite(ptrig, LOW);
     duracion = pulseIn(pecho, HIGH);          // Lee el tiempo del Echo
     distancia = (duracion/2) / 29;            // calcula la distancia en centimetros
     delay(10); 
     Serial.println(distancia);
     if (distancia >=1 && distancia <= 15){    // si la distancia es menor de 15cm
        Parar();
        digitalWrite(13,HIGH);                 // Enciende buzzer
        delay(1000);
        digitalWrite(13,LOW);
     }else{
      SeguidorLinea();            //Función seguidor de linea
   }        
}
