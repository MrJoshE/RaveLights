#include <Keyboard.h>
#include <FastLED.h>

#define LED_PIN     3             // Pin 5 does not seem to work on our board.
#define NUM_LEDS    300//259// * 2
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define LED_UPDATE_COUNT 18
#define SNAKE_COUNT NUM_LEDS/LED_UPDATE_COUNT

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

struct Snake {
  CRGB color;
  int front;
  int back;
};

CRGB leds[NUM_LEDS];
Snake snakes[SNAKE_COUNT];

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

int getSnakeLength(Snake* snake){
  return snake->front - snake->back;
}

void nextSnakeFrame(Snake* snake){

  if(snake->back == NUM_LEDS){
    snake->back = -1;
    snake->front = -1;
  }

  if(snake->front < 0 || snake->back < 0){
    return;
  }
  
  if(getSnakeLength(snake) >= LED_UPDATE_COUNT || snake->front >= NUM_LEDS - 1){
    leds[snake->back] = 0x000000;
    snake->back++;
  }
  
  if(snake->front < NUM_LEDS - 1){
    snake->front++;
    leds[snake->front] = snake->color;
  }
    
  //FastLED.show();
  
}

void createSnake(CRGB color = 0xFFFFFF) {
  for (int i = 0; i < SNAKE_COUNT; i++) {
    Snake* snake = &snakes[i];
    
    if (snake->front < 0 && snake->back < 0) {
      snake->color = color;
      snake->front = 0;
      snake->back = 0;
      return;
    }
  }
}

/*
 * I present, the world's most complex function:
 * snek creator.
 */
void summonSnek(CRGB color = 0xFFFFFF) { createSnake(color); }


void checkInput() {
  
  if (Serial.available()) {
    // String command = Serial.readString();
    // processCommand(command);
    uint8_t hue = (uint8_t) Serial.read();
    summonSnek(CHSV(hue, 255, 255));
  }
  
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


void setup() { 

  Serial.begin(230400);

  
  FastLED
    .addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection( TypicalLEDStrip );

  FastLED.setBrightness(BRIGHTNESS);


  for(int i = 0; i < SNAKE_COUNT; i++){
    snakes[i] = {0xFFFFFF, -1,-1};
    
  }
  

  delay(200);

  for(int j = 0; j < NUM_LEDS; j++){
    leds[j] = 0x000000;
  }

  FastLED.show();
 
}


void loop() {
  
  checkInput();

  for(int i = 0; i < SNAKE_COUNT; i++){
    Snake* snake = &snakes[i];
    
    if(snake->front >= 0 && snake->back >= 0){
      nextSnakeFrame(snake);
    }
  }
  
  FastLED.show();
  delay(1);

}


void processCommand(String command) {

  if (command[0] == '!') {
    int input = (command.substring(1)).toInt();
    summonSnek(getColor(input));
  }
  
}



CHSV getColor(int input){

  input = min(1500, input);

  int hue = round(map(input, 0, 1500, 0, 255));

  CHSV color = CHSV(hue, 255, 255);
  return color;
  
}
