// Pin definition
int mr1=22;  //motor right 1
int mr2=23;  //motor right 2
int ml1=16;   //motor left 1
int ml2=17;
int enr=21; 
int enl=19;

const int irPins[8] = {32, 33 ,25 ,26 ,27 ,14 ,13, 18};
int threshold=200;

int P, D, I, previousError, PIDvalue, error;
int lsp, rsp;
int lfspeed = 150;
int tdelay=20;

float Kp = 0;
float Kd = 0;
float Ki = 0;

void setup() { 
  Serial.begin(115200);

  for (int i = 0; i < 8; i++) {
    pinMode(irPins[i], INPUT);
  }
  pinMode(mr1,OUTPUT);
  pinMode(mr2,OUTPUT);
  pinMode(ml1,OUTPUT);
  pinMode(ml2,OUTPUT);
   
 delay(2000);
}

void loop() {

    int ir1=analogRead(irPins[0]);
    int ir2=analogRead(irPins[1]);
    int ir3=analogRead(irPins[2]);  //detect black line (3cm)
    int ir4=analogRead(irPins[3]);  //detect black line
    int ir5=analogRead(irPins[4]);
    int ir6=analogRead(irPins[5]);
    int ir7=analogRead(irPins[6]);
    int ir8=analogRead(irPins[7]);

    int weighted = ir1*1000 + ir2*2000 + ir3*3000 + ir4*4000 + ir5*5000 + ir6*6000 + ir7*7000 + ir8*8000;
    int sum = ir1 + ir2 + ir3 + ir4 + ir5 + ir6 + ir7 + ir8;
    int position = weighted/sum;
    int set = 4500;
    int error = set - position;

    if (ir1 > threshold && ir8 < threshold){
      lsp = 0; rsp = lfspeed;
      right(255,255);
      delay(5);

    }
    else if ((ir1 < threshold ) && (ir8 > threshold)){ 
      lsp = lfspeed; rsp = 0;
      left(255, 255);
      delay(5);
   
    }
    else if (ir2 > threshold && ir7 < threshold){
      lsp = 0; rsp = lfspeed;
      right(255,255);
      delay(5);
  
    }
    else if ((ir2 < threshold ) && (ir7 > threshold)){ 
      lsp = lfspeed; rsp = 0;
      left(255, 255);
      delay(5);
    
    }
    else if (ir4 > threshold || ir5 > threshold){  //black
      Kp = 0.8; //0.8
      Kd =8; //8
      Ki = 0; //0.001
      linefollow(error);
    }
    else if (ir1 < threshold && ir2 < threshold && ir3 < threshold && ir4 < threshold && ir5 < threshold && ir6 < threshold && ir7 < threshold && ir8 < threshold){
      forward(100, 100);
      delay(10);
      forward(60, 0);
      delay(10);
    }
 }

void linefollow(int error){

  P = error;
  I = I + error;
  D = error - previousError;

  PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
  previousError = error;

  lsp = lfspeed - PIDvalue;
  rsp = lfspeed + PIDvalue;

  if (lsp > 255) {
    lsp = 255;
  }
  if (lsp < 0) {
    lsp = 0;
  }
  if (rsp > 255) {
    rsp = 255;
  }
  if (rsp < 0) {
    rsp = 0;
  }
  forward(lsp, rsp);
  delay(2);
}

void forward(int s1, int s2){
    digitalWrite(mr1,HIGH);
    digitalWrite(mr2,LOW);
    digitalWrite(ml1,HIGH);
    digitalWrite(ml2,LOW);
    analogWrite (enr,s1);
    analogWrite (enl,s2);
 } 

void backward(int s1, int s2){
    digitalWrite(mr1,LOW);
    digitalWrite(mr2,HIGH);
    digitalWrite(ml1,LOW);
    digitalWrite(ml2,HIGH);
    analogWrite (enr,s1);
    analogWrite (enl,s2);
   }

void right(int s1, int s2){
    digitalWrite(mr1,LOW);
    digitalWrite(mr2,HIGH);
    digitalWrite(ml1,HIGH);
    digitalWrite(ml2,LOW);
    analogWrite (enr,s1);
    analogWrite (enl,s2);
    delay(tdelay);
 } 

void left(int s1, int s2){
    digitalWrite(mr1,HIGH);
    digitalWrite(mr2,LOW);
    digitalWrite(ml1,LOW);
    digitalWrite(ml2,HIGH);
    analogWrite(enr,s1);
    analogWrite(enl,s2);
    delay(tdelay);
}  

void stop(){
    analogWrite(enr,0);
    analogWrite(enl,0);
 }