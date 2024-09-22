const int relayPin = 2;  // GPIO 3 is connected to the relay

void setup() {
  // Initialize the relay pin as an output
  pinMode(relayPin, OUTPUT);
  // Start Serial communication at 115200 baud rate
  Serial.begin(115200);
  // Turn the relay OFF initially
  digitalWrite(relayPin, LOW);
  
  Serial.println("Type 'on' to turn the relay ON and 'off' to turn it OFF.");
}

void loop() {
  // Check if data is available in the Serial Monitor
  if (Serial.available() > 0) {
    // Read the incoming string
    String command = Serial.readStringUntil('\n');
    command.trim();  // Remove any extra spaces or newlines

    if (command == "off") {
      // Turn the relay ON
      digitalWrite(relayPin, HIGH);
      Serial.println("Relay is ON");
    }
    else if (command == "on") {
      // Turn the relay OFF
      digitalWrite(relayPin, LOW);
      Serial.println("Relay is OFF");
    }
    else {
      // Handle unrecognized commands
      Serial.println("Invalid command. Type 'on' or 'off'.");
    }
  }
}
