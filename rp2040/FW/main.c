// swantroller FW
// Author: zwenergy
#include "pico/stdlib.h"

// Pin definitions.
// WS inputs.
#define X1 11
#define X2 12
#define X3 14
#define X4 13
#define Y1 6
#define Y2 5
#define Y3 10
#define Y4 9
#define A 29
#define B 28
#define START 26
#define POWER 27
#define SOUND 15

// SNES controller.
#define SNES_DAT 0
#define SNES_LATCH 1
#define SNES_CLK 2

// Adapted SNES controller "protocol".
// We send UP and DOWN pressed to identify our
// "special" controller.
#define SNES_DPAD_UP 4
#define SNES_DPAD_DOWN 5

#define SNES_WSC_X1 0
#define SNES_WSC_X2 1
#define SNES_WSC_X3 2
#define SNES_WSC_X4 3
#define SNES_WSC_Y1 6
#define SNES_WSC_Y2 7
#define SNES_WSC_Y3 8
#define SNES_WSC_Y4 9
#define SNES_WSC_A 10
#define SNES_WSC_B 11
#define SNES_WSC_START 12
#define SNES_WSC_SOUND 13
#define SNES_WSC_POWER 14

// Global vars.
unsigned int snes_buttons[16];

void preparePins() {
  // All WSC inputs should use a pull up.
  gpio_init( X1 );
  gpio_set_dir( X1, GPIO_IN );
  gpio_pull_up( X1 );
  
  gpio_init( X2 );
  gpio_set_dir( X2, GPIO_IN );
  gpio_pull_up( X2 );
  
  gpio_init( X3 );
  gpio_set_dir( X3, GPIO_IN );
  gpio_pull_up( X3 );
  
  gpio_init( X4 );
  gpio_set_dir( X4, GPIO_IN );
  gpio_pull_up( X4 );
  
  gpio_init( Y1 );
  gpio_set_dir( Y1, GPIO_IN );
  gpio_pull_up( Y1 );
  
  gpio_init( Y2 );
  gpio_set_dir( Y2, GPIO_IN );
  gpio_pull_up( Y2 );
  
  gpio_init( Y3 );
  gpio_set_dir( Y3, GPIO_IN );
  gpio_pull_up( Y3 );
  
  gpio_init( Y4 );
  gpio_set_dir( Y4, GPIO_IN );
  gpio_pull_up( Y4 );
  
  gpio_init( A );
  gpio_set_dir( A, GPIO_IN );
  gpio_pull_up( A );
  
  gpio_init( B );
  gpio_set_dir( B, GPIO_IN );
  gpio_pull_up( B );
  
  gpio_init( START );
  gpio_set_dir( START, GPIO_IN );
  gpio_pull_up( START );
  
  gpio_init( SOUND );
  gpio_set_dir( SOUND, GPIO_IN );
  gpio_pull_up( SOUND );
  
  gpio_init( POWER );
  gpio_set_dir( POWER, GPIO_IN );
  gpio_pull_up( POWER );

  gpio_init( SNES_CLK );
  gpio_set_dir( SNES_CLK, GPIO_IN );
  
  gpio_init( SNES_LATCH );
  gpio_set_dir( SNES_LATCH, GPIO_IN );

  // SNES controller outputs.
  gpio_init( SNES_DAT );
  gpio_set_dir( SNES_DAT, GPIO_OUT );
  gpio_put( SNES_DAT, 1 );
}

void readWSC() {
  // SNES DPAD UP and DPAD DOWN are always "pressed".
  snes_buttons[ SNES_DPAD_DOWN ] = 0;
  snes_buttons[ SNES_DPAD_UP ] = 0;
  
  // Read pad.
  unsigned int tmp = gpio_get_all();
  
  // Process buttons.
  // A
  snes_buttons[ SNES_WSC_A ] = ( tmp >> ( A ) ) & 1;
  // B
  snes_buttons[ SNES_WSC_B ] = ( tmp >> ( B ) ) & 1;
  // X3
  snes_buttons[ SNES_WSC_X3 ] = ( tmp >> ( X3 ) ) & 1;
  // X2
  snes_buttons[ SNES_WSC_X2 ] = ( tmp >> ( X2 ) ) & 1;
  // X4
  snes_buttons[ SNES_WSC_X4 ] = ( tmp >> ( X4 ) ) & 1;

  // X1
  snes_buttons[ SNES_WSC_X1 ] = ( tmp >> ( X1 ) ) & 1;
  // Y1
  snes_buttons[ SNES_WSC_Y1 ] = ( tmp >> ( Y1 ) ) & 1;
  // Y4
  snes_buttons[ SNES_WSC_Y4 ] = ( tmp >> ( Y4 ) ) & 1;
  // Y2
  snes_buttons[ SNES_WSC_Y2 ] = ( tmp >> ( Y2 ) ) & 1;
  // Y3
  snes_buttons[ SNES_WSC_Y3 ] = ( tmp >> ( Y3  ) ) & 1;
  
  // SOUND
  snes_buttons[ SNES_WSC_SOUND ] = ( tmp >> ( SOUND ) ) & 1;

  // START
  snes_buttons[ SNES_WSC_START ] = ( tmp >> ( START ) ) & 1;
  // POWER
  snes_buttons[ SNES_WSC_POWER ] = ( tmp >> ( POWER ) ) & 1;  
}

void handleSNES() {
  // Blocking function which awaits SNES clocks and answers accordingly.
  unsigned int cnt = 0;
  unsigned int lastClk = 1;

  // Output first data bit.
  if ( snes_buttons[ cnt ] ) {
    gpio_put( SNES_DAT, 1 );
  } else {
    gpio_put( SNES_DAT, 0 );
  }

  // We only care to output 15 bits.
  while ( cnt < 15 ) {
    // Read SNES clock.
    unsigned int curClk = gpio_get( SNES_CLK );

    // Rising clock?
    if ( curClk && !lastClk ) {
      // Increase cnt.
      ++cnt;

      // Output new data bit.
      if ( snes_buttons[ cnt ] ) {
        gpio_put( SNES_DAT, 1 );
      } else {
        gpio_put( SNES_DAT, 0 );
      }
    }
    
    lastClk = curClk;
  }
}

void waitForLatchRise() {
  unsigned int lastLatch = 0;

  while ( 1 ) {
    unsigned int curLatch = gpio_get( SNES_LATCH );

    // Rising Latch?
    if ( curLatch && !lastLatch ) {
      break;
    }

    lastLatch = curLatch;
  }
}

int main() {
  preparePins();
  
  // Simple loop.
  while ( 1 ) {
    // Read WSC buttons.
    readWSC();
    // Wait for Latch signal rise.
    waitForLatchRise();
    // Do SNES communication.
    handleSNES();
  }
}
