#include "FPS_GT511C3.h" //Get library from https://github.com/sparkfun/Fingerprint_Scanner-TTL 
#include "SoftwareSerial.h" //Software serial library 
#include <LiquidCrystal.h> //Library for LCD

FPS_GT511C3 fps(4, 5); //FPS connected to D4 and D5

const int rs = 6, en = 7, d4 = 8, d5 = 9, d6 = 10, d7 = 11; //Mention the pin number for LCD connection
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);//Initialize LCD method

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2); //Initialise 16*2 LCD
  lcd.print("GT511C3 FPS"); //Intro Message line 1
  lcd.setCursor(0, 1);
  lcd.print("with Arduino"); //Intro Message line 2
  delay(2000);
  lcd.clear();

  fps.Open();         //send serial command to initialize fps 
  fps.SetLED(true);   //turn on LED so fps can see fingerprint

  pinMode(2,INPUT_PULLUP); //Connect to internal pull up resistor as input pin 
}

void loop()
{ 
  if (digitalRead(2)==0)//If button pressed 
  {
  Enroll(); //Enroll a fingerprint 
  }

// Identify fingerprint test
  if (fps.IsPressFinger())
  {
    fps.CaptureFinger(false);
    int id = fps.Identify1_N();
      lcd.clear();
      lcd.print("Welcome:"); 
      if (id==200) lcd.print("Unkown  "); //If not recognised 
      lcd.print(id);
      delay(1000);
  }
  else
  {
    lcd.clear(); lcd.print("Hi!....."); //Display hi when ready to scan 
  }

}

void Enroll() //Enrol function from library exmaple program 
{
  int enrollid = 0;
  bool usedid = true;
  while (usedid == true)
  {
    usedid = fps.CheckEnrolled(enrollid);
    if (usedid==true) enrollid++;
  }
  fps.EnrollStart(enrollid);

  // enroll
  lcd.clear();
  lcd.print("Enroll #");
  lcd.print(enrollid);
  while(fps.IsPressFinger() == false) delay(100);
  bool bret = fps.CaptureFinger(true);
  int iret = 0;
  if (bret != false)
  {
    lcd.clear();
    lcd.print("Remove finger");
    fps.Enroll1(); 
    while(fps.IsPressFinger() == true) delay(100);
    lcd.clear(); lcd.print("Press again");
    while(fps.IsPressFinger() == false) delay(100);
    bret = fps.CaptureFinger(true);
    if (bret != false)
    {
      lcd.clear(); lcd.print("Remove finger");
      fps.Enroll2();
      while(fps.IsPressFinger() == true) delay(100);
      lcd.clear(); lcd.print("Press yet again");
      while(fps.IsPressFinger() == false) delay(100);
      bret = fps.CaptureFinger(true);
      if (bret != false)
      {
        lcd.clear(); lcd.print("Remove finger");
        iret = fps.Enroll3();
        if (iret == 0)
        {
          lcd.clear(); lcd.print("Enrolling Success");
        }
        else
        {
          lcd.clear();
          lcd.print("Enroll Failed:");
          lcd.print(iret);
        }
      }
      else lcd.print("Failed 1");
    }
    else lcd.print("Failed 2");
  }
  else lcd.print("Failed 3");
}