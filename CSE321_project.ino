//Reactionyzer V1
//Sean Mackay
//11-16-19
//cheap-to-make replacement to breathalyzers that test peoples ability to react to visual stimuli

//Here are the states for the FSM (for maintenance use and expansion)
/*States: 
   * 0: Default state, nothing happened
   * 2: All three buttons pressed, go into settings
   * 6: Right button -> go into car mode (flip bool value to True
   * 7: Centre button -> exit settings
   * 8: Press centre button again -> Begin randomizer
   * 9: (delay till led turns on) -> correct button pressed -> calculate the reaction time
   */



#include <LiquidCrystal.h>


  LiquidCrystal lcd(12, 10, 5, 4, 3, 2);

  //button pin initialize
  const int button1pin = 6;
  const int button2pin = 13;
  const int button3pin = 0;

  //ledpin initialize
  const int led1pin = 7;
  const int led2pin = 8;
  const int led3pin = 9;
  
  const int carPowerPin = 11;

  //Vars for overall variable types
  int state =0; //set to initial state
  int mode = -1; //0 for car, 1 for field. Set to -1 (error) by default
  int randomLED =4; //seed for choosing a random led (from 1 to 3) 
  int randomTime =0; //the random time the led will be turned on.

  //states for each of the LED's 
  int ledState1=LOW;
  int ledState2=LOW;
  int ledState3=LOW;
  int buttonState1=LOW;
  int buttonState2=LOW;
  int buttonState3=LOW;
  
  int carPowerPinState=LOW;

  int button2Pressed=0;

  int reactionTime=9999;
  
  //for debouncing of the buttons
  unsigned long lastDebounceTime1 =0;
  unsigned long lastDebounceTime2 =0;
  unsigned long lastDebounceTime3 =0;
  unsigned long debouncedelay =50;


//turns on the random led 
void ledOn(int led){
  
   if (led==0){     
    digitalWrite(led1pin,HIGH);   
   }   
   else if(led==1){
    digitalWrite(led2pin,HIGH);  
   }   
   else if(led==2){     
    digitalWrite(led3pin,HIGH);   
   }   
}

//handles actions on center button
void statezero(){
  if(buttonState1==LOW && buttonState2==HIGH && buttonState3==LOW && state==0){
    delay(500);
    state=2;
    }
  else if(buttonState1==HIGH && buttonState2==LOW && buttonState3==HIGH ){  
      delay(500);
      state=5;
      randomSeed(analogRead(0));
      randomLED=random(0,3);
      randomTime=random(1000,3000);
      delay(randomTime);
      ledOn(randomLED);
      int timeLit=millis();
      while(1){
        buttonState1=digitalRead(button1pin);   
        buttonState2=digitalRead(button2pin);   
        buttonState3=digitalRead(button3pin);   
        
        if(randomLED==2 && buttonState1==LOW && buttonState2==HIGH && buttonState3==HIGH){
          int timepressed=millis();
          reactionTime=timepressed-timeLit;
          if(timepressed-timeLit<=500){
            state=8;
            break;
          }
          else{
            state=6;
            break;
          }
        }
        
        else if(randomLED==1 && buttonState1==HIGH && buttonState2==LOW && buttonState3==HIGH){
          int timepressed=millis();
          reactionTime=timepressed-timeLit;
          if(timepressed-timeLit<=500){
            state=8;
            break;
          }
          else{
            state=6;
            break;
          }
        }

        else if(randomLED==0 && buttonState1 ==HIGH && buttonState2==HIGH && buttonState3==LOW){
          int timepressed=millis();
          reactionTime=timepressed-timeLit;
          if(timepressed-timeLit<=500){
            state=8;
            break;
          }
          else{
            state=6;
            break;
          }
        }

        else if(buttonState1==HIGH && buttonState2==HIGH && buttonState3==HIGH){
          //continue;
        }
        else{
          state=7;
          break;
        }
        
      }
  }
  else{
    state=0;
  }
}

//handles settings menu
void menuControls(){
  state=2;
  lcd.clear();
    lcd.print("    Settings    ");
    lcd.setCursor(0,1);
    lcd.print("<-CAR FIELD ->   ");
  while(1){
    buttonState1=digitalRead(button1pin);   
    buttonState2=digitalRead(button2pin);   
    buttonState3=digitalRead(button3pin);  
    
    if(buttonState1==LOW && buttonState2==HIGH && buttonState3==HIGH){
        lcd.clear();
        lcd.print("    Settings    ");
        lcd.setCursor(0,1);
        lcd.print("    Car Mode    ");
        mode=0;
    }
    else if(buttonState1==HIGH && buttonState2==LOW && buttonState3==HIGH){
      lcd.clear();
      lcd.print("    Settings    ");
      lcd.setCursor(0,1);
      lcd.print("    Saved");
      state=0;
      button2Pressed=0;
      delay(2000);
      lcd.clear();
      lcd.print("  Reactionyzer!");
      break;
    }
    else if(buttonState1==HIGH && buttonState2==HIGH && buttonState3==LOW){
        lcd.clear();
        lcd.print("    Settings    ");
        lcd.setCursor(0,1);
        lcd.print("    Field Mode ");
        mode=1;
      //delay(1000);
    }
    else if(buttonState1==HIGH && buttonState2==HIGH && buttonState3==HIGH){
      
    }
    else{
      lcd.clear();
      lcd.print("button error");
    }
    
  }
  
}

//prints information to screen when the user passes
void screenPrinting(){
  lcd.clear();
  lcd.print("TEST PASSED");
  lcd.setCursor(0,1);
  if(mode==1){
    
    digitalWrite(led1pin,HIGH);
    delay(300);
    digitalWrite(led2pin,HIGH);
    delay(300);
    digitalWrite(led3pin,HIGH);
    delay(1000);
    digitalWrite(led3pin,LOW);
    delay(300);
    digitalWrite(led2pin,LOW);
    delay(300);
    digitalWrite(led1pin,LOW);
    
    lcd.print("PERSON SOBRE");
    delay(5000);
    lcd.clear();
    lcd.print("PERSON SOBRE");
    lcd.setCursor(0,1);
    lcd.print(reactionTime);
    lcd.setCursor(4,1);
    lcd.print("MS");
  }
  else if(mode==0){
    lcd.print("CAR TURNED ON");
    digitalWrite(carPowerPin,HIGH);
  }
  else{
    lcd.print("NO MODE SET");  
}
}
//setup function
void setup() {
  // put your setup code here, to run once:
  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Reactionyzer!");
  pinMode(button1pin,INPUT_PULLUP);
  pinMode(button2pin,INPUT_PULLUP);
  pinMode(button3pin,INPUT_PULLUP);

  pinMode(led1pin,OUTPUT);
  pinMode(led2pin,OUTPUT);
  pinMode(led3pin,OUTPUT);
  pinMode(carPowerPin,OUTPUT);
  
  digitalWrite(led1pin,LOW);
  digitalWrite(led2pin,LOW);
  digitalWrite(led3pin,LOW);
  
  digitalWrite(carPowerPin,LOW);
  
  button2Pressed=0;

  
}

//main function
void loop() {
    buttonState1=digitalRead(button1pin);   
    buttonState2=digitalRead(button2pin);   
    buttonState3=digitalRead(button3pin);
    
  switch(state){
    //first case (home screen
    case(0):
      statezero(); 
      break;

    //if left and right pressed, go to settings menu
    case(2):
      menuControls();
      break;

    //if response was too slow
    case(6):
      lcd.clear();
      lcd.print("    TOO SLOW  ");
      lcd.setCursor(0,1);
      lcd.print("   TEST FAILED");
      delay(5000);
      state=9;
      break;

    //if the wrong button was pressed by the user
    case(7):
      lcd.clear();
      lcd.print("  WRONG BUTTON  ");
      lcd.setCursor(0,1);
      lcd.print("  TEST FAILED   ");
      delay(5000);
      state=9;
      break;
    
    //test passed
    case(8):
      lcd.clear();
      screenPrinting();
      
      delay(10000);
       digitalWrite(carPowerPin,LOW);
      state=9;
      break;
    case(9):
      digitalWrite(led1pin,LOW);
      digitalWrite(led2pin,LOW);
      digitalWrite(led3pin,LOW);
      lcd.clear();
      lcd.print("Reactionyzer!");
      state=0;
      break;
  }

}
