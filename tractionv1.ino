//declaring pins
const int pin_hf = A0; //for change of voltage detection
const int pin_lf = 12; // for frequency detection
const int switching = 13; //connected to NMOS
const int pin_magnetic = A2; 
const int pin_acoustic1 = A4; 
const int pin_acoustic2 = A3; 
const int pin_IR = 11; 
const int DIRL = 3;
const int DIRR = 4;
const int PWML = 2;
const int PWMR = 5;


//declaring variables for HF detection
long sensorValue; 
const long treshold_hf = 520; 
bool s103khz = false;
bool s67khz = false;
bool NMOS = LOW; 

//declaring variable for LF detection
long microSecs;
long freq;
long sensitivity_lf = 25;  
int lastBit;
int nowVal;
int i; 
long secs;
bool found = false; 
bool s239hz = false; 
bool s151hz = false; 

//declaring variables for IR detection
bool sIRdetect = false;  
bool s421hz = false; 
bool s607hz = false; 
long sensitivity_IR = 40; 
bool foundIR = false; 
int countIR = 0; 

//declaring variables for acoustic detection: 
long state_acoustic = false; 
bool sAcoustic = false; 
long acousticA; 
long acousticB; 
int treshold_acoustic = 560; 
bool sMagnetic = false; 

//declaring signal sources: 
bool Gaborite = false;
bool Nucinkisite = false;
bool Brookesite = false;
bool Durranium = false;
bool Cheungtium = false;
bool Yeatmanine = false;

int data = 0;
int sens = 0;
int previous_sens = 0;
int tmp = 0;

bool stopped = false;

int FB = 0;
int RL =  0;
double SpeedR = 0;
double SpeedL = 0;
int cL = 0;
int cR = 0;

int MS = 0;
int LS = 0;
int num;
int pFB;
int pRL;

bool mom = true;

bool first = true;

void setup(){
  Serial.begin(9600);  
  pinMode(DIRL, OUTPUT);   
  pinMode(DIRR, OUTPUT); 
  pinMode(PWML, OUTPUT);   
  pinMode(PWMR, OUTPUT); 
  //declaring inputs
  pinMode(pin_lf,INPUT);
  pinMode(pin_hf, INPUT); 
  pinMode(pin_magnetic, INPUT); 
  pinMode(pin_acoustic1,INPUT); 
  pinMode(pin_acoustic2, INPUT); 
  pinMode(pin_IR, INPUT); 
  pinMode(7, INPUT);
  //declaring outputs
  pinMode(switching, OUTPUT); 
  //declaring lf variables
  freq = 0;
  lastBit = 0;
  nowVal =0;
  i=0; 
}
 
void loop(){
  
 if(Serial.available()>1){
    
    LS = Serial.read();
    MS = Serial.read();
    
    //if(LS == 214 && MS == 11){
    if(!mom){
      Gaborite = false;
      Nucinkisite = false;
      Brookesite = false;
      Durranium = false;
      Cheungtium = false;
      Yeatmanine = false;
    }
    
    else{
      
      //if(LS == 228 && MS == 7){
      if(mom){
        stopped = true;
      }
      else{
        stopped = false;
        num = MS*pow(2,8) + LS;
        pFB = num/100;
        pRL = num - pFB*100;
        if(pFB > 9){
         FB = pFB - 10;
       }
        else{
         FB = 0 - pFB;
       }
       if(pRL > 9){
         RL = pRL - 10;
       }
       else{
          RL = 0 - pRL;
        }
        //Serial.println(FB);
        //Serial.println(RL);
      }
    }
  }

  if(stopped){
    // STEP 1: HIGH FREQUENCY DETECTION: 

    sensorValue= analogRead(pin_hf);
    
    map(sensorValue, 0, 1023, 0, 5000);
    //DEBUG: Serial.println (sensorValue); 
    // if the FET is off turn it on and vice-versa:
   
    if (NMOS == LOW) {
      if (sensorValue> treshold_hf){
        s103khz = HIGH;
     //   Serial.println("s103khz detected!"); 
      }
      else{
       NMOS = HIGH;
      }
    } 
    else {
       if (sensorValue> treshold_hf){
        s67khz = HIGH;
      //  Serial.println("s67khz detected!"); 
      }
      else{
        NMOS = LOW;
      }
    }
    digitalWrite(switching, NMOS);
    
    //STEP 2: LOW FREQUENCY DETECTION (only if a HIGH FREQUENCY has been detected)

    if (s103khz||s67khz){
         // setup
         delay(4);
          microSecs = micros(); 
          secs= millis(); 
         while(!found){
          if ((microSecs+15)<micros()){
              nowVal=digitalRead(pin_lf);
              if (lastBit!=nowVal) {
                lastBit=nowVal;
                freq= freq+1;
              }
              microSecs = micros();
          }
          if (secs+250<millis()){
              freq*=2; 
              //Serial.print((freq));
              //Serial.print("hz");
              if ((freq < (151+sensitivity_lf))&&(freq >(151-sensitivity_lf))){
                   s151hz = true; 
                //   Serial.print("S151hz detected!"); 
                   found = true; 
                 }
                 
              if ((freq < (239+sensitivity_lf))&&(freq >(239-sensitivity_lf))){
                   s239hz = true; 
                   found = true; 
                  //Serial.print("S239hz detected!"); 
                 }  
              freq=0;
              secs=millis();
              i++; 
          }
          if (i==6){
            found = true; 
          }
        }
     }      
     i=0; 
     found = false;   
    //STEP 3 : ACOUSTIC DETECTION:
    
    acousticA = analogRead(pin_acoustic1); 
    acousticB = analogRead(pin_acoustic2); 
 
    Serial.println("Acoustic 1"); 
    Serial.println(acousticA); 
    Serial.println("Acoustic 2"); 
    Serial.println(acousticB);
    map(acousticA , 0, 1023, 0, 5000); 
    map(acousticB , 0, 1023, 0, 5000); 
    if ((acousticA> treshold_acoustic)||(acousticB > treshold_acoustic)){
      sAcoustic = true; 
      //Serial.println("Signal acoustic detected!"); 
    }

    //STEP 4: IR detection: (only happens if radio signals NOT detected)
  
   // if ((!s103khz)||(!s67khz)){
  
       sIRdetect = digitalRead(pin_IR); 
       if (sIRdetect){
        //Serial.println(" isgnal IR detected!"); 
       }

      if (sIRdetect){
          microSecs = micros(); 
          secs= millis(); 
      
      while (!foundIR){
        if ((microSecs+20)<micros()){
            nowVal=digitalRead(11);
        if (lastBit!=nowVal) {
              lastBit=nowVal;
              freq=freq+1;
            }
            microSecs = micros();
        }
        if (secs+250<millis()){
            freq *=2;
            Serial.print((freq));
            Serial.print("hz");

            if ((freq < (421+sensitivity_IR))&&(freq >(421-sensitivity_IR))){
                   s421hz = true; 
                   //Serial.print("S421hz detected!"); 
                   foundIR = true; 
              }
                 
            if ((freq < (607+sensitivity_IR))&&(freq >(607-sensitivity_IR))){
                s607hz = true; 
                foundIR = true; 
                //Serial.print("S607hz detected!"); 
                }  
            freq=0;
            countIR++; 
            secs=millis();
        }
        if (countIR == 5){
          foundIR = true; 
        }
      }
      foundIR = false; 
      }
    
    //STEP 5: CONDITIONS CHECK FOR THE SOURCES: 
      if (/*(s67khz && s151hz) || (s67khz && sAcoustic) || (s151hz && sAcoustic)*/7 == HIGH){
      Gaborite = true;
      Serial.println("Gaborite FOUND!"); 
      }
     if ((s67khz && s239hz) || (s67khz && sMagnetic) || (s239hz && sMagnetic)){
      Nucinkisite = true;
      Serial.println("Nucinkisite FOUND!"); 
      }
     if ((s103khz && s151hz) || (s103khz && sMagnetic) || (s151hz && sMagnetic)){
      Durranium = true;
      Serial.println("Durranium FOUND!"); 
      }
     if (s103khz && s239hz){
      Brookesite = true;
      Serial.println("Brookesite FOUND!"); 
      }
     if (s421hz){
       Cheungtium  = true;
       Serial.println("Cheungtium FOUND!"); 
       }
       
     if (s607hz||(sIRdetect && sAcoustic)){
       Yeatmanine = true;
       Serial.println("Yeatmanine FOUND!"); 
       }

    //STEP 6: RESET ALL PARAMETERS: 
     s103khz = false; 
     s67khz = false; 
     
     s239hz = false; 
     s151hz = false; 
     
     sAcoustic = false; 
     sMagnetic = false; 
     
     s607hz = false; 
     s421hz = false; 
     sIRdetect = false; 
     foundIR = false; 
     countIR = 0; 
  }
  
  if(Gaborite){
    sens = 1;
  }
  if(Nucinkisite){
    sens = 2;
  }
  if(Brookesite){
    sens = 3;
  }
  if(Durranium){
    sens = 4;
  }
  if(Cheungtium){
    sens = 5;
  }
  if(Yeatmanine){
    sens = 6;
  }

  if(sens != previous_sens){
    Serial.write(sens);
  }  
  previous_sens = sens;
  if(!stopped){
      turnWheels(FB, RL);
  }
}

void turnWheels(int FB, int RL){
  SpeedR = FB - RL;
  if(SpeedR > 10){
    SpeedR = 10;
  }
  if(SpeedR < -10){
    SpeedR = -10;
  }
  SpeedL = FB + RL;
  if(SpeedL > 10){
    SpeedL = 10;
  }
  if(SpeedL < -10){
    SpeedL = -10;
  }
  
  if(SpeedL>0){
    digitalWrite(DIRL, HIGH);
  }
  else{
    digitalWrite(DIRL, LOW);
  }
  if(cL < 8 & SpeedL!=0){
    if(cL<abs(SpeedL)){
      digitalWrite(PWML, HIGH);
    }
    else{
      digitalWrite(PWML, LOW);
    }
    cL++;
  }
  else{
    cL = 0;
    if(SpeedL!=0){
      digitalWrite(PWML , HIGH);
    }
    else{
      digitalWrite(PWML, LOW);
    }
  }
  
  if(SpeedR>0){
    digitalWrite(DIRR, HIGH);
  }
  else{
    digitalWrite(DIRR, LOW);
  }
  if(cR < 8 & SpeedR!=0){
    if(cR<abs(SpeedR)){
      digitalWrite(PWMR, HIGH);
    }
    else{
      digitalWrite(PWMR, LOW);
    }
    cR++;
  }
  else{
    cR = 0;
    if(SpeedR!=0){
      digitalWrite(PWMR , HIGH);
    }
    else{
      digitalWrite(PWMR, LOW);
    }
  }
}
