#include <Stepper.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <MFRC522.h>

const int rs = A0 ,en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN,RST_PIN);
const byte ROWS = 4; 
const byte COLS = 4;
char Keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {3, 2, 1, 0}; 
byte colPins[COLS] = {7, 6, 5, 4};
Keypad keypad = Keypad( makeKeymap(Keys), rowPins, colPins, ROWS, COLS); 
const int stepsPerRevolution=100;
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);
void setup() {
Serial.begin(9600);
SPI.begin();
mfrc522.PCD_Init();
lcd.begin(16,2);
myStepper.setSpeed(60);

}

void loop() {
if(mfrc522.PICC_IsNewCardPresent()&& mfrc522.PICC_ReadCardSerial()){
  String uid ="";
  for(byte i =0;i< mfrc522.uid.size;i++)
  {uid.concat(String(mfrc522.uid.uidByte[i] <0*10 ?"0":""));
  uid.concat(String(mfrc522.uid.uidByte[i],HEX));
}
Serial.println("UID de la carte:" + uid );
if (uid =="538f4397"){
  Serial.println("Carte autorisée");
  lcd.clear();
  lcd.print("carte autorisée");
  delay(1000);

  lcd.clear();
  lcd.print("code :");
  String code ="";
  while(code.length()<4){
    char key = keypad.getKey();
    if(key !=NO_KEY){
      code += key;
      lcd.setCursor(code.length(),1);
      lcd.print("*");
    }
  }
  if (code =="1234"){
    Serial.println("code autorise ");
    lcd.clear();
    lcd.print("code autorise");
    delay(2000);

    myStepper.step(stepsPerRevolution);
    delay(500);
    myStepper.step(-stepsPerRevolution);
    delay(500);
   
 
    }
    else{Serial.println("code non autorisé");
    lcd.clear();
    lcd.print("code non autorisé");
    
    
  }
}else{Serial.println("carte non autorisé");
lcd.clear();
lcd.setCursor(-8,0);
lcd.print("carte non autorisé");
delay(1000);

}mfrc522.PICC_HaltA();
}
    }
 
