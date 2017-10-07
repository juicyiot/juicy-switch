#include "WiFiScanner.h"
#include "ESP8266WiFi.h"

WiFiScanner::WiFiScanner() {
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);
}

WiFiScanner::~WiFiScanner() {
	WiFi.disconnect();
}

vector<string> WiFiScanner::networks() {
	int n = WiFi.scanNetworks();

	vector<string> networks;
	for (int i = 0; i < n; i++) {
		string network(WiFi.SSID(i).c_str());
		networks.push_back(network);
	}

	return networks;
}
