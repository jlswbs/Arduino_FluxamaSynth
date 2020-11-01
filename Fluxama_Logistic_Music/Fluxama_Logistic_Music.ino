// Fluxama-Synth Logistic music generator //

#include "Fluxamasynth.h"

Fluxamasynth synth;

/*

 Based Logistic equation

 Pot0 = chain note A
 Pot1 = chain note B
 Pot2 = chain instrument
 Pot3 = chaos constant
 Pot4 = morph drum
 Pot5 = none // tempo/speed 45 - 350 BPM 16th note

 Channel 1-4 voices
 Channel 10 drums
 
 Created by JLS 2020

*/

#define MAXTEMPO  14 // 350 BPM 16th note
#define MINTEMPO 111 // 45 BPM 16th note
#define MAXADC  1023 // max ADC value
#define VOLUME 18    // Master volume

  float r;
  float x = 0.2f;

  uint8_t xout = 0;
  uint8_t rel_off = 0;
  uint8_t delay_ms = 0;


void setup(){

  synth.midiReset();

  // synth.setMasterVolume(25);

  synth.setChannelVolume(0, VOLUME); // Voice 1
  synth.setChannelVolume(1, VOLUME); // Voice 2
  synth.setChannelVolume(2, VOLUME); // Voice 3
  synth.setChannelVolume(3, VOLUME); // Voice 4

  synth.setChannelVolume(9, VOLUME); // Drums

  synth.pan(0, 32); // panning chan.1
  synth.pan(1, 80); // panning chan.2
  synth.pan(2, 48); // panning chan.3
  synth.pan(3, 96); // panning chan.4

  synth.setReverb(0, 3, 64, 25); // reverb chan.1
  synth.setReverb(1, 3, 64, 25); // reverb chan.2
  synth.setReverb(2, 3, 64, 25); // reverb chan.3
  synth.setReverb(3, 3, 64, 25); // reverb chan.4
   
}


void loop() {

  r = map(analogRead(3), 0, MAXADC, 35000, 39900);
  r /= 10000.0f;
  
  float nx = x;
  x = r * nx * (1.0f - nx);
            
  xout = 128 * x;
  
  uint8_t note_a = 24 + ((analogRead(0) * xout / analogRead(0)) / 2)%84;
  uint8_t note_b = 36 + ((analogRead(1) * xout / analogRead(1)) / 2)%72;
  uint8_t note_c = map(note_a ^ note_b, 0, 127, 36, 84);
  uint8_t note_d = map(note_a & note_b, 0, 127, 24, 72);
  uint8_t drum = map(analogRead(4) * x, 0, MAXADC, 35, 81);
  uint8_t prog = (analogRead(2) * xout / analogRead(2));

  if (prog == 19) prog = 0; // replace church organ to grand piano 

  synth.programChange (0, rand()%4, prog & 127);
  
  synth.noteOn(0, note_a & 127, 24 + rand()%48);
  synth.noteOn(1, note_b & 127, 24 + rand()%48);
  synth.noteOn(2, note_c & 127, 24 + rand()%48);
  synth.noteOn(3, note_d & 127, 24 + rand()%48);
  
  synth.noteOn(9, drum & 127, 48 + rand()%48);
 
  delay_ms = MINTEMPO; //map(analogRead(5), 0, MAXADC, MINTEMPO, MAXTEMPO); 

  delay (delay_ms);

  if (rel_off == (1 + rand()%4)){
    
    rel_off = 0;

   synth.allNotesOff (0); // all sound off chan.1
   synth.allNotesOff (1); // all sound off chan.2
   synth.allNotesOff (2); // all sound off chan.3
   synth.allNotesOff (3); // all sound off chan.4
  
  }

  rel_off++;
  
  delay (2 * delay_ms);
   
}
