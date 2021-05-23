#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 21 
#define RST_PIN 22
#include "ESP32_MailClient.h"
#define REPEAT_CAL false
#include <TFT_eSPI.h> 
int numbers[4] = {-1,-1,-1,-1};
int i;
String str;
 uint16_t x, y;
TFT_eSPI LcdTouch = TFT_eSPI();
int W = 0;
int B;
const char* ssid = "shimon";
const char* password = "0578174093";
#define emailSenderAccount    "esp3210@gmail.com"    
#define emailSenderPassword   "esp4life"
#define emailRecipient        "orabergel12@gmail.com"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "ESP32 Test"
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp32.h>
char auth[] = "XVYepHFJvrXvTmbS5-WQdnI9n2ksncSI";
bool sent = false;
int val = 0;
int F; 
int WorkG = 33;
int z;
#define LED_G 25 //define green LED pin
#define RELAY 27 //relay pin
#define ACCESS_DELAY 4000
#define DENIED_DELAY 1000
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int J;
bool rfidon = true;

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);


void setup()
{
  Serial.begin(57600);
  Serial2.begin(115200);
  SPI.begin();          // Initiate  SPI bus
mfrc522.PCD_Init();   // Initiate MFRC522
finger.begin(57600);
z = 1;
pinMode(LED_G, OUTPUT);
pinMode(RELAY, OUTPUT);
pinMode (WorkG,INPUT);
pinMode(WorkG, OUTPUT);
x = 0;
B = 0;
digitalWrite(RELAY, HIGH);
Serial.println("Put your card to the reader...");
Serial.println();
 if (finger.verifyPassword()) 
 {
   Serial.println("Fingerprint Sensor Connected");
 }
 Blynk.begin(auth, ssid, password);
 Serial.print("Connecting");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(200);

    Serial.println();
  Serial.println("WiFi connected.");
  }
   i = 0;
 LcdTouch.init();
  LcdTouch.setRotation(0);
  //clibriat();
  uint16_t calData[5] = { 907, 2658, 627, 3098, 4};
  LcdTouch.setTouch(calData);
  LcdTouch.fillScreen(ILI9341_BLACK);
  LcdTouch.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  LcdTouch.drawRect(10, 10, 145, 56, ILI9341_BLUE); //first line
  LcdTouch.drawRect(160, 10, 70, 56, ILI9341_BLUE); //first line X
  LcdTouch.drawRect(10, 71, 70, 56, ILI9341_BLUE); //second line 1
  LcdTouch.drawRect(85, 71, 70, 56, ILI9341_BLUE); //second line 2
  LcdTouch.drawRect(160, 71, 70, 56, ILI9341_BLUE); //second line 3
  LcdTouch.drawRect(10, 132, 70, 56, ILI9341_BLUE); //third line 4
  LcdTouch.drawRect(85, 132, 70, 56, ILI9341_BLUE); //third line 5
  LcdTouch.drawRect(160, 132, 70, 56, ILI9341_BLUE); //third line 6
  LcdTouch.drawRect(10, 193, 70, 56, ILI9341_BLUE); //forth line 7
  LcdTouch.drawRect(85, 193, 70, 56, ILI9341_BLUE); //forth line 8
  LcdTouch.drawRect(160, 193, 70, 56, ILI9341_BLUE); //forth line 9
  LcdTouch.drawRect(10, 254, 70, 56, ILI9341_BLUE); //fifth line 0
  LcdTouch.drawRect(85, 254, 145, 56, ILI9341_BLUE); //fifth line "ENTER"
 
  LcdTouch.drawString ("1", 35, 85, 4);
  LcdTouch.drawString ("2", 110, 85, 4);
  LcdTouch.drawString ("3", 185, 85, 4);
  LcdTouch.drawString ("4", 35, 145, 4);
  LcdTouch.drawString ("5", 110, 145, 4);
  LcdTouch.drawString ("6", 185, 145, 4);
  LcdTouch.drawString ("7", 35, 205, 4);
  LcdTouch.drawString ("8", 110, 205, 4);
  LcdTouch.drawString ("9", 185, 205, 4);
  LcdTouch.drawString ("0",35, 265, 4);
  LcdTouch.drawString ("ENTER", 100, 267, 4);
  LcdTouch.drawString ("X", 185, 25, 4);
  
  //LcdTouch.print (27, 25, "1", 4, ILI9341_WHITE); //numbers location 
  //LcdTouch.print (57, 25, "4", 4, ILI9341_WHITE); //numbers location
  //LcdTouch.print (87, 25, "9", 4, WHITE); //numbers location
  //LcdTouch.print (117, 25, "6", 4, WHITE); //numbers location

}

void loop()
{
  if (z == 1)
  {
    Rfid();
  }
  if (z == 2)
  {
    Gmail();
    z = 3;
  }
  if ( z == 3)
  {
    TOUCH();
    if ( W == F)
    {
      z = 4;
      W = 0;
    }
  }
  if ( z == 4)
  {
    Fingerprint();
  }
}




int Rfid() 
{
  Serial.println("put the card"); 
 // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    Serial.println("no new card");
    return z ;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    Serial.println("no read card serial");
    return z ;
  }
 
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  content.toUpperCase();
  
  if (content.substring(1) == ( "53 44 B0 03"))
  {
   z = 2;
   return z;
  }
 
 else   {
     z = 1;  
    Serial.println(" Access denied");
    delay(DENIED_DELAY);
    

  }
  
}

int Gmail()
{
  val = digitalRead(WorkG);

  Blynk.run();  
  semail();
}

// Callback function to get the Email sending status
void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());

  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
}

void semail()
 {
 
Serial.println(i);

  F = random(1000,10000);
  String N = String (F);
  
  delay(10000);

 SMTPData smtpData;
// Callback function to get the Email sending status
void sendCallback(SendStatus info);


   Serial.println("Preparing to send email");
  Serial.println();
  
  // Set the SMTP Server Email host, port, account and password
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);

  // For library version 1.2.0 and later which STARTTLS protocol was supported,the STARTTLS will be 
  // enabled automatically when port 587 was used, or enable it manually using setSTARTTLS function.
  //smtpData.setSTARTTLS(true);

  // Set the sender name and Email
  smtpData.setSender("ESP32", emailSenderAccount);

  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");

  // Set the subject
  smtpData.setSubject(emailSubject);

  // Set the message with HTML format
  smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>LED!</h1><p>- YOUR CODE IS:</p></div>", true);
  // Set the email message in text format (raw)
  smtpData.setMessage(N, true);

  // Add recipients, you can add more than one recipient
  smtpData.addRecipient(emailRecipient);
  //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");

  smtpData.setSendCallback(sendCallback);

  //Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  //Clear all data from Email object to free memory
  smtpData.empty();
}


int Fingerprint()
{
int res = getFingerprintIDez();
delay(50);
return res;


}
 
// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() 
{
 uint8_t p = finger.getImage();
 if (p != FINGERPRINT_OK)  
{
 
   Serial.println("Waiting For Valid Finger");
   delay(1000);
   return -1;
 }
 
 p = finger.image2Tz();
 if (p != FINGERPRINT_OK)  
 {
   Serial.println("Messy Image Try Again");
   B++;
   if ( B > 2 )
   {
    B = 0;
    z = 1;
   }
   delay(3000);
   return -1;
 }
 
 p = finger.fingerFastSearch();
 if (p != FINGERPRINT_OK)  {
   
   
   Serial.println("Not Valid Finger");
   B++;
   if ( B > 2 )
   {
    B = 0;
    z = 1;
   }
   delay(3000);
   return -1;
   return z;
 }
 
 // found a match!
  Serial.println("working welcome");
  
  digitalWrite(RELAY, LOW);
    digitalWrite(LED_G, HIGH);
    delay(ACCESS_DELAY);
    digitalWrite(RELAY, HIGH);
    digitalWrite(LED_G, LOW);
    z  = 1;
    return 2;
  
}


int TOUCH()
{
  LcdTouch.getTouch (&x , &y );
   if (LcdTouch.getTouch(&x , &y))
  {
    //LcdTouch.print (27, 25, "1", 4, WHITE);
    //LcdTouch.readTouch();
   // x = LcdTouch.xTouch;
    //y = LcdTouch.yTouch;
    //Serial.print("x = ");
    //Serial.print(x);
    //Serial.print(", y = ");
    //Serial.println(y);
    //delay(1000);
    
     if ((x >= 160) && (x <= 230) && (y >= 10) && (y <= 66)) 
      {
        Serial.println("X");
                if (i>0)
        {
          //LcdTouch.drawRect(85, 254, 145, 56, DARKGREY); //fifth line "ENTER
         // LcdTouch.print (100, 267, "ENTER", 4, DARKGREY);
          switch(i)
          {
            case 1:
            LcdTouch.fillRect(26, 20, 25, 35, ILI9341_BLACK);
            break;
            case 2:
            LcdTouch.fillRect(56, 20, 25, 35, ILI9341_BLACK);
            break;
            case 3:
            LcdTouch.fillRect(86, 20, 25, 35, ILI9341_BLACK);
            break;
            case 4:
            LcdTouch.fillRect(116, 20, 25, 35,ILI9341_BLACK);
            break;
          }
          i--;
          numbers[i] = -1;
          if (i==0)
          {
           // LcdTouch.drawRect(160, 10, 70, 56, DARKGREY);
           // LcdTouch.print (185, 25, "X", 4, DARKGREY);
          }
        }
      }
      else if ((x >= 10) && (x <= 80) && (y >= 71) && (y <= 127))
      {
        Serial.println("1");
        LcdTouch.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        // LcdTouch.drawString ("1", 26, 25, 4);
         if (i<4)
        {
          numbers[i] = 1;
          i++;
          switch(i)
          {
            case 1:
            
            LcdTouch.drawString ("1", 26, 25, 4);
            break;
            case 2:
            LcdTouch.drawString ("1", 56, 25, 4);
            break;
            case 3:
            LcdTouch.drawString ("1", 86, 25, 4);
            break;
            case 4:
            LcdTouch.drawString ("1", 116, 25, 4);
           // LcdTouch.drawRect(85, 254, 145, 56, BLUE); //fifth line "ENTER"
            //LcdTouch.print (100, 267, "ENTER", 4, WHITE);
            break;
          }
         // LcdTouch.drawRect(160, 10, 70, 56, BLUE); //first line X
          //LcdTouch.print (185, 25, "X", 4, WHITE);
        }
        
      }
      else if ((x >= 85) && (x <= 155) && (y >= 71) && (y <= 127))
      {
        Serial.println("2");
         if (i<4)
        {
          numbers[i] = 2;
          i++;
          switch(i)
          {
            case 1:
            LcdTouch.drawString ("2", 26, 25, 4);
            break;
            case 2:
            LcdTouch.drawString ("2", 56, 25, 4);
            break;
            case 3:
            LcdTouch.drawString ("2", 86, 25, 4);
            break;
            case 4:
            LcdTouch.drawString ("2", 116, 25, 4);
       
            //LcdTouch.drawRect(85, 254, 145, 56, BLUE); //fifth line "ENTER"
           // LcdTouch.print (100, 267, "ENTER", 4, WHITE);
            break;
          }
         // LcdTouch.drawRect(160, 10, 70, 56, BLUE); //first line X
          //LcdTouch.print (185, 25, "X", 4, WHITE);
        }
      }
      else if ((x >= 160) && (x <= 230) && (y >= 71) && (y <= 127))
      {
        Serial.println("3");
        if (i<4)
        {
          numbers[i] = 3;
          i++;
          switch(i)
          {
            case 1:
            LcdTouch.drawString ("3", 26, 25, 4);
            break;
            case 2:
            LcdTouch.drawString ("3", 56, 25, 4);
            break;
            case 3:
            LcdTouch.drawString ("3", 86, 25, 4);
            break;
            case 4:
            LcdTouch.drawString ("3", 116, 25, 4);
           // LcdTouch.drawRect(85, 254, 145, 56, BLUE); //fifth line "ENTER"
            //LcdTouch.print (100, 267, "ENTER", 4, WHITE);
            break;
          }
          //LcdTouch.drawRect(160, 10, 70, 56, BLUE); //first line X
          //LcdTouch.print (185, 25, "X", 4, WHITE);
        }
      }
      else if ((x >= 10) && (x <= 80) && (y >= 132) && (y <= 188))
      {
        Serial.println("4");
         if (i<4)
        {
          numbers[i] = 4;
          i++;
          switch(i)
          {
            case 1:
            LcdTouch.drawString ("4", 26, 25, 4);
            break;
            case 2:
           LcdTouch.drawString ("4", 56, 25, 4);
            break;
            case 3:
           LcdTouch.drawString ("4", 86, 25, 4);
            break;
            case 4:
            LcdTouch.drawString ("4", 116, 25, 4);
           // LcdTouch.drawRect(85, 254, 145, 56, BLUE); //fifth line "ENTER"
           // LcdTouch.print (100, 267, "ENTER", 4, WHITE);
            break;
          }
         // LcdTouch.drawRect(160, 10, 70, 56, BLUE); //first line X
          //LcdTouch.print (185, 25, "X", 4, WHITE);
        }
      }
      else if ((x >= 85) && (x <= 155) && (y >= 132) && (y <= 188))
      {
        Serial.println("5");
        if (i<4)
        {
          numbers[i] = 5;
          i++;
          switch(i)
          {
            case 1:
             LcdTouch.drawString ("5", 26, 25, 4);
            break;
            case 2:
             LcdTouch.drawString ("5", 56, 25, 4);
            break;
            case 3:
            LcdTouch.drawString ("5", 86, 25, 4);
            break;
            case 4:
            LcdTouch.drawString ("5", 116, 25, 4);
            //LcdTouch.drawRect(85, 254, 145, 56, BLUE); //fifth line "ENTER"
            //LcdTouch.print (100, 267, "ENTER", 4, WHITE);
            break;
          }
          //LcdTouch.drawRect(160, 10, 70, 56, BLUE); //first line X
          //LcdTouch.print (185, 25, "X", 4, WHITE);
        }
      }
      else if ((x >= 160) && (x <= 230) && (y >= 132) && (y <= 188))
      {
        Serial.println("6");
        if (i<4)
        {
          numbers[i] = 6;
          i++;
          switch(i)
          {
            case 1:
            LcdTouch.drawString ("6", 26, 25, 4);
            break;
            case 2:
            LcdTouch.drawString ("6", 56, 25, 4);
            break;
            case 3:
            LcdTouch.drawString ("6", 86, 25, 4);
            break;
            case 4:
            LcdTouch.drawString ("6", 116, 25, 4);
           // LcdTouch.drawRect(85, 254, 145, 56, BLUE); //fifth line "ENTER"
           // LcdTouch.print (100, 267, "ENTER", 4, WHITE);
            break;
          }
          //LcdTouch.drawRect(160, 10, 70, 56, BLUE); //first line X
         // LcdTouch.print (185, 25, "X", 4, WHITE);
        }
      }
      else if ((x >= 10) && (x <= 80) && (y >= 193) && (y <= 249))
      {
        Serial.println("7");
        if (i<4)
        {
          numbers[i] = 7;
          i++;
          switch(i)
          {
            case 1:
            LcdTouch.drawString ("7", 26, 25, 4);
            break;
            case 2:
            LcdTouch.drawString ("7", 56, 25, 4);
            break;
            case 3:
            LcdTouch.drawString ("7", 86, 25, 4);
            break;
            case 4:
            LcdTouch.drawString ("7", 116, 25, 4);
            //LcdTouch.drawRect(85, 254, 145, 56, BLUE); //fifth line "ENTER"
            //LcdTouch.print (100, 267, "ENTER", 4, WHITE);
            break;
          }
         // LcdTouch.drawRect(160, 10, 70, 56, BLUE); //first line X
        //  LcdTouch.print (185, 25, "X", 4, WHITE);
        }
      }
      else if ((x >= 85) && (x <= 160) && (y >= 193) && (y <= 249))
      {
        Serial.println("8");
        if (i<4)
        {
          numbers[i] = 8;
          i++;
          switch(i)
          {
            case 1:
            LcdTouch.drawString ("8", 26, 25, 4);
            break;
            case 2:
            LcdTouch.drawString ("8", 56, 25, 4);
            break;
            case 3:
            LcdTouch.drawString ("8", 86, 25, 4);
            break;
            case 4:
            LcdTouch.drawString ("8", 116, 25, 4);
            //LcdTouch.drawRect(85, 254, 145, 56, BLUE); //fifth line "ENTER"
            //LcdTouch.print (100, 267, "ENTER", 4, WHITE);
            break;
          }
         // LcdTouch.drawRect(160, 10, 70, 56, BLUE); //first line X
          //LcdTouch.print (185, 25, "X", 4, WHITE);
        }
      }
      else if ((x >= 160) && (x <= 230) && (y >= 193) && (y <= 249))
      {
        Serial.println("9");
        if (i<4)
        {
          numbers[i] = 9;
          i++;
          switch(i)
          {
            case 1:
            LcdTouch.drawString ("9", 26, 25, 4);
            break;
            case 2:
            LcdTouch.drawString ("9", 56, 25, 4);
            break;
            case 3:
            LcdTouch.drawString ("9", 86, 25, 4);
            break;
            case 4:
            LcdTouch.drawString ("9", 116, 25, 4);
            //LcdTouch.drawRect(85, 254, 145, 56, BLUE); //fifth line "ENTER"
            //LcdTouch.print (100, 267, "ENTER", 4, WHITE);
            break;
          }
          //LcdTouch.drawRect(160, 10, 70, 56, BLUE); //first line X
          //LcdTouch.print (185, 25, "X", 4, WHITE);
        }
      }
      else if ((x >= 10) && (x <= 80) && (y >= 254) && (y <= 310))
      {
        Serial.println("0");
        if (i<4)
        {
          numbers[i] = 0;
          i++;
          switch(i)
          {
            case 1:
            LcdTouch.drawString ("0", 26, 25, 4);
            break;
            case 2:
            LcdTouch.drawString ("0", 56, 25, 4);
            break;
            case 3:
            LcdTouch.drawString ("0", 86, 25, 4);
            break;
            case 4:
            LcdTouch.drawString ("0", 116, 25, 4);
            //LcdTouch.drawRect(85, 254, 145, 56, BLUE); //fifth line "ENTER"
            //LcdTouch.print (100, 267, "ENTER", 4, WHITE);
            break;
          }
          //LcdTouch.drawRect(160, 10, 70, 56, BLUE); //first line X
         //LcdTouch.print (185, 25, "X", 4, WHITE);
        }
      }
      else if ((x >= 85) && (x <= 230) && (y >= 254) && (y <= 310))
      {
        Serial.println("ENTER");
        {
          Serial.print("ENTER ");
        Serial.print(numbers[0]);
        Serial.print(numbers[1]);
        Serial.print(numbers[2]);
        Serial.println(numbers[3]);
         W =(numbers[0]*1000)+ (numbers[1]*100)+ (numbers[2]*10)+ numbers[3] ;
        }
      }
      delay(250);
      
      while (!LcdTouch.getTouch(&x , &y));
    
  }
  
}

void clibriat()
{
    uint16_t calData[5];
    LcdTouch.fillScreen(TFT_BLACK);
  LcdTouch.setCursor(20, 0);
  LcdTouch.setTextFont(2);
  LcdTouch.setTextSize(1);
  LcdTouch.setTextColor(TFT_WHITE, TFT_BLACK);
  LcdTouch.println("Touch corners as indicated");
  LcdTouch.setTextFont(1);
  LcdTouch.println();
  LcdTouch.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
  Serial.println();
  Serial.println();
  Serial.print("  uint16_t calData[5] = ");
  Serial.print("{ ");
  
  for (uint8_t i = 0; i < 5; i++)
  {
    Serial.print(calData[i]);
    
    if (i < 4) 
      Serial.print(", ");
  }
  
}
   
