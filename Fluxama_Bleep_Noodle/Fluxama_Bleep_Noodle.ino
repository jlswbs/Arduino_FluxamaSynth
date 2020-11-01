// Fluxama-Synth bleep noodle generator //

#include "Fluxamasynth.h"

Fluxamasynth synth;

/*

 Based Logistic equation

 Pot0 = chain note
 Pot1 = chain instrument
 Pot2 = chaos constant
 Pot3 = tempo/speed 45 - 350 BPM 16th note
 Pot4 = none
 Pot5 = none
 
 Created by JLS 2020

*/

#define MAXTEMPO 1   // 14 = 350 BPM 16th note
#define MINTEMPO 111 // 45 BPM 16th note
#define MAXADC  1023 // max ADC value
#define VOLUME 18    // Master volume

  float r;
  float x = 0.2f;

  uint8_t xout = 0;
  uint8_t prog = 0;
  uint8_t bank = 0;
  uint8_t delay_ms = 0;


void setup(){

  synth.midiReset();

  //synth.setMasterVolume(30);

  synth.setChannelVolume(0, VOLUME); // volume chan.1

  synth.setReverb(0, 7, 100, 100); // pan delay chan.1
   
}


void loop() {

  r = map(analogRead(2), 0, MAXADC, 35000, 39900);
  r /= 10000.0f;
  
  float nx = x;
  x = r * nx * (1.0f - nx);
            
  xout = 128 * x;

  prog = analogRead(1) * xout / analogRead(1);

  if (prog == 19) prog = 0; // replace church organ to grand piano

  // if (rand()%2 == 1) bank = 127; else bank = 0; // random bank selection
  
  synth.programChange(bank, 0, prog & 127); // program change chan.1

  synth.pan(0, (32+(xout/2))&127); // panning chan.1
  
  synth.noteOn(0, (36 + (analogRead(0) * xout/analogRead(0)))&127, (32+(xout/2))&127);

  delay_ms = map(analogRead(3), 0, MAXADC, MINTEMPO, MAXTEMPO);

  delay (delay_ms);

  synth.noteOff (0, (36 + (analogRead(0) * xout/analogRead(0)))&127);

  //synth.allNotesOff (0); // all sound off chan.1

  delay (delay_ms);
   
}
