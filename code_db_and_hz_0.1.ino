#include <Wire.h>;
#include <LiquidCrystal_I2C.h>;
LiquidCrystal_I2C lcd(0x27,16,2);
const int sampleWindow = 200; // Sample window width in mS (50 mS = 20Hz)
unsigned int db,sample, sigMax, sigMin;
int f;
int to,le,ri,bo,der,back,check,maxa;
int limit = 60;
int first=0;
unsigned long long delaytime,delayhz;
unsigned long startMillis= millis();

 
void setup() 
{
   Serial.begin(9600);
   sigMax = 0;
   sigMin = 1024;
   pinMode(A0, INPUT); //top
  pinMode(A1, INPUT); //right
  pinMode(A2, INPUT); //bot
  pinMode(A3, INPUT); //left
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  
  pinMode(9, OUTPUT); //top & right
  pinMode(8, OUTPUT); //right
  pinMode(7, OUTPUT); //bot & right
  pinMode(6, OUTPUT); //bot
  pinMode(5, OUTPUT); //left & bot
  pinMode(4, OUTPUT); //left
  pinMode(3, OUTPUT); //top & left
  pinMode(2, OUTPUT); //top
  
digitalWrite(2,LOW);
digitalWrite(3,LOW);
digitalWrite(4,LOW);
digitalWrite(5,LOW);
digitalWrite(6,LOW);
digitalWrite(7,LOW);
digitalWrite(8,LOW);
back!=-1;
lcd.init();       //turnonlcd
lcd.backlight();   //led panel on  
   
}
 
void dbre()
{
if(delaytime >= millis()){

to = ((to+83.2073) / 11.003);
le = analogRead(A3);
le = ((le+83.2073) / 11.003);
ri = analogRead(A1);
ri = ((ri+83.2073) / 11.003);
bo = analogRead(A2);
bo = ((bo+83.2073) / 11.003);
Serial.print(to);
Serial.print(" ");
Serial.print(le);
Serial.print(" ");
Serial.print(ri);
Serial.print(" ");
Serial.print(bo);
Serial.print("\n");
maxa= bo;
der=4;
if (maxa < le){
maxa=le;
der=2;
}
if (maxa < ri){
  maxa=ri;
  der=3;
}
if (maxa < to){
  maxa=to;
  der=1;
}

if(to==le&&le==ri&&ri==bo){
  if (back!=-1) der = back;
  else check = 0;
}
//TH2
else{
if (to>limit||le > limit||ri > limit||bo > limit)
{  
digitalWrite(2,LOW);
digitalWrite(3,LOW);
digitalWrite(4,LOW);
digitalWrite(5,LOW);
digitalWrite(6,LOW);
digitalWrite(7,LOW);
digitalWrite(8,LOW);
digitalWrite(9,LOW);
if (der==1) //top is max
   {
      if (to > limit){
        if (le>= limit || ri>=limit){
            if (le>ri){
              digitalWrite(3,HIGH);
              }else{ 
                    if (le=ri) {digitalWrite(2,HIGH);
                                }else{digitalWrite(9,HIGH);}
                   }
        }else{digitalWrite(2,HIGH);}
          delaytime=millis()+250;
        }
          }
if (der==2) //left is max
   {
      if (le > limit){
        if (to>= limit || bo>=limit){
            if (to>bo){
              digitalWrite(9,HIGH);
              }else{ 
                    if (to=bo) {digitalWrite(8,HIGH);
                                }else{digitalWrite(7,HIGH);}
                   }
        }else{digitalWrite(8,HIGH);}
          delaytime=millis()+250;
        }
          }
if (der==3) //right is max
    {
      if (ri > limit){
        if (to>= limit || bo>=limit){
            if (to>bo){
              digitalWrite(3,HIGH);
              }else{ 
                    if (to=bo) {digitalWrite(4,HIGH);
                                }else{digitalWrite(5,HIGH);}
                   }
        }else{digitalWrite(4,HIGH);}
          delaytime=millis()+250;
        }
          }
if (der==4)//bot is max
    {
      if (bo> limit){
        if (le >= limit || ri >= limit){
            if (le>ri){
              digitalWrite(5,HIGH);
              }else{ 
                    if (le=ri) {digitalWrite(6,HIGH);
                                }else{digitalWrite(7,HIGH);}
                   }
        }else{digitalWrite(6,HIGH);}
          delaytime=millis()+250;
        }
          }
  check = 1;  
}
}
}
} 
  
 
void loop() 
{    
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int before = 0, sample;
   unsigned long start = 0, stop = 0;
   bool up = false;      
   while (true)
   {            
      sample = analogRead(A0);
      to = sample;
      dbre();
//      Serial.println(sample);
      if (sample > sigMax)
        sigMax = sample;
      if (sample < sigMin)
        sigMin = sample;
      if (start == 0) {
        if (sample > before) {
          start = millis();
          up = true;          
        }
      } else {
        if ((sample > before) && (up == false)) {
          break;
        } 
        else if (sample < before) {
          stop = millis();
          up = false;
        }
      }
      before = sample;      
   }
   f ++;
   if (millis() - startMillis >= sampleWindow) {
//      if ((sigMax - sigMin <= 20)) {
//        Serial.println("NOISY");
//      } else {
        lcd.clear();
        Serial.print(f * (1000 / sampleWindow)); 
        Serial.print("Hz "); 
        lcd.print(f * (1000 / sampleWindow)); 
        lcd.print("Hz "); 
              
        
        if ((f * (1000 / sampleWindow)) >= 700 && (sigMax - sigMin >= 100)) {
          
        } 
                
      sigMax = 0;
      sigMin = 1024;
      startMillis = millis();  
      f = 0;
   }   
}
