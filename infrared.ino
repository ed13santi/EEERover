
long microSecs;
long freq;
int lastVal;
long pulses; 
int nowVal;
long secs;
bool s239hz = false; 
bool s151hz = false; 
long sensitivity_lf = 15; 
const int NMOS = 3; 
void setup() {

  pinMode(A1,INPUT);
  pinMode(NMOS, OUTPUT); 

  Serial.begin(9600);
}
void lf_detect(bool& s239hz, bool& s151hz); 
void loop() {
  lf_setup(); 
  if ((microSecs+20)<micros()){
      nowVal=digitalRead(A1);
 
      if (lastVal!=nowVal) {
        lastVal=nowVal;
        pulses ++;
      }
      microSecs = micros();
  }
  if (secs+250<millis()){
      freq = pulses*2; 
      
      if ((freq < (151+sensitivity_lf))&&(freq >(151-sensitivity_lf))){
        s151hz = true; 
      }
      if ((freq < (239+sensitivity_lf))&&(freq >(239-sensitivity_lf))){
        s239hz = true; 
      }
      Serial.print((freq));
      Serial.println("hz");
      freq=0;
      pulses = 0; 
      secs=millis();
  }

}


void lf_setup(){
  microSecs = micros();
  freq = 0;
  lastVal = 0;
  nowVal =0;
  pulses = 0; 
  secs = millis();
  
}


