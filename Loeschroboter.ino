/*
Larissa Metzner und Jola Jacobsen
01/2023
Löschroboter
*/

#include <SPI.h>
#include <Wire.h>

//legt fest welcher Motor an welchem Pin angeschlossen ist
int motor1 = 8; //das soll der links vorne sein
int motor2 = 9; //das soll der rechts vorne sein
int motor3 = 10; //das soll der links hinten sein
int motor4 = 11; //das soll rechts hinten sein

//legt fest welche Wassergeräte an welchem Pin angeschlossen sind
int pump = 5;
int trigger = 7;
int echo = 6;

//Variablen zur Berechnung des Wasserstandes
long duration = 0;
long distance = 0;
double percentage = 0;

void setup() 
{
  //festlegen in welchem Modus die PINs sind
  pinMode (motor1, OUTPUT);
  pinMode (motor2, OUTPUT);
  pinMode (motor3, OUTPUT);
  pinMode (motor4, OUTPUT);

  pinMode (trigger, OUTPUT);
  pinMode (echo, INPUT);
  pinMode (pump, OUTPUT);

  //Kommunikationsgeschwindikeit in Bit/sek festlegen
  Serial.begin (9600);

  Serial.println ("Test: Setup");
}

void forward ()
{
  //Motoransteuerung zum vorwärts Fahren
  digitalWrite (motor1, HIGH);
  digitalWrite (motor2, HIGH);
  digitalWrite (motor3, HIGH);
  digitalWrite (motor4, HIGH);
}

void stop ()
{
  //Motoransteuerung zum Stoppen
  digitalWrite (motor1, LOW);
  digitalWrite (motor2, LOW);
  digitalWrite (motor3, LOW);
  digitalWrite (motor4, LOW);
}

void left ()
{
  //Motoransteuerung zum nach links Fahren
  digitalWrite (motor1, LOW);
  digitalWrite (motor2, HIGH);
  digitalWrite (motor3, LOW);
  digitalWrite (motor4, HIGH);
}

void right ()
{
  //Motoransteuerung zum nach rechts Fahren
  digitalWrite (motor1, HIGH);
  digitalWrite (motor2, LOW);
  digitalWrite (motor3, HIGH);
  digitalWrite (motor4, LOW);
}

void extinguish () 
{
  digitalWrite (pump, HIGH);
  Serial.println ("Wasser läuft!");
}

void loop() 
{
  //Bedingungen die in den Funktionen resultieren

  if (Serial.available () ) //wenn Daten gegeben sind
  {
    char command = Serial.read (); //legt char an mit dem, was die GUI sendet

    if (command == 'w' || 'VK_UP') //überprüft ob die GUI w oder Pfeil nach oben gesendet hat
    {
      forward (); //Func forward wird ausgeführt
      Serial.println ("Wir fahren vorwärts!");
    }
    
    else if (command == 'a' || 'VK_LEFT')
    {
      left();
      Serial.println ("Wir drehen nach links!");
    }

    else if (command == 'd' || 'VK_RIGHT')
    {
      right();
      Serial.println ("Wir drehen nach rechts!");
    }


    else if (command == 's' || 'VK_DOWN')
    {
      stop();
      Serial.println ("Wir stoppen!");
    }

    else if (command == 'q')
    {
      extinguish();
    }

    else
    {
      Serial.println ("Diese Eingabe hat keine Funktion!");
    }

  }

  digitalWrite (trigger, LOW);
  delay (5); 
  digitalWrite (trigger, HIGH);
  delay (10); 
  digitalWrite (trigger, LOW);

  duration = pulseIn (echo, HIGH);
  distance = (duration/2) * 0.03432; 

  double heightTank = 18.3; //Höhe Tank (cm)
  int deviation = 2; // Höhe Sensor über maximalem Wasserspiegel

  percentage = 100-(((distance-deviation)*100)/heightTank);

  if (distance >= 500 || distance <= 0) 
  {
    Serial.println("Kein Messwert vorhanden");
  }

  else
  {
    // Abstufungen 
    if (percentage < 25) 
    {
      Serial.println("Sehr geringer Wasserstand!");
      Serial.println("Bitte Wasser auffüllen.");     
    }

    else if (percentage < 50) 
    {
      Serial.println("Wasserstand gering.");
    }

    else 
    {
      Serial.println("Es ist ausreichend Wasser da.");
    }
  }

  Serial.println("Test: loop Ende");

}
