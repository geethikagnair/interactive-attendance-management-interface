#include <Keypad.h>
#include <LiquidCrystal.h>

const byte ROWS = 4;
const byte COLS = 4;
LiquidCrystal lcd(15, 14, 13, 12, 11, 10);
int led = 16; // Single LED for feedback

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
String v_passcode = ""; // Current input passcode
unsigned long lastInputTime = millis();
const unsigned long timeoutDuration = 5000; // 5 seconds timeout
// User structure
struct User {
  String name;
  String passcode;
};

// User database
User users[] = {
  {"Alice", "123A"},
  {"Bob", "456B"},
  {"Charlie", "789C"},
  {"David", "234A"},
  {"Eva", "567B"},
  {"Frank", "890C"},
  {"Grace", "345D"},
  {"Hannah", "678A"},
  {"Ian", "901B"},
  {"Julia", "234C"},
  {"Kevin", "567D"},
  {"Liam", "890A"},
  {"Mia", "123B"},
  {"Noah", "456C"},
  {"Olivia", "789D"},
  {"Parker", "012A"},
  {"Quinn", "345B"},
  {"Ryan", "678C"},
  {"Sophia", "901D"},
  {"Tess", "234B"},
  {"Uma", "567C"},
  {"Vera", "890D"},
  {"Will", "123C"},
  {"Xena", "456D"},
  {"Yara", "789A"},
  {"Zara", "012B"}
};

const int userCount = sizeof(users) / sizeof(users[0]);

// Attendance tracking variables
int presentCount = 0;
int absentCount = userCount; // Initially, everyone is absent

void setup() {
  lcd.begin(16, 2);
  pinMode(led, OUTPUT);
  lcd.print("Enter Passcode:");
}

void loop() {
  char key = customKeypad.getKey();
  if (key) {
    lastInputTime = millis(); // Reset the timer

    if (key == '*') {
      v_passcode = "";
      lcd.setCursor(12, 0);
      lcd.print("        "); // Clear display
      lcd.setCursor(0, 1);
      lcd.print("Passcode Cleared ");
      delay(1000); // Pause to show the message
      lcd.setCursor(0, 1);
      lcd.print("                "); // Clear the message
    } else if (v_passcode.length() < 4) {
      v_passcode += key;
      lcd.setCursor(12, 0);
      lcd.print(v_passcode);
    }

    // Check if the user confirms their entry
    if (key == '#') {
      bool accessGranted = false;
      String userName = "";

      for (int i = 0; i < userCount; i++) {
        if (v_passcode == users[i].passcode) {
          userName = users[i].name;
          accessGranted = true;
          break;
        }
      }

      if (accessGranted) {
        lcd.setCursor(0, 1);
        lcd.print(userName + " Present");
        digitalWrite(led, HIGH); // Turn on LED for valid entry
        presentCount++; // Increment present count
        absentCount = userCount - presentCount; // Update absent count
        delay(2000); // Keep the LED on while displaying "Present"
        digitalWrite(led, LOW); // Turn off LED after delay
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Invalid ID    ");
        for (int i = 0; i < 3; i++) { // Blink LED for invalid entry
          digitalWrite(led, HIGH);
          delay(200);
          digitalWrite(led, LOW);
          delay(200);
        }
      }

      // Reset passcode after confirmation
      v_passcode = "";
      delay(2000); // Delay to show result

      // Clear the LCD screen and show the prompt again
      lcd.clear();
      lcd.print("Enter Passcode:");

      // Display total present and absent counts
      lcd.setCursor(0, 0);
      lcd.print("P: ");
      lcd.print(presentCount);
      lcd.print(" A: ");
      lcd.print(absentCount);
    }

    // Display current input length
    lcd.setCursor(0, 1);
    lcd.print("Length: ");
    lcd.print(v_passcode.length());
  }

  // Check for timeout
  if (millis() - lastInputTime > timeoutDuration) {
    v_passcode = "";
    lcd.setCursor(12, 0);
    lcd.print("        "); // Clear display
  }
}
