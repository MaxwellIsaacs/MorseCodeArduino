#define MAIN_LED_PIN 9                // pin for main led, morse code here
#define START_BUTTON_PIN 3            // start/restart/pause/unpause button
#define GREEN_LED_PIN 10              // green led indicates start
#define RED_LED_PIN 11                // red led shows we're done
#define POTENTIOMETER_PIN A0          // potentiometer for speed control

// timing for morse code elements, tweak these for different speeds
#define BASE_DOT_TIME 250
#define BASE_DASH_TIME 750
#define BASE_BETWEEN_ELEMENTS_TIME 250
#define BASE_BETWEEN_LETTERS_TIME 750
#define BASE_BETWEEN_WORDS_TIME 1500

unsigned long lastButtonPress = 0;    // tracks last button press to debounce
bool isPaused = false;                // is the output paused?
bool isPlaying = false;               // are we currently playing morse?

void setup() {
  pinMode(MAIN_LED_PIN, OUTPUT);
  pinMode(START_BUTTON_PIN, INPUT_PULLUP); // use internal pullup for button
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(POTENTIOMETER_PIN, INPUT);
  
  // start with both leds off
  digitalWrite(GREEN_LED_PIN, LOW);   
  digitalWrite(RED_LED_PIN, LOW);
  
  Serial.begin(9600);                 // for debugging
}

void loop() {

  // button toggles play/pause
  if (digitalRead(START_BUTTON_PIN) == LOW) {
    unsigned long currentTime = millis();
    // debounce logic
    if (currentTime - lastButtonPress > 500) { 
      // start or unpause
      if (!isPlaying || isPaused) {
        isPlaying = true;
        isPaused = false;
        // visual feedback
        digitalWrite(GREEN_LED_PIN, HIGH);
        digitalWrite(RED_LED_PIN, LOW);
        lastButtonPress = currentTime;
      } else if (!isPaused) { // pause if playing
        isPaused = true;
        lastButtonPress = currentTime;
      }
    }
  }

  // play the message
  if (isPlaying && !isPaused) {
    playMorseCode("HELLOWORLD");
    isPlaying = false; // done with the message
    // show we're done
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
    // wait a bit before turning off the red led
    delay(readTimingValue(BASE_BETWEEN_WORDS_TIME)); 
    digitalWrite(RED_LED_PIN, LOW);
  }
}

void playMorseCode(String code) {
  for (unsigned int i = 0; i < code.length(); i++) {
    if (isPaused) break; // stop if paused

    switch (code[i]) {
      case 'A': dot(); dash(); break;
      case 'B': dash(); dot(); dot(); dot(); break;
      case 'C': dash(); dot(); dash(); dot(); break;
      case 'D': dash(); dot(); dot(); break;
      case 'E': dot(); break;
      case 'F': dot(); dot(); dash(); dot(); break;
      case 'G': dash(); dash(); dot(); break;
      case 'H': dot(); dot(); dot(); dot(); break;
      case 'I': dot(); dot(); break;
      case 'J': dot(); dash(); dash(); dash(); break;
      case 'K': dash(); dot(); dash(); break;
      case 'L': dot(); dash(); dot(); dot(); break;
      case 'M': dash(); dash(); break;
      case 'N': dash(); dot(); break;
      case 'O': dash(); dash(); dash(); break;
      case 'P': dot(); dash(); dash(); dot(); break;
      case 'Q': dash(); dash(); dot(); dash(); break;
      case 'R': dot(); dash(); dot(); break;
      case 'S': dot(); dot(); dot(); break;
      case 'T': dash(); break;
      case 'U': dot(); dot(); dash(); break;
      case 'V': dot(); dot(); dot(); dash(); break;
      case 'W': dot(); dash(); dash(); break;
      case 'X': dash(); dot(); dot(); dash(); break;
      case 'Y': dash(); dot(); dash(); dash(); break;
      case 'Z': dash(); dash(); dot(); dot(); break;
      default: break; // In case of an unknown character
    }

    if (i < code.length() - 1) delay(readTimingValue(BASE_BETWEEN_LETTERS_TIME)); // Delay between letters, but not after the last one
  }
}


void dot() {
  // turn on, wait, turn off, wait a bit
  digitalWrite(MAIN_LED_PIN, HIGH);
  delay(readTimingValue(BASE_DOT_TIME));
  digitalWrite(MAIN_LED_PIN, LOW);
  delay(readTimingValue(BASE_BETWEEN_ELEMENTS_TIME));
}

void dash() {
  // same as dot but longer
  digitalWrite(MAIN_LED_PIN, HIGH);
  delay(readTimingValue(BASE_DASH_TIME));
  digitalWrite(MAIN_LED_PIN, LOW);
  delay(readTimingValue(BASE_BETWEEN_ELEMENTS_TIME));
}

unsigned long readTimingValue(unsigned long baseTime) {
  // read potentiometer, adjust timing
  int sensorValue = analogRead(POTENTIOMETER_PIN);
  // scale speed between 1x and 4x
  float multiplier = map(sensorValue, 0, 1023, 1, 4); 
  return (unsigned long)(baseTime * multiplier / 10);
}
