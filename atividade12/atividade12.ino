#define ledpin 22

void blink_led(){
    for (int i=0; i<5; i++){
      digitalWrite(ledpin, HIGH);
      delay(2000);
      digitalWrite(ledpin, LOW);
      delay(2000);
    }
  }

void setup(){
  Serial.begin(9600);
  pinMode(ledpin, OUTPUT);
  blink_led();
  }


void loop(){
  }
