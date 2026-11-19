#include <Wire.h>
#include <LCD-I2C.h>
#include <EEPROM.h>

LCD_I2C lcd(0x27, 16, 2);

// ---------------- BUTTON PINS ----------------
const byte button1 = 2;
const byte button2 = 3;
const byte button3 = 4;

// ---------------- LED PINS ----------------
const byte voteLED = 12;
const byte statusLED = 13;

// ---------------- VOTE COUNTERS ----------------
int votes1 = 0;
int votes2 = 0;
int votes3 = 0;

// ---------------- FSM STATES ----------------
enum State {
  IDLE,
  VOTE,
  CONFIRM,
  RESULTS
};

State currentState = IDLE;

// ---------------- FSM VARIABLES ----------------
byte selectedCandidate = 0;
unsigned long stateStartTime = 0;

// ---------------- DEBOUNCING ----------------
const byte buttonPins[3] = {button1, button2, button3};

bool lastReading[3] = {HIGH, HIGH, HIGH};
bool stableState[3] = {HIGH, HIGH, HIGH};

unsigned long lastDebounceTime[3] = {0, 0, 0};
const unsigned long debounceDelay = 50;

// ---------------- EEPROM ADDRESSES ----------------
const int addr1 = 0;
const int addr2 = 2;
const int addr3 = 4;
const int signatureAddr = 6;

const byte EEPROM_SIGNATURE = 0xA5;

// ---------------- TIMING ----------------
const unsigned long confirmDuration = 1000;
const unsigned long resultsDuration = 2500;
const unsigned long idleResultsInterval = 8000;

unsigned long lastResultsTime = 0;

// ---------------- FUNCTION DECLARATIONS ----------------
void loadVotes();
void saveVotes();
bool buttonPressed(byte index);
void showReady();
void showConfirmation();
void showVoteCounts();
void processState();
void enterState(State newState);

// =================================================
// EEPROM FUNCTIONS
// =================================================

void loadVotes() {

  if (EEPROM.read(signatureAddr) != EEPROM_SIGNATURE) {

    votes1 = 0;
    votes2 = 0;
    votes3 = 0;

    saveVotes();
    return;
  }

  EEPROM.get(addr1, votes1);
  EEPROM.get(addr2, votes2);
  EEPROM.get(addr3, votes3);

  // Validate stored values
  if (votes1 < 0 || votes1 > 10000) votes1 = 0;
  if (votes2 < 0 || votes2 > 10000) votes2 = 0;
  if (votes3 < 0 || votes3 > 10000) votes3 = 0;

  saveVotes();
}

void saveVotes() {

  EEPROM.put(addr1, votes1);
  EEPROM.put(addr2, votes2);
  EEPROM.put(addr3, votes3);

  EEPROM.write(signatureAddr, EEPROM_SIGNATURE);
}

// =================================================
// BUTTON DEBOUNCING
// =================================================

bool buttonPressed(byte index) {

  bool reading = digitalRead(buttonPins[index]);

  if (reading != lastReading[index]) {
    lastDebounceTime[index] = millis();
    lastReading[index] = reading;
  }

  if (millis() - lastDebounceTime[index] >= debounceDelay) {

    if (reading != stableState[index]) {

      stableState[index] = reading;

      // INPUT_PULLUP: LOW means pressed
      if (stableState[index] == LOW) {
        return true;
      }
    }
  }

  return false;
}

// =================================================
// LCD FUNCTIONS
// =================================================

void showReady() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Voting Machine");

  lcd.setCursor(0, 1);
  lcd.print("Select Candidate");
}

void showConfirmation() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vote Recorded");

  lcd.setCursor(0, 1);
  lcd.print("Candidate ");
  lcd.print(selectedCandidate);
}

void showVoteCounts() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("C1:");
  lcd.print(votes1);

  lcd.print(" C2:");
  lcd.print(votes2);

  lcd.setCursor(0, 1);
  lcd.print("C3:");
  lcd.print(votes3);

}

// =================================================
// FSM STATE TRANSITION
// =================================================

void enterState(State newState) {

  currentState = newState;
  stateStartTime = millis();

  switch (currentState) {

    case IDLE:
      showReady();
      digitalWrite(voteLED, LOW);
      digitalWrite(statusLED, LOW);
      break;

    case VOTE:
      digitalWrite(voteLED, HIGH);
      digitalWrite(statusLED, HIGH);
      break;

    case CONFIRM:
      showConfirmation();
      break;

    case RESULTS:
      showVoteCounts();
      digitalWrite(voteLED, LOW);
      digitalWrite(statusLED, HIGH);
      break;
  }
}

// =================================================
// FSM PROCESSING
// =================================================

void processState() {

  switch (currentState) {

    // ---------------------------------------------
    // IDLE STATE
    // ---------------------------------------------
    case IDLE:

      if (button
