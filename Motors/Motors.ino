const int pin1A = 6;
const int pin2A = 7;
const int en12 = 8;
const int pin3A = 10;
const int pin4A = 11;
const int en34 = 12;
const int Encoder1A = 13;
const int Encoder1B = 14;
const int Encoder2A = 15;
const int Encoder2B = 16;


void setup() {
  pinMode(en12,OUTPUT);
  pinMode(pin1A,OUTPUT);
  pinMode(pin2A,OUTPUT);
  pinMode(en34,OUTPUT);
  pinMode(pin3A,OUTPUT);
  pinMode(pin4A,OUTPUT);
  pinMode(Encoder1A,INPUT);
  pinMode(Encoder1B,INPUT);
  pinMode(Encoder2A,INPUT);
  pinMode(Encoder2B,INPUT);
}

void loop() {

  digitalWrite(pin1A,HIGH);
  digitalWrite(pin2A,HIGH);

  digitalWrite(pin3A,HIGH);
  digitalWrite(pin4A,HIGH);

  read_encoder()

}

void stop(){
  digitalWrite(pin1A,LOW);
  digitalWrite(pin2A,LOW);
  digitalWrite(pin3A,LOW);
  digitalWrite(pin4A,LOW);
}

void turn(int degrees, bool left_right) {

}

void read_encoder(){

}


