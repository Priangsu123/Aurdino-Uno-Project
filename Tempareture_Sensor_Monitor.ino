// Arduino Uno: Temperature-controlled Fan with 10k NTC Thermistor

const int sensorPin = A0;   // NTC Thermistor on A0
const int fanPin = 9;       // Use PWM pin for fan (instead of D13 which is not PWM)
                            // If you only want ON/OFF with LED, use D13

// Steinhart-Hart parameters for 10k NTC
const float BETA = 3950;    // Beta coefficient
const float R0 = 10000.0;   // Resistance at 25°C
const float T0 = 298.15;    // 25°C in Kelvin
const float SERIES_RESISTOR = 10000.0; // 10k pull-up resistor

void setup() {
  Serial.begin(9600);
  pinMode(fanPin, OUTPUT);
}

float readTemperature() {
  int adcValue = analogRead(sensorPin);
  float voltage = adcValue * (5.0 / 1023.0);
  float resistance = SERIES_RESISTOR * (1.0 / (1023.0 / adcValue - 1.0));

  // Steinhart-Hart equation
  float tempK = 1.0 / (1.0/T0 + (1.0/BETA) * log(resistance / R0));
  float tempC = tempK - 273.15;
  return tempC;
}

void loop() {
  float temperature = readTemperature();
  int fanSpeed = 0;
  String state;

  if (temperature < 25) {
    fanSpeed = 0;
    state = "OFF";
  } 
  else if (temperature >= 25 && temperature < 30) {
    fanSpeed = 128;  // 50% duty cycle (PWM: 0-255)
    state = "50% speed";
  } 
  else {
    fanSpeed = 255;  // 100% duty cycle
    state = "100% speed";
  }

  analogWrite(fanPin, fanSpeed);

  // Print to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C | Fan State: ");
  Serial.println(state);

  delay(1000);
}