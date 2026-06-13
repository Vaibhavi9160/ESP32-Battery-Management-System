#include <Arduino.h>

#define CELL1_PIN 34
#define CELL2_PIN 35
#define CELL3_PIN 32
#define CELL4_PIN 33

void setup() {
  Serial.begin(115200);
}

void loop() {

  // Read cell voltages
  float cellVoltage[4];

  cellVoltage[0] = analogRead(CELL1_PIN) * 3.3 / 4095.0;
  cellVoltage[1] = analogRead(CELL2_PIN) * 3.3 / 4095.0;
  cellVoltage[2] = analogRead(CELL3_PIN) * 3.3 / 4095.0;
  cellVoltage[3] = analogRead(CELL4_PIN) * 3.3 / 4095.0;

  // Calculate average voltage
  float averageVoltage = 0;

  for (int i = 0; i < 4; i++) {
    averageVoltage += cellVoltage[i];
  }

  averageVoltage /= 4.0;

  // Find strongest and weakest cell
  float maxVoltage = cellVoltage[0];
  float minVoltage = cellVoltage[0];

  int strongestCell = 1;
  int weakestCell = 1;

  for (int i = 1; i < 4; i++) {

    if (cellVoltage[i] > maxVoltage) {
      maxVoltage = cellVoltage[i];
      strongestCell = i + 1;
    }

    if (cellVoltage[i] < minVoltage) {
      minVoltage = cellVoltage[i];
      weakestCell = i + 1;
    }
  }

  // Calculate imbalance percentage
  float imbalancePercent = 0;

  if (averageVoltage >= 0.1) {
    imbalancePercent =
        ((maxVoltage - minVoltage) / averageVoltage) * 100.0;
  }

  // Health Classification
  String healthState;

  if (imbalancePercent < 2.0) {
    healthState = "HEALTHY";
  }
  else if (imbalancePercent < 5.0) {
    healthState = "MINOR IMBALANCE";
  }
  else if (imbalancePercent < 10.0) {
    healthState = "CRITICAL IMBALANCE";
  }
  else {
    healthState = "PACK FAILURE";
  }

  // Display Results
  Serial.println("\n==============================");

  for (int i = 0; i < 4; i++) {
    Serial.print("Cell ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(cellVoltage[i], 2);
    Serial.println(" V");
  }

  Serial.println("------------------------------");

  Serial.print("Average Voltage: ");
  Serial.print(averageVoltage, 2);
  Serial.println(" V");

  Serial.print("Strongest Cell: Cell ");
  Serial.println(strongestCell);

  Serial.print("Weakest Cell: Cell ");
  Serial.println(weakestCell);

  Serial.print("Imbalance: ");
  Serial.print(imbalancePercent, 2);
  Serial.println(" %");

  Serial.print("Battery Health: ");
  Serial.println(healthState);

  Serial.println("==============================");

  delay(2000);
}