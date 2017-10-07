#include <Arduino.h>
#include "WiFiScanner/WiFiScanner.h"

#include <vector>
#include <string>

void setup() {
    Serial.begin(115200);
	delay(3000);

	WiFiScanner scanner;
	vector<string> networks = scanner.networks();

	int i = 0;
	for(auto &n : networks) {
		Serial.printf("[%d]: ", i++);
		Serial.println(n.c_str());
	}
}

void loop() {
    // put your main code here, to run repeatedly:
}
