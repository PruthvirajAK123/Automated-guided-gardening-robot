#include <SPI.h>
#include <SD.h>
#define ENC_IN_LEFT_A 2
#define ENC_IN_RIGHT_A 3
#define ENC_IN_LEFT_B 4
#define ENC_IN_RIGHT_B 11

boolean Direction_left = true;
boolean Direction_right = true;
const int encoder_minimum = -32768;
const int encoder_maximum = 32767;
volatile int counter = 0;
volatile int counter1 = 0;
int interval = 100;
long previousMillis = 0;
long currentMillis = 0;

#define m1 6
#define d1 7
#define m2 8
#define d2 9

#define s 15

int k = -1;
int store[100];
int store1[100];



File myFile;


void setup() {
  pinMode(ENC_IN_LEFT_A , INPUT_PULLUP);
  pinMode(ENC_IN_LEFT_B , INPUT);
  pinMode(ENC_IN_RIGHT_A , INPUT_PULLUP);
  pinMode(ENC_IN_RIGHT_B , INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_IN_LEFT_A), left_wheel_tick, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_IN_RIGHT_A), right_wheel_tick, RISING);

  pinMode(m1, OUTPUT);
  pinMode(d1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(d2, OUTPUT);

  analogWrite(m1, s);
  analogWrite(d1, 0);
  analogWrite(m2, s);
  analogWrite(d2, 0);

  Serial.begin(9600);


  while (!Serial) {

  }
  Serial.print("Initializing SD card...");
  pinMode(53, OUTPUT);
  digitalWrite(53, HIGH);
  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");


}


void loop() {

  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    Serial.print("Right Wheel Value = ");
    Serial.print(counter);
    Serial.print(" , Left Wheel Value = ");
    Serial.println(counter1);
  }

  if (Serial.available() > 0) {
    char c = Serial.read();

    //###############################INSERT THE DATA #####################################


    if (c == 'a') {
      //break;
    }

    else if (c == 'F') {
      analogWrite(m1, s);
      analogWrite(d1, s);
      analogWrite(m2, s);
      analogWrite(d2, s);
      Serial.println("Inside F");

    }
    else if (c == 'B') {
      analogWrite(m1, 0);
      analogWrite(d1, s);
      analogWrite(m2, 0);
      analogWrite(d2, s);
      Serial.println("Inside B");
    }
    else if (c == 'R') {
      analogWrite(m1, 0);
      analogWrite(d1, s);
      analogWrite(m2, s);
      analogWrite(d2, s);
      Serial.println("Inside R");
    }
    else if (c == 'L') {
      analogWrite(m1, s);
      analogWrite(d1, s);
      analogWrite(m2, 0);
      analogWrite(d2, s);
      Serial.println("Inside L");
    }


    else if (c == 'S') {
      analogWrite(m1, s);
      analogWrite(d1, 0);
      analogWrite(m2, s);
      analogWrite(d2, 0);
      Serial.println("Inside S");
            myFile = SD.open("test.txt", FILE_WRITE);
            if (myFile) {
              Serial.print("Writing to test.txt...");
              while (counter >= 255 and counter1 >= 255) {
                myFile.write(255);
                myFile.write(1);
                myFile.write(255);
                myFile.write(1);
                counter -= 255;
                counter1 -= 255;
              }
              while (counter <= -255 and counter1 <= -255) {
                myFile.write(255);
                myFile.write(2);
                myFile.write(255);
                myFile.write(2);
                counter = counter + 255;
                counter1 = counter1 + 255;
              }
              if (counter >= 0 and counter1 >= 0 ) {
                myFile.write(counter);
                myFile.write(1);
                myFile.write(counter1);
                myFile.write(1);
                counter = 0;
                counter1 = 0;
              } else {
                myFile.write(counter);
                myFile.write(2);
                myFile.write(counter1);
                myFile.write(2);
                counter = 0;
                counter1 = 0;
              }
              myFile.close();
              Serial.println("done.");
          }
      
          else {
            Serial.println("error opening test.txt");
          }
    }




    //################################DELETE THE DATA ####################################

    else if (c == 'E') {
      SD.remove("test.txt");
      if (SD.exists("test.txt") == 0) {
        Serial.print("done deleting file");
      }
    }
    //################################ EXECUTE ACTIONS ACCORDING TO THE STORED DATA ####################################


    else if (c == 'T') {
       ********************************************************************************************************************
            myFile = SD.open("test.txt");
            if (myFile) {
              Serial.println("test.txt:");
              int e, es, f, fs;
              while (myFile.available()) {
                e = myFile.read();
                es = myFile.read();
                f = myFile.read();
                fs = myFile.read();
      
                k = k + 1;
                if (es == 1) {
                  store[k] = e;
                } else {
                  store[k] = -e;
                }
                if (fs == 1) {
                  store1[k] = f;
                } else {
                  store1[k] = -f;
                }
                // Serial.println( store[k]);
                // Serial.println( store1[k]);
              }
              myFile.close();
//      k = k + 1;
//      store[k] = counter;
//      store1[k] = counter1;
//      counter = 0;
//      counter1 = 0;
      for (int j = 0; j <= k; j++) {
        Serial.println(store[j]);
      }
      Serial.print("######### \n");
      for (int j = 0; j <= k; j++) {
        Serial.println(store1[j]);
      }
          else {
            Serial.println("error opening test.txt");
          }
      delay(2000);
      int j = 0;
      while (j <= k ) {
        int distance = 10;







        //*******************************************************************************************************************
        if (distance != 3 ) {

          if (counter > store[j] && counter1 > store1[j]
             ) {
            analogWrite(m1, 0);
            analogWrite(d1, s);
            analogWrite(m2, 0);
            analogWrite(d2, s);
            Serial.print("BACKWARD __");
            Serial.print(counter);
            Serial.print("__");
            Serial.println(counter1);
          }
          else if (counter < store[j] && counter1 < store1[j]) {
            analogWrite(m1, s);
            analogWrite(d1, s);
            analogWrite(m2, s);
            analogWrite(d2, s);
            Serial.print("FORWARD __");

            Serial.print(counter);
            Serial.print("__");
            Serial.println(counter1);
          }
          else if (counter == store[j] || counter1 == store1[j]) {
            analogWrite(m1, s);
            analogWrite(d1, 0);
            analogWrite(m2, s);
            analogWrite(d2, 0);
            Serial.print("STOP __");
            Serial.print(counter);
            Serial.print("__");
            Serial.println(counter1);
            counter1 = 0;
            counter = 0;
            store[j] = 0;
            store1[j] = 0;

            j = j + 1;
            delay(500);
          }

          else if ( counter < store[j] && counter1 > store1[j] ) {
            analogWrite(m1, 0);
            analogWrite(d1, s);
            analogWrite(m2, s);
            analogWrite(d2, s);
            Serial.print(" RIGHT __");
            Serial.print(counter);
            Serial.print("__");
            Serial.println(counter1);
          }

          else if (counter > store[j] && counter1 < store1[j] ) {
            analogWrite(m1, s);
            analogWrite(d1, s);
            analogWrite(m2, 0);
            analogWrite(d2, s);
            Serial.print(" LEFT __");
            Serial.print(counter);
            Serial.print("__");
            Serial.println(counter1);
            //Serial.println("backward");
          }
        }
      }

    }
  }

}
void right_wheel_tick() {
  int val = digitalRead(ENC_IN_RIGHT_B);
  if (val == LOW) {
    Direction_right = false; // Reverse
  }
  else {
    Direction_right = true; // Forward
  }
  if (Direction_right) {
    if (counter == encoder_maximum) {
      counter = encoder_minimum;
    }
    else {
      counter++;
    }
  }
  else {
    if (counter == encoder_minimum) {
      counter = encoder_maximum;
    }
    else {
      counter--;
    }
  }
}
void left_wheel_tick() {
  int val = digitalRead(ENC_IN_LEFT_B);
  if (val == LOW) {
    Direction_left = true; // Reverse
  }
  else {
    Direction_left = false; // Forward
  }

  if (Direction_left) {
    if (counter1 == encoder_maximum) {
      counter1 = encoder_minimum;
    }
    else {
      counter1++;
    }
  }
  else {
    if (counter1 == encoder_minimum) {
      counter1 = encoder_maximum;
    }
    else {
      counter1--;
    }
  }
}
