// Rotary Encoder Inputs
#define CLK 18  // Use GPIO 18 for CLK
#define DT 19   // Use GPIO 19 for DT
#define SW 21   // Use GPIO 21 for SW

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;

void setup() {
    // Set encoder pins as inputs
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    pinMode(SW, INPUT_PULLUP);

    // Setup Serial Monitor
    Serial.begin(115200);

    // Read the initial state of CLK
    lastStateCLK = digitalRead(CLK);
}

void loop() {
    // Read the current state of CLK
    currentStateCLK = digitalRead(CLK);

    // If last and current state of CLK are different, then pulse occurred
    // React to only 1 state change to avoid double count
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1) {

        // If the DT state is different than the CLK state then
        // the encoder is rotating CCW so decrement
        if (digitalRead(DT) != currentStateCLK) {
            counter--;
            currentDir = "CCW";
        } else {
            // Encoder is rotating CW so increment
            counter++;
            currentDir = "CW";
        }

        Serial.print("Direction: ");
        Serial.print(currentDir);
        Serial.print(" | Counter: ");
        Serial.println(counter);
    }

    // Remember last CLK state
    lastStateCLK = currentStateCLK;

    // Read the button state
    int btnState = digitalRead(SW);

    // If we detect LOW signal, button is pressed
    if (btnState == LOW) {
        // If 50ms have passed since last LOW pulse, it means that the
        // button has been pressed, released and pressed again
        if (millis() - lastButtonPress > 50) {
            Serial.println("Button pressed!");
        }

        // Remember last button press event
        lastButtonPress = millis();
    }

    // Slight delay to help debounce the reading
    delay(1);
}