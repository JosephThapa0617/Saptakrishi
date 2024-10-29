volatile bool dataReceived = false;  // Volatile flag to signal data reception
String receivedData = "";            // Regular String, not volatile

unsigned long lastDebounceTime = 0;  // To store the last time the button was pressed
const unsigned long debounceDelay = 200;  // Debounce time in milliseconds

void IRAM_ATTR handleButtonPress() {
  unsigned long currentTime = millis();  // Get the current time

  // Debounce: Only register the button press if enough time has passed since the last press
  if ((currentTime - lastDebounceTime) > debounceDelay) {
    dataReceived = true;  // Set the flag when a valid button press occurs
    lastDebounceTime = currentTime;  // Update the last debounce time
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(21, INPUT_PULLUP);  // Assume pin 27 is for button

  // Attach an interrupt to the button pin (FALLING edge indicates button press)
  attachInterrupt(digitalPinToInterrupt(21), handleButtonPress, FALLING);
}

void loop() {
  // Check if dataReceived flag is set by the ISR
  if (dataReceived) {
    receivedData = "Button was pressed!";
    Serial.println(receivedData);

    // Reset the flag after handling
    dataReceived = false;
  }

  delay(100);  // Adjust the delay based on your needs
}
