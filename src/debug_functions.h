/*
 * debug_functions.h - Funções de Debug para o Controlador MIDI
 *
 * Este arquivo contém funções para auxiliar na depuração do projeto,
 * permitindo verificar a leitura dos botões e a comunicação MIDI.
 */

#ifndef DEBUG_FUNCTIONS_H
#define DEBUG_FUNCTIONS_H

#include <Arduino.h>
#include "config.h"
#include "modes.h"

// Flag para ativar/desativar as funções de debug
const bool ENABLE_DEBUG = true; // Defina como 'false' para desativar o debug

// Função para imprimir o estado dos botões no Serial Monitor
void debugPrintButtonStates() {
  if (!ENABLE_DEBUG) return;
  Serial.println("--- Button States ---");
  for (int i = 0; i < numNoteButtons; i++) {
    Serial.print("Note Button ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(digitalRead(notePins[i]) == LOW ? "PRESSED" : "RELEASED");
  }
  Serial.print("Sharp Button: ");
  Serial.println(digitalRead(sharpPin) == LOW ? "PRESSED" : "RELEASED");
  Serial.print("Octave Down Button: ");
  Serial.println(digitalRead(octaveDownPin) == LOW ? "PRESSED" : "RELEASED");
  Serial.print("Octave Up Button: ");
  Serial.println(digitalRead(octaveUpPin) == LOW ? "PRESSED" : "RELEASED");
  Serial.print("Mode Button: ");
  Serial.println(digitalRead(modePin) == LOW ? "PRESSED" : "RELEASED");
  Serial.println("---------------------");
}

// Função para depurar o envio de mensagens MIDI
void debugPrintMidiMessage(const char* type, int channel, int note, int velocity) {
  if (!ENABLE_DEBUG) return;
  Serial.print("MIDI ");
  Serial.print(type);
  Serial.print(" - Ch: ");
  Serial.print(channel + 1); // Canais MIDI são 1-indexed
  Serial.print(", Note: ");
  Serial.print(note);
  Serial.print(", Vel: ");
  Serial.println(velocity);
}

#endif // DEBUG_FUNCTIONS_H


