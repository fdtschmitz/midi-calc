
extern bool serialCommandReady;
extern String serialBuffer;
extern ChordData chords[7];


// Function declarations
void processSerialCommand(String command);
void handleSerialCommunication();
void loadChordConfig();
void saveChordConfig();


// SERIAL COMMUNICATION FUNCTIONS
void processSerialCommand(String command) {
  command.trim();
  
  if (command == "GET_CHORDS") {
    // Enviar todos os acordes
    for (int i = 0; i < 7; i++) {
      Serial.print("CHORD_DATA:");
      Serial.print(i);
      Serial.print(",");
      
      for (int j = 0; j < chords[i].noteCount; j++) {
        Serial.print(chords[i].notes[j]);
        if (j < chords[i].noteCount - 1) Serial.print(",");
      }
      Serial.print(",");
      Serial.println(chords[i].octave);
    }
    Serial.println("CHORDS_SENT");
    
  } else if (command.startsWith("SET_CHORD:")) {
    // Formato: SET_CHORD:index,note1,note2,note3,octave
    String data = command.substring(10);
    int firstComma = data.indexOf(',');
    
    if (firstComma > 0) {
      int chordIndex = data.substring(0, firstComma).toInt();
      
      if (chordIndex >= 0 && chordIndex < 7) {
        String noteData = data.substring(firstComma + 1);
        
        // Parse notes and octave
        int noteCount = 0;
        int notes[8];
        int octave = 4;
        
        int startPos = 0;
        int commaPos = noteData.indexOf(',');
        
        // Parse notes
        while (commaPos != -1 && noteCount < 8) {
          String noteStr = noteData.substring(startPos, commaPos);
          if (noteStr.length() > 0) {
            int note = noteStr.toInt();
            if (note >= 0 && note <= 11) {
              notes[noteCount++] = note;
            }
          }
          startPos = commaPos + 1;
          commaPos = noteData.indexOf(',', startPos);
        }
        
        // Last part should be octave
        String lastPart = noteData.substring(startPos);
        if (lastPart.length() > 0) {
          octave = lastPart.toInt();
          if (octave < 2 || octave > 6) octave = 4;
        }
        
        // Update chord
        chords[chordIndex].noteCount = noteCount;
        chords[chordIndex].octave = octave;
        for (int i = 0; i < noteCount; i++) {
          chords[chordIndex].notes[i] = notes[i];
        }
        
        Serial.print("CHORD_SET:");
        Serial.println(chordIndex);
      }
    }
    
  } else if (command == "SAVE_CONFIG") {
    // Salvar configuração na EEPROM
    saveChordConfig();
    Serial.println("CONFIG_SAVED");
    
  } else if (command == "LOAD_CONFIG") {
    // Carregar configuração da EEPROM
    loadChordConfig();
    Serial.println("CONFIG_LOADED");
    
  } else {
    Serial.println("UNKNOWN_COMMAND");
  }
}

void handleSerialCommunication() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    
    if (c == '\n' || c == '\r') {
      if (serialBuffer.length() > 0) {
        processSerialCommand(serialBuffer);
        serialBuffer = "";
      }
    } else {
      serialBuffer += c;
    }
  }
}


void saveChordConfig() {
  ChordConfig config;
  config.magic = CHORD_CONFIG_MAGIC;
  
  for (int i = 0; i < 7; i++) {
    config.chords[i] = chords[i];
  }
  
  EEPROM.put(CHORD_CONFIG_START_ADDR, config);
}

void loadChordConfig() {
  ChordConfig config;
  EEPROM.get(CHORD_CONFIG_START_ADDR, config);
  
  if (config.magic == CHORD_CONFIG_MAGIC) {
    for (int i = 0; i < 7; i++) {
      chords[i] = config.chords[i];
    }
    Serial.println("CHORD_CONFIG_LOADED");
  } else {
    // Use default chords if no saved config
    Serial.println("NO_SAVED_CONFIG_USING_DEFAULTS");
  }
}