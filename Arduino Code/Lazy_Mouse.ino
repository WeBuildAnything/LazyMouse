#include <BleCombo.h>
#include <BleComboKeyboard.h>
#include <Wire.h>
#include <MPU6050.h>

#define BAT_LEVEL 4 //Analog IN
#define STAT_LED 23//Output

//Following pins are input_pullups
#define SCRL_UP 27
#define SCRL_DOWN 12
#define SCRL_RIGHT 14
#define SCRL_LEFT 25

#define CLICK_MID 26

#define LEFTCLICK 18
#define RIGHTCLICK 19

#define FORWARD 34
#define BACK 35

#define PLAY_PAUSE 17
#define NEXT 16
#define PREVIOUS 5

#define VOL_SIG_A 32
#define VOL_SIG_B 33
#define MUTE 13

int pins[] = {5, 12, 13, 14, 16,17,18,19,25,16,27,32,33,34,35};

MPU6050 mpu;
int16_t ax,ay,az,gx,gy,gz; //variable for IMU
int dx, dy; //mapped Variables

unsigned long previousMillis = 0;
const long interval = 900000;
unsigned long currentMillis;
volatile int lastEncoderA = LOW;
volatile int lastEncoderB = LOW;
float bat_volt;
int stat;

void setup()
{
  // put your setup code here, to run once:
  Wire.begin();
  mpu.initialize();
  Mouse.begin();
  Keyboard.begin();
  Serial.begin(115200);
  for(int i=0;i<15;i++)
  {
    pinMode(pins[i],INPUT_PULLUP);
  }
  attachInterrupt(digitalPinToInterrupt(VOL_SIG_A), updateVolume, CHANGE);
  attachInterrupt(digitalPinToInterrupt(VOL_SIG_B), updateVolume, CHANGE);
  bat_volt = (analogRead(BAT_LEVEL)/4095)*3.3;
  stat = map(bat_volt,3,4.2,0,100);
  Keyboard.setBatteryLevel(stat);
}

void loop() {
  currentMillis = millis();
  if(Keyboard.isConnected())
  {
   currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
      {
      previousMillis = currentMillis;
      bat_volt = (analogRead(BAT_LEVEL)/4095)*3.3;
      stat = map(bat_volt,3,4.2,0,100);
      Keyboard.setBatteryLevel(stat);
      }
   //Connected Status
   digitalWrite (STAT_LED,HIGH);
   //Cursor_Move
   mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
   dx=(gx+260)/150;
   dy=(gy+260)/150;
   Mouse.move(dx,dy);
   //Mouse Funtions
   if (digitalRead(LEFTCLICK) == LOW){
      Mouse.press(MOUSE_LEFT);}
   if (digitalRead(LEFTCLICK) == HIGH) {
      Mouse.release(MOUSE_LEFT);}
   if (digitalRead(RIGHTCLICK) == LOW){
      Mouse.press(MOUSE_RIGHT);}
   if (digitalRead(RIGHTCLICK) == HIGH) {
      Mouse.release(MOUSE_RIGHT);}
   if (digitalRead(CLICK_MID) == LOW){
      Mouse.press(MOUSE_MIDDLE);}
   if (digitalRead(CLICK_MID) == HIGH){
     Mouse.release(MOUSE_MIDDLE);}
   if (digitalRead(FORWARD) == LOW){
     Mouse.click(MOUSE_FORWARD);}
   if (digitalRead(BACK) == LOW){
     Mouse.click(MOUSE_BACK);}
   if (digitalRead(BACK) == LOW){
     Mouse.click(MOUSE_BACK);}
    if (digitalRead(SCRL_UP) == LOW){
     Mouse.move(0,0,1);}
    if (digitalRead(SCRL_DOWN) == LOW){
     Mouse.move(0,0,-1);}
    if (digitalRead(SCRL_RIGHT) == LOW){
     Mouse.move(0,0,0,1);}
    if (digitalRead(SCRL_LEFT) == LOW){
     Mouse.move(0,0,0,-1);} 
    //Media Control
    if (digitalRead(MUTE) == LOW){
     Keyboard.write(KEY_MEDIA_MUTE);}
    if (digitalRead(PLAY_PAUSE) == LOW){
     Keyboard.write(KEY_MEDIA_PLAY_PAUSE);}
    if (digitalRead(NEXT) == LOW){
     Keyboard.write(KEY_MEDIA_NEXT_TRACK);}
    if (digitalRead(PREVIOUS) == LOW){
     Keyboard.write(KEY_MEDIA_PREVIOUS_TRACK);}
  }
  //Blink LED if Bluetooth Disconnected
  else
  {
    digitalWrite(STAT_LED,LOW);
    digitalWrite(STAT_LED,HIGH);
    delay(500);
    digitalWrite(STAT_LED,LOW);
    delay(500);
  }
}

void updateVolume(){
  int encoderA = digitalRead(VOL_SIG_A);
  int encoderB = digitalRead(VOL_SIG_B);
  
  if ((lastEncoderA == LOW && encoderA == HIGH) || (lastEncoderB == LOW && encoderB == HIGH))
    {
    // Clockwise rotation
    Keyboard.write(KEY_MEDIA_VOLUME_UP);
    Serial.println("VolumeUp");
    }   
  else if ((lastEncoderA == HIGH && encoderA == LOW) || (lastEncoderB == HIGH && encoderB == LOW))
    {
    // Counterclockwise rotation
    Keyboard.write(KEY_MEDIA_VOLUME_DOWN);
    Serial.println("Volume Down");
    }
  lastEncoderA = encoderA;
  lastEncoderB = encoderB;
}