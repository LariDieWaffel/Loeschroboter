/*
Larissa Metzner und Jola Jacobsen
01/2023
Löschroboter
*/

#include <SPI.h>
#include <Wire.h>

//legt fest welcher Motor an welchem Pin angeschlossen ist
int leftF = 3; //links vorwärts
int leftB = 5; //links rückwärts
int rightB = 6; //rechts rückwärts
int rightF = 7; //rechts vorwärts

int anaST = 14; //der eine mit analogWrite
int anaND = 15; //der andere mit analogWrite

//legt fest welche Wassergeräte an welchem Pin angeschlossen sind
int pump = 13;
int trigger = 12;
int echo = 11;

//Variablen zur Berechnung des Wasserstandes
long duration = 0;
long distance = 0;
double percentage = 0;
double heightTank = 16; //Höhe Tank (cm)
int deviation = 2; // Höhe Sensor über maximalem Wasserspiegel

void setup() 
{
  //festlegen in welchem Modus die PINs sind
  pinMode (leftF, OUTPUT);
  pinMode (leftB, OUTPUT);
  pinMode (rightF, OUTPUT);
  pinMode (rightB, OUTPUT);
  
  pinMode (anaST, OUTPUT);
  pinMode (anaND, OUTPUT);

  pinMode (trigger, OUTPUT);
  pinMode (echo, INPUT);
  pinMode (pump, OUTPUT);

  //Kommunikationsgeschwindikeit in Bit/sek festlegen
  Serial.begin (9600);

  Serial.println ("Test: Setup");
}

void forward ()
{
  //Pumpe ausstellen
  digitalWrite (pump, LOW);

  //Motoransteuerung zum vorwärts Fahren
  digitalWrite (anaST, HIGH);
  digitalWrite (anaND, HIGH);
  digitalWrite (leftF, HIGH);
  digitalWrite (rightF, HIGH);
  digitalWrite (leftB, LOW);
  digitalWrite (rightB, LOW);
}

void stop ()
{
  //Pumpe ausstellen
  digitalWrite (pump, LOW);

  //Motoransteuerung zum rückwärts Fahren
  digitalWrite (anaST, LOW);
  digitalWrite (anaND, LOW);
  digitalWrite (leftF, LOW);
  digitalWrite (rightF, LOW);
  digitalWrite (leftB, LOW);
  digitalWrite (rightB, LOW);
}

void backwards ()
{
  //Pumpe ausstellen
  digitalWrite (pump, LOW);

  //Motoransteuerung zum rückwärts Fahren
  digitalWrite (anaST, HIGH);
  digitalWrite (anaND, HIGH);
  digitalWrite (leftF, LOW);
  digitalWrite (rightF, LOW);
  digitalWrite (leftB, HIGH);
  digitalWrite (rightB, HIGH);
}

void left ()
{
  //Pumpe ausstellen
  digitalWrite (pump, LOW);

  //Motoransteuerung zum nach links Fahren
  digitalWrite (anaST, HIGH);
  digitalWrite (anaND, HIGH);
  digitalWrite (leftF, LOW);
  digitalWrite (rightF, HIGH);
  digitalWrite (leftB, LOW);
  digitalWrite (rightB, HIGH);
}

void right ()
{
  //Pumpe ausstellen
  digitalWrite (pump, LOW);

  //Motoransteuerung zum nach rechts Fahren
  digitalWrite (anaST, HIGH);
  digitalWrite (anaND, HIGH);
  digitalWrite (leftF, HIGH);
  digitalWrite (rightF, LOW);
  digitalWrite (leftB, HIGH);
  digitalWrite (rightB, LOW);
}

void extinguish () 
{
  //alle Motoren stoppen
  stop ();

  //Pumpe anstellen
  digitalWrite (pump, HIGH);
  Serial.println ("Wasser läuft!");
}

void loop() 
{
  //Bedingungen zum Fahren, die in den Funktionen resultieren

  if (Serial.available () ) //wenn Daten gegeben sind
  {
    char command = Serial.read (); //legt char an mit dem, was das GUI sendet

    if (command == 'w' || 'VK_UP') //überprüft ob das GUI w oder Pfeil nach oben gesendet hat
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
      backwards();
      Serial.println ("Wir fahren rückwärts!");
    }

    else if (command== 'e')
    {
      stop();
      Serial.println ("Wir stoppen!");
    }

    else if (command == 'q')
    {
      extinguish();
      Serial.println ("Wir löschen!");
    }

    else //falls eine Taste genutzt wird, die keine Funktion hat
    {
      Serial.println ("Diese Eingabe hat keine Funktion!");
    }

  }
  
  //Stellt Messgerät für 10sek an um eine Messung durchzuführen
  digitalWrite (trigger, LOW);
  delay (5); 
  digitalWrite (trigger, HIGH);
  delay (10); 
  digitalWrite (trigger, LOW);
  
  //Berechnung des Wasserstandes
  duration = pulseIn (echo, HIGH);
  distance = (duration/2) * 0.03432; //Berechnet Entfernung vom Sensor zur Wasseroberfläche

  percentage = 100-(((distance-deviation)*100)/heightTank); //Rechnet die Entfernung um in eine Prozentzahl abhängig von der max. Füllung

  if (distance >= 500 || distance <= 0) //wenn der Tank komplett leer oder überfüllt ist
  {
    Serial.println("Kein Messwert vorhanden");
  }

  else
  {
    //serielle Ausgabe in Abhängigkeit von der berechnet Prozentzahl, um den Wasserstand bekannt zu geben

    if (percentage < 25) 
    {
      //Serial.println("Sehr geringer Wasserstand!");
      //Serial.println("Bitte Wasser auffüllen."); 
      Serial.println("m");    
    }

    else if (percentage < 50) 
    {
      //Serial.println("Wasserstand gering!");
      Serial.println("n");
    }

    else 
    {
      //Serial.println("Es ist ausreichend Wasser da.");
      Serial.println("b");
    }
  }

  Serial.println("Test: loop Ende");

}
