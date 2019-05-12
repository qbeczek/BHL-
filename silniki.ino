// Defines Tirg and Echo pins of the Ultrasonic Sensors
#define trigPin1 52
#define echoPin1 53
#define trigPin2 50
#define echoPin2 51
#define trigPin3 48
#define echoPin3 49

#define BUZZER 13

// Variables for the duration and the distance
#define S_P1 22
#define S_P2 24
#define S_L1 28
#define S_L2 30
#define P_PWM A1
#define L_PWM A2

#define STB 26

#define PWM_MAX 400


int distance1;
int distance2;
int distance3;
int i = 0;
int menu;

int bol1 = 0;
int bol2 = 0;

unsigned long czasLeki = 0;
unsigned long czasKolacja = 0;
unsigned long czasSpac = 0;

void setup() 
{
  pinMode(S_P1, OUTPUT);
  pinMode(S_P2, OUTPUT);
  pinMode(P_PWM, OUTPUT);
  
  pinMode(S_L1, OUTPUT);
  pinMode(S_L2, OUTPUT);
  pinMode(L_PWM, OUTPUT);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  
  pinMode(BUZZER, OUTPUT);
  
  pinMode(STB, OUTPUT);
  digitalWrite(STB, HIGH);
  
  Serial.begin(9600);
  Serial3.begin(9600);

  czasSpac = millis();
  czasKolacja = millis();
  czasLeki = millis();
}

void loop() 
{ 
  komunikat();
    if(Serial3.available()>0)
       menu = Serial3.read();
      
  switch(menu) //wybor opcji "zabawy"
  {
      case 'k':
         zdalneSterowanie();
         bol2 = 0;
         break;
         
      case 'l': 
          autonomicznaJazda();
          bol1 = 0;
          break;
          
      case 'e':
          bol1 = 0;
          bol2 = 0;
          break;
  }
}
void autonomicznaJazda(){
  
for(;;){
  if(bol2!=0) break;
  distance1 = calculateDistance1();
  distance2 = calculateDistance2();
  distance3 = calculateDistance3();
  Serial.print(distance1);
  Serial.print("\t"); 
  Serial.print(distance2);
  Serial.print("\t");
  Serial.println(distance3);
    if(distance1<30){
      if(distance2>3){
        rightMotor(-40);
        leftMotor(40);
        Serial3.println("lewo");
        delay(200);
        
      }
      else if(distance3>2){
        rightMotor(40);
        leftMotor(-40);
        Serial3.println("prawo");
        delay(200);
      }
      
      delay(400);
  }   else {
    rightMotor(40);
    leftMotor(40);
    }
  
  if(Serial3.available()>0)
      if( Serial3.read()=='9') 
      { 
        stopMotors();
        ++bol2;
      }
  }
}
void obrotPrawo(){
  rightMotor(-30);
  leftMotor(30);
  delay(500);
}
void obrotLewo(){
  rightMotor(30);
  leftMotor(-30);
  delay(500);
}

void komunikat(){
  if((millis()-czasLeki) >= 120000){
    Serial3.println("Dzień Dobry, tu Przyjaciel proszę nie zapomnieć o lekach. Miłego dnia :)");
    
    czasLeki = millis();
    digitalWrite(BUZZER,HIGH);
    delay(300);
    digitalWrite(BUZZER, LOW);
  }
  if((millis()-czasKolacja) >= 180000){
    Serial3.println("To już czas na na Kolację! :)");
    
    czasKolacja = millis();
    digitalWrite(BUZZER,HIGH);
    delay(300);
    digitalWrite(BUZZER, LOW);
  }
  if((millis()-czasSpac) >= 220000){
    Serial3.println("Już dosyć późno, czas na sen, dobranoc!");
    
    czasSpac = millis();
    digitalWrite(BUZZER, HIGH);
    delay(300);
    digitalWrite(BUZZER, LOW);
  }
}

int calculateDistance1(){ 
  long duration;
  int distance_tmp;
  digitalWrite(trigPin1, LOW); 
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin1, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration = pulseIn(echoPin1, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance_tmp= duration*0.034/2;
  return distance_tmp;
}
int calculateDistance2(){ 
  long duration;
  int distance_tmp;
  digitalWrite(trigPin2, LOW); 
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin2, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration = pulseIn(echoPin2, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance_tmp= duration*0.034/2;
  return distance_tmp;
}
int calculateDistance3(){ 
  long duration;
  int distance_tmp;
  digitalWrite(trigPin3, LOW); 
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin3, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  duration = pulseIn(echoPin3, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance_tmp= duration*0.034/2;
  return distance_tmp;
}

void zdalneSterowanie(){
  int kierunek  = 0;
  for(;;){
     komunikat();
     if(bol1 != 0) break; //jesli bylo wybrane '0' wyjdz z programu
                ///zdalne sterowanie/////
     if(Serial3.available()>0)
             kierunek = Serial3.read();
             switch(kierunek)
             {
                case '8': //do przodu
                  rightMotor(40);
                  leftMotor(40);
                  Serial.println("DO PRZODU");
                  break;
                  
                case '2': //do tylu
                  rightMotor(-40);
                  leftMotor(-40);
                  Serial.println("DO TYŁU");
                  break;
                  
                case '4': //w lewo
                  rightMotor(40);
                  leftMotor(-40);
                  Serial.println("LEWO");
                  break;
                  
                case '6': //w prawo
                  rightMotor(-40);
                  leftMotor(40);
                  break;
                  
                case '5':
                  stopMotors();
                  break;
                case '9':
                  stopMotors();
                  ++bol1;
                  break;
            } 
         }
}

void rightMotor(int V){
   if (V > 0) { //Jesli predkosc jest wieksza od 0 (dodatnia)
    V = map(V, 0, 100, 0, PWM_MAX);
     analogWrite(P_PWM, V); //Ustawienie predkosci 
    digitalWrite(S_P1,  HIGH); //Kierunek: do przodu
    digitalWrite(S_P2, LOW);
   
  } else {
    V = abs(V); //Funkcja abs() zwroci wartosc V  bez znaku
    V = map(V, 0, 100, 0, PWM_MAX);
    analogWrite(P_PWM, V); //Ustawienie predkosci    
    digitalWrite(S_P1,  LOW); //Kierunek: do przodu
    digitalWrite(S_P2, HIGH);
  } 
}
void leftMotor(int V){
   if (V > 0) { //Jesli predkosc jest wieksza od 0 (dodatnia)
    V = map(V, 0, 100, 0, PWM_MAX);
     analogWrite(L_PWM, V); //Ustawienie predkosci 
    digitalWrite(S_L1,  HIGH); //Kierunek: do przodu
    digitalWrite(S_L2, LOW);
   
  } else {
    V = abs(V); //Funkcja abs() zwroci wartosc V  bez znaku
    V = map(V, 0, 100, 0, PWM_MAX);
    analogWrite(L_PWM, V); //Ustawienie predkosci    
    digitalWrite(S_L1, LOW); //Kierunek: do przodu
    digitalWrite(S_L2, HIGH);
  } 
}
void stopMotors() {
  analogWrite(L_PWM, 0); //Wylaczenie silnika lewego
  analogWrite(P_PWM, 0); //Wylaczenie silnika prawego
}

