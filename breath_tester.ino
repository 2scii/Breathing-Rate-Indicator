#include <LiquidCrystal.h>

/* constant definitions */
#define CREAD A0
#define NTRIAL 5
#define DTHRESH 100

/* function prototypes */
int analog_average(int, int);
void reset();
void help();
void data(int);
float voltage(int);
void lcd_print(char*, char*);

/* constant definitions */
const int p1 = 4, p2 = 5;
const int d4 = 6, d5 = 7, d6 = 9, d7 = 10;
LiquidCrystal lcd(12, 11, d4, d5, d6, d7);
long conductivity_baseline;

/* body */
void setup() {
  pinMode(p1, INPUT);
  pinMode(p2, INPUT);

  Serial.begin(9600);
  lcd.begin(16,2);

  delay(1000);
  conductivity_baseline = analog_average(CREAD, NTRIAL);
}

void loop() {
  int avg_read = analog_average(CREAD, NTRIAL);

  if (digitalRead(p2) == HIGH) {
    reset();
  } else if (digitalRead(p1) == HIGH) {
    help();
  } else {
    data(avg_read);
    float intermediate = 80 * conductivity_baseline + 20 * avg_read;
    conductivity_baseline = intermediate / 100;
  }

}

/* function definitions */
int analog_average(int pin, int ntrial) {
  int avg_read = 0;

  for (int i = 0; i < ntrial; i++) {
    avg_read += analogRead(pin);
    delay(100);
  }

  return avg_read / ntrial;
}

float voltage(int input) {
  return (5.0 * input / 1023);
}

void reset() {
  lcd_print("Reset activated!", "Setup running...");
  delay(3000);
  setup();
}

void help() {
  const int intermessage_delay = 2000;
  lcd_print("Help Menu", "----------------");
  lcd_print("Breath tester!", "By:");
  delay(intermessage_delay);
  lcd_print("Jim Chen, Sisi", "Jiang, Govind");
  delay(intermessage_delay);
  lcd_print("Suresh", "This project");
  delay(intermessage_delay);
  lcd_print("calculates the", "effect of your");
  delay(intermessage_delay);
  lcd_print("breath on the", "conductivity,");
  delay(intermessage_delay);
  lcd_print("in (V) then in", "a scale 0-1023,");
  delay(intermessage_delay);
  lcd_print("of water!", "----------------");
  
  delay(intermessage_delay);
  lcd_print("Note that initial", "results will be");
  delay(intermessage_delay);
  lcd_print("inaccurate. Wait", "for more accuracy");

  delay(intermessage_delay);
  lcd_print("Controls:", "Button 1: Reset");
  delay(intermessage_delay);
  lcd_print("Button 2: Help", "----------------");

  delay(intermessage_delay);
  lcd_print("Operation:", "Blow through the");
  delay(intermessage_delay);
  lcd_print("mask and read", "the on-screen");
  delay(intermessage_delay);
  lcd_print("information", "----------------");

  delay(intermessage_delay);
  lcd_print("Thank you for", "reading!");
}

void data(int avg_read) {
  const int sdelay = 2000;
  const int ldelay = 3000;
  const int threshold = 2;
  lcd_print("Avg Rate (V):", "");
  lcd.print(voltage(avg_read));
  delay(sdelay);
  lcd_print("Raw Avg Rate:", "");
  lcd.print(avg_read);
  delay(sdelay);
  lcd_print("Baseline (V):", "");
  lcd.print(voltage(conductivity_baseline));
  delay(sdelay);
  lcd_print("Raw Baseline:", "");
  lcd.print(conductivity_baseline);
  delay(sdelay);
  lcd_print("Your score:", "");
  lcd.print(avg_read - conductivity_baseline);
  delay(sdelay);
  
  if (avg_read - conductivity_baseline > threshold) {
    lcd_print("Your score","exceeds");
  } else {
    lcd_print("Your score","didn't meet");
  }

  delay(sdelay);

  lcd_print("The threshold of ", "");
  lcd.print(threshold);
  delay(ldelay);
}

void lcd_print(char* line1, char* line2) {
  lcd.clear();
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}
