int data = 0;

int sens = 0;
int previous_sens = 0;
int tmp = 0;

const int DIRL = 4;
const int DIRR = 3;
const int PWML = 5;
const int PWMR = 2;

bool Ra = false;
bool Rb = false;
bool Rc = false;
bool Rd = false;
bool Infa = false;
bool Infb = false;
bool Acou = false;
bool Magn = false;

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

void setup(){
  Serial.begin(9600);  
  pinMode(DIRL, OUTPUT);   
  pinMode(DIRR, OUTPUT); 
  pinMode(PWML, OUTPUT);   
  pinMode(PWMR, OUTPUT); 
}
 
void loop(){
  if(Serial.available()>1){
    
    LS = Serial.read();
    MS = Serial.read();
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
    Serial.println(FB);
    Serial.println(RL);
  }

  if(Ra){
    sens = sens % int(pow(10, 7)) + int(pow(10, 7));
  }
  else{
    sens = sens % int(pow(10, 7));
  }
  if(Rb){
    tmp = sens;
    tmp = sens % 6 + sens/int(pow(10, 7))*int(pow(10,7));
    sens = tmp + int(pow(10 , 6));
  }
  else{
    tmp = sens;
    tmp = sens % 6 + sens/int(pow(10, 7))*int(pow(10,7));
    sens = tmp;
  }
  if(Rc){
    tmp = sens;
    tmp = sens % 5 + sens/int(pow(10, 6))*int(pow(10,6));
    sens = tmp + int(pow(10 , 5));
  }
  else{
    tmp = sens;
    tmp = sens % 5 + sens/int(pow(10, 6))*int(pow(10,6));
    sens = tmp;
  }
  if(Rd){
    tmp = sens;
    tmp = sens % 4 + sens/int(pow(10, 5))*int(pow(10,5));
    sens = tmp + int(pow(10 , 4));
  }
  else{
    tmp = sens;
    tmp = sens % 4 + sens/int(pow(10, 5))*int(pow(10,5));
    sens = tmp;
  }
  if(Infa){
    tmp = sens;
    tmp = sens % 3 + sens/int(pow(10, 4))*int(pow(10,4));
    sens = tmp + int(pow(10 , 3));
  }
  else{
    tmp = sens;
    tmp = sens % 3 + sens/int(pow(10, 4))*int(pow(10,4));
    sens = tmp;
  }
  if(Infb == HIGH){
    tmp = sens;
    tmp = sens % 2 + sens/int(pow(10, 3))*int(pow(10,3));
    sens = tmp + int(pow(10 , 2));
  }
  else{
    tmp = sens;
    tmp = sens % 2 + sens/int(pow(10, 3))*int(pow(10,3));
    sens = tmp;  
   }
  if(Acou){
    tmp = sens;
    tmp = sens % 1 + sens/int(pow(10, 2))*int(pow(10,2));
    sens = tmp + 10;
  }
  else{
    tmp = sens;
    tmp = sens % 1 + sens/int(pow(10, 2))*int(pow(10,2));
    sens = tmp;
  }
  if(Magn){
    tmp = sens;
    tmp = sens/10*10;
    sens = tmp + 1;
  }
  else{
    tmp = sens;
    tmp = sens/10*10;
    sens = tmp;
  }
  if(sens != previous_sens){
    Serial.print(sens);
  }
  previous_sens = sens;
  turnWheels(FB, RL);
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
