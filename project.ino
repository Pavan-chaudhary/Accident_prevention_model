int trigPin1=11;//sensor 1
int echoPin1=12;//sensor 1
int trigPin2=8;//sensor 2
int echoPin2=9;//sensor 2
int rled=5;//red signal
int yled=6;//yellow signal
int gled=7;//green signal
int flag=0;//for detecting if someone has arrived at sensor1.
long t,t1,t2,t3;
long d;
long distance=100;//distance between sensor2 and signal pole(in mts.)
long sp;//speed of vehicle
long vlength;//length of vehicle,yellow signal for bikes(which have length<200cms)
long normalDist1,normalDist2;//distance returned without any object infront by sensors.
long newDist1,newDist2;//distance returned when any object comes in front of sensors.

void setup(){
  Serial.begin(9600);
  pinMode(trigPin1,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(trigPin2,OUTPUT);
  pinMode(echoPin2,INPUT);

  digitalWrite(gled,HIGH);//initially signal is green
  
  digitalWrite(trigPin1,LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin1,HIGH);
  delayMicroseconds(10);
  d=pulseIn(echoPin1,HIGH);
  normalDist1=(d/2)/29.1;
  
  digitalWrite(trigPin1,LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin1,HIGH);
  delayMicroseconds(10);
  d=pulseIn(echoPin2,HIGH);
  normalDist2=(d/2)/29.1;
}

void showSignal(int s,int len){
  
    t=(distance*100)/s; //time taken by vehicle to passby signal in microseconds
    s=s*36; //converting cm/ms to kmph
    
    if(s>26 && len>200){
        digitalWrite(gled,LOW);
        digitalWrite(rled,HIGH);
        digitalWrite(yled,LOW);
        delayMicroseconds(t);
    }
    else if((s<26 && s>18) || len<200){
        digitalWrite(gled,LOW);
        digitalWrite(yled,HIGH);
        digitalWrite(rled,LOW);
        delayMicroseconds(t);
    }
    else{
        digitalWrite(rled,LOW);
        digitalWrite(gled,HIGH);
        digitalWrite(yled,LOW);
        delayMicroseconds(t);
    }    
}

void loop(){
  digitalWrite(trigPin1,LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin1,HIGH);
  delayMicroseconds(10);
  d=pulseIn(echoPin1,HIGH);
  newDist1=(d/2)/29.1;
  
   if(flag==1 && newDist1>normalDist1-20){ //using plus-minus 20 to make sensor less sensitive.
     t2=millis(); //vehicles leaves at sensor1
     t=t2-t1; 
     while(1){
          digitalWrite(trigPin2,LOW);
          delayMicroseconds(10);
          digitalWrite(trigPin2,HIGH);
          delayMicroseconds(10);
          d=pulseIn(echoPin2,HIGH);
          newDist2=(d/2)/29.1;
          if(newDist2<normalDist2-20){
               t3=millis();
               t=t3-t1;
               sp=distance/t;
               t=t2-t1;
               vlength=sp*t;
               showSignal(sp,vlength);
               break;    
          }
     }
     flag=0;
     delay(10);
  }
  
  if(newDist1<normalDist1-20 && flag==0){
      Serial.println("entry");
      t1=millis();
      Serial.print(t1);
      flag=1; //for vechile arrives at sensor1   
  }
  
  
}
