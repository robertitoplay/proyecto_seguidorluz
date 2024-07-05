#include <Servo.h> // libreria control de Servos
#include "DHT.h"   // Libreria sensor DHT
// Configura sensor DHT
#define DHTPIN 2        // Pin donde está conectado el sensor (Pin Digital 2)
#define DHTTYPE DHT11   // Se asigna sensor tipo DHT11
DHT dht(DHTPIN, DHTTYPE); 

Servo servohori; //SERVO HORIZONTAL (ABAJO)
int servoh = 0; //assign servo at 0 degree
int servohLimitHigh = 180; //Maximo rango del servo 180 grados
int servohLimitLow = 10;   //Minimo rango del servo 10 grados

Servo servoverti; //SERVO VERTICAL
int servov = 0;
int servovLimitHigh = 180;
int servovLimitLow = 10;

int ldrtopr = 1; //LDR Arriba derecha - Pin A1
int ldrtopl = 2; //LDR Arriba izquierda - Pin A2

int ldrbotr = 0; // LDR Abajo derecha - Pin A0
int ldrbotl = 3; // LDR Arriba izquierda - Pin A3


const int switchCERRADO = 3; // Pin de switch abanico cerrado
const int switchABIERTO = 4; // Pin de switch abanico abierto
int switchCERRADOv = 0;
int switchABIERTOv = 0;
const int MotorCERRAR = 5; // Pin Arranque motor sentido horario
const int MotorABRIR = 6; // Pin Arranque motor sentido antihorario

void setup () //Inicializa configuracion
{
  dht.begin(); //Inicializa DHT
  pinMode(switchCERRADO, INPUT); //Configura pines estado switch
  pinMode(switchABIERTO, INPUT);
  pinMode(MotorCERRAR, OUTPUT); //Configura pines mando motor
  pinMode(MotorABRIR, OUTPUT);
  
  servohori.attach(10); // Servo horizontal (base) al pin 10
  servohori.write(0);   // Inicializa estado inicial

  servoverti.attach(9); // Servo vertical (segundo nivel) al pin 9
  servoverti.write(0);  // Inicializa estado inicial
  delay(500);           // Espera medio segundo
}
// RUTINA PRINCIPAL
void loop()
{
  float h = dht.readHumidity(); //Lectura de humedad
  switchCERRADOv = digitalRead(switchCERRADO);
  switchABIERTOv = digitalRead(switchABIERTO);
 
    if (h < 80)  //Humedad menor a 80?
      { if(switchABIERTOv == HIGH){
          ControlPanel();
          }
         else{
          AbrirPanel();
         }       
      }      
     
      else 
      { if(switchCERRADOv == HIGH){
                 
          }
          else{
           CerrarPanel(); 
          }
             
       }
}
//ControlPanel();
//CerrarPanel();
//AbrirPanel();


// RUTINA PARA CONTROL DE INCLINACION LUZ PANEL
void ControlPanel()
{
  //Analis previo de estado
  servoh = servohori.read();
  servov = servoverti.read();
  
  int topl = analogRead(ldrtopl); // Lee valor LDR Arriba izquierda
  int topr = analogRead(ldrtopr); // Lee valor LDR Arriba derecha
  int botl = analogRead(ldrbotl); // Lee valor LDR Abajo izquierda
  int botr = analogRead(ldrbotr); // Lee valor LDR Arriba derecha

  int avgtop = (topl + topr) / 2; //Promedio de LDR arriba
  int avgbot = (botl + botr) / 2; //Promedio de LDR abajo
  int avgleft = (topl + botl) / 2; //Promedio de LDR izquierda
  int avgright = (topr + botr) / 2; //Promedio de LDR derecha
  
  if (avgtop < avgbot)             //1° condicion
    { servoverti.write(servov - 1);
        if (servov > servovLimitHigh)
          {servov = servovLimitHigh;
          }
      delay(8);
    }
  else if (avgbot < avgtop)       //2° condicion
    { servoverti.write(servov + 1);
        if (servov < servovLimitLow)
          {servov = servovLimitLow;
          }
      delay(8);
    }
    else
      {servoverti.write(servov);
      }

  if (avgleft > avgright)       //3° condicion
    { servohori.write(servoh - 1);
        if (servoh > servohLimitHigh)
          {servoh = servohLimitHigh;
          }
      delay(8);
    }
  
  else if (avgright > avgleft)  //4° condicion
    { servohori.write(servoh + 1);
         if (servoh < servohLimitLow)
            {servoh = servohLimitLow;
            }
        delay(8);
    }
    
  else
    {servohori.write(servoh); // Manda valor al servo
    }
  delay(50);
}
//RUTINA PARA CERRAR PANEL
void CerrarPanel(){  
  servohori.write(90); //Inicializa servo horizontal
  servoverti.write(90);  //Inicializa servo venrtical
   while (switchCERRADOv == LOW){ //Arranca motor sentido cerrar hasta que swich se active
          digitalWrite(MotorCERRAR, HIGH);
          switchCERRADOv = digitalRead(switchCERRADO); 
    }
   digitalWrite(MotorCERRAR, LOW); 
   loop();
  }

//RUTINA PARA ABRIR PANEL  
void AbrirPanel(){
  servohori.write(90); //Inicializa servo horizontal
  servoverti.write(90);  //Inicializa servo venrtical
    while (switchABIERTOv == LOW){ //Arranca motor sentido cerrar hasta que swich se active
        digitalWrite(MotorABRIR, HIGH);
        switchABIERTOv = digitalRead(switchABIERTO);  
      }
    digitalWrite(MotorABRIR, LOW);
    loop();
}
