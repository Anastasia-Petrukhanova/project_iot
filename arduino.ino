#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <NewPing.h>
#define PIN_TRIG 12
#define PIN_ECHO 11
#define MAX_DISTANCE 200

SoftwareSerial gsm(7, 8);
OneWire oneWire(13);
DallasTemperature s_t(&oneWire);
NewPing s_d(PIN_TRIG, PIN_ECHO, MAX_DISTANCE);

String str1 = "";
int val = 0;

void setup()
{
  Serial.begin(9600);
  
  gsm.begin(9600);

  s_t.begin(); 
  pinMode(9, OUTPUT);

  Serial.println("Initializing...");
  delay(1000);

  gsm.println("AT");
  updateSerial();

  gsm.println("AT+CPIN?");
  updateSerial();

  gsm.println("AT+CSQ");
  updateSerial();

  gsm.println("AT+CREG?");
  updateSerial();

  gsm.println("AT+CGATT?");
  updateSerial();

  gsm.println("AT+CIPMODE=0"); 
  updateSerial();

  gsm.println("AT+CIPMUX=0");
  updateSerial();

  gsm.println("AT+CIPSTATUS");
  updateSerial();

  gsm.println("AT+CSTT=\"internet\"");
  updateSerial();

  gsm.println("AT+CIPSTATUS");
  updateSerial();

  gsm.println("AT+CIICR");
  updateSerial();

  gsm.println("AT+CIPSTATUS");
  updateSerial();

  gsm.println("AT+CIFSR");
  updateSerial();

  gsm.println("AT+CIPSTATUS");
  updateSerial();

  gsm.println("AT+CIPSTART=\"TCP\",\"HOST\",\"PORT\""); // вместо HOST и PORT укажите свои значения
  updateSerial(); 
}

void loop()
{
  
  if (gsm.available())
  {
    char c = gsm.read();
    if ('\n' == c)
    {
      if (str1.indexOf("tmp") != -1)
      {
        s_t.requestTemperatures();
        float t = s_t.getTempCByIndex(0);
        gsm.println("AT+CIPSEND");
        updateSerial();
        gsm.print(t);
        updateSerial();
        gsm.write(26);
      }
      else 
      {
        if (str1.indexOf("dis") != -1)
        {
          unsigned int d = s_d.ping_cm();
          gsm.println("AT+CIPSEND");
          updateSerial();
          gsm.print(200 - d);
          updateSerial();
          gsm.write(26);
        }
        else if (str1.indexOf("val") != -1)
        {
          val = 0;
          for(int i=str1.indexOf("val")+3; i < str1.length(); ++i)
          {
            int a = str1[i];
            if (isDigit(a))
            {
              val = val*10 + (str1[i]-'0');
            }
          }
          analogWrite(9, val*8);
          gsm.println("AT+CIPSEND");
          updateSerial();
          gsm.print("Ok");
          updateSerial();
          gsm.write(26);
        }
      }
      str1 = ""; 
    }
    else if ('\n' != c) 
    {
      str1 += String(c);
    }
  }
}

void updateSerial() //для получения сообщений с gsm и общения с gsm через монитор порта
{
  delay(500);
  while (Serial.available()) 
  {
    gsm.write(Serial.read());
  }
  while(gsm.available()) 
  {
    Serial.write(gsm.read());
  }
}
