#include <LiquidCrystal.h>

/*void debouncer(){
  if(reading1!= buttonState1){
    lastDebounceTime1=millis();
  }
  if(reading2!= buttonState2){
    lastDebounceTime2=millis();
  }
  if(reading3!= buttonState3){
    lastDebounceTime3=millis();
  }

  if((millis() - lastDebounceTime1) > debouncedelay){
    if(reading1 != buttonState1) {
      buttonState1 = reading1;
    }
  }
  if((millis() - lastDebounceTime2) > debouncedelay){
    if(reading2 != buttonState2) {
      buttonState2 = reading2;
    }
  }
  if((millis() - lastDebounceTime3) > debouncedelay){
    if(reading3 != buttonState3) {
      buttonState3 = reading3;
    }
  }
}*/


  LiquidCrystal lcd(12, 10, 5, 4, 3, 2);

  //button pin initialize
  const int button1pin = 0;
  const int button2pin = 1;
  const int button3pin = 6;

  //ledpin initialize
  const int led1pin = 9;
  const int led2pin = 8;
  const int led3pin = 7;

  //Vars for overall variable types
  int state =0;
  int mode = 0;
  int randomLED =4;
  int randomTime =0;

  //states for each of the LED's 
  int ledState1=LOW;
  int ledState2=LOW;
  int ledState3=LOW;
  int buttonState1=LOW;
  int buttonState2=LOW;
  int buttonState3=LOW;
  
  //for debouncing of the buttons
  unsigned long lastDebounceTime1 =0;
  unsigned long lastDebounceTime2 =0;
  unsigned long lastDebounceTime3 =0;
  unsigned long debouncedelay =50;


void ledOn(int led){
  Serial.print(led2pin);
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

void ledOOF(int led){
   if (led==0){     
    digitalWrite(led1pin,LOW);   
   }   
   else if(led==1){
    digitalWrite(led2pin,LOW);  
   }   
   else if(led==2){     
    digitalWrite(led3pin,LOW);   
   }   
}

void statezero(){
  if(buttonState1==LOW && buttonState2==LOW && buttonState3==LOW){
    state=2;
    lcd.clear();
    lcd.print("    Settings    ");
    lcd.print("   Field Mode   ");
  }
  else if(buttonState2==LOW && buttonState1==HIGH && buttonState3==HIGH ){
      state=5;
      randomSeed(analogRead(0));
      randomLED=random(0,2);
      randomTime=random(5000,12000);
      delay(randomTime);
      ledOn(randomLED);
      int timeLit=millis();
      while(1){
        buttonState1=digitalRead(button1pin);   
        buttonState2=digitalRead(button2pin);   
        buttonState3=digitalRead(button3pin);   


        if(randomLED==0 && buttonState1==LOW && buttonState2==HIGH && buttonState3==HIGH){
          int timepressed=millis();
          if(timepressed-timeLit<=300){
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
          if(timepressed-timeLit<=300){
            state=8;
            break;
          }
          else{
            state=6;
            break;
          }
        }

        else if(randomLED==2 && buttonState1 ==HIGH && buttonState2==HIGH && buttonState3==LOW){
          int timepressed=millis();
          if(timepressed-timeLit<=300){
            state=8;
            break;
          }
          else{
            state=6;
            break;
          }
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

void menuControls(){
  state=2;
  while(1){
    buttonState1=digitalRead(button1pin);   
    buttonState2=digitalRead(button2pin);   
    buttonState3=digitalRead(button3pin);  
    
    if(buttonState1==LOW && buttonState2==HIGH && buttonState3==HIGH){
      if(mode==0){
        lcd.clear();
        lcd.print("    Settings    ");
        lcd.print("    Car Mode    ");
        mode=1;
      }
      else{
        lcd.clear();
        lcd.print("    Settings    ");
        lcd.print("   Field Mode   ");
        mode=0;
      }
      delay(1000);
    }
    else if(buttonState1==HIGH && buttonState2==LOW && buttonState3==HIGH){
      lcd.clear();
      lcd.print("    Settings    ");
      lcd.print("    Saved");
      state=0;
      //delay(1000);
      break;
    }
    else if(buttonState1==HIGH && buttonState2==HIGH && buttonState3==LOW){
      if(mode==0){
        lcd.clear();
        lcd.print("    Settings    ");
        lcd.print("    Car Mode    ");
        mode=1;
      }
      else{
        lcd.clear();
        lcd.print("    Settings    ");
        lcd.print("   Field Mode   ");
        mode=0;
      }
      //delay(1000);
    }
    else if(buttonState1==HIGH && buttonState2==HIGH && buttonState3==HIGH){
      continue;
    }
    else{
      lcd.clear();
      lcd.print("button error");
    }
    
  }
  
}
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Reactionizer!");
  pinMode(button1pin,INPUT_PULLUP);
  pinMode(button2pin,INPUT_PULLUP);
  pinMode(button3pin,INPUT_PULLUP);

  pinMode(led1pin,OUTPUT);
  pinMode(led2pin,OUTPUT);
  pinMode(led3pin,OUTPUT);

  digitalWrite(led1pin,LOW);
  digitalWrite(led2pin,LOW);
  digitalWrite(led3pin,LOW);

  /*States: 
   * 0: Default state, nothing happened
   * 2: All three buttons pressed, go into settings
   * 3: Right button -> go into car mode (flip bool value to True
   * 4: Centre button -> exit settings
   * 5: Press centre button again -> Begin randomizer
   * 6: (delay till led turns on) -> correct button pressed -> calculate the reaction time
   * 7: Wrong button pressed, push error to screen
   * 8: Output the time to the screen, if fast enough, turn on all three led's and output on signal to car if in car mode
   */
}

void loop() {
  // put your main code here, to run repeatedly:
    buttonState1=digitalRead(button1pin);   
    buttonState2=digitalRead(button2pin);   
    buttonState3=digitalRead(button3pin);
  switch(state){
    case(0):
      statezero();
      break;

    case(2):
      menuControls();
      break;

    case(6):
      lcd.clear();
      lcd.print("    TOO SLOW    ");
      lcd.print("   TEST FAILED");
      state=0;
      break;

    case(7):
      lcd.clear();
      lcd.print("  WRONG BUTTON  ");
      lcd.setCursor(0,1);
      lcd.print("  TEST FAILED   ");
      state=0;
      break;

    case(8):
      lcd.clear();
      lcd.print("  TEST PASSED   ");
      
      digitalWrite(led1pin,HIGH);
      digitalWrite(led2pin,HIGH);
      digitalWrite(led3pin,HIGH);
      delay(30000);
      state=9;
      break;
    case(9):
      digitalWrite(led1pin,LOW);
      digitalWrite(led1pin,LOW);
      digitalWrite(led1pin,LOW);
      state=0;
      break;
  }
  
  
  
  
  //lcd.setCursor(0, 1);
  //lcd.print(millis()/1000);

}
