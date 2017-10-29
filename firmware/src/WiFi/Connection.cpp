#include "Connection.h"

#include <ESP8266WiFi.h>

Connection::Connection() {
	CredentialsStorage::load(&credentials, sizeof(credentials));
	Serial.print("Loaded credentials: ");
	Serial.print(String() + credentials.ssid + " ");
	Serial.print(String() + credentials.password + "\n");
}

Connection::Connection(const char *ssid, const char *password) {
	strncpy(credentials.ssid, ssid, CREDENTIAL_SIZE);
	strncpy(credentials.password, password, CREDENTIAL_SIZE);
}

bool Connection::connect() const {
	WiFi.persistent(false); // Don't automatically persist credentials. We'll do it ourselves.
	WiFi.begin(credentials.ssid, credentials.password);
	uint8_t status = WiFi.waitForConnectResult();

	return isConnected();
}

bool Connection::isConnected() const {
	wl_status_t status = WiFi.status();

	if (status != WL_CONNECTED) {
		Serial.println("Not Connected");
		return false;
	}

	Serial.println("Connected.");
	return true;
}

void Connection::disconnect() const {
	if (isConnected()) {
		WiFi.disconnect();
	}
}

void Connection::persistCredentials() const {
	Serial.print("Persisting credentials: ");
	Serial.print(String() + credentials.ssid + " ");
	Serial.print(String() + credentials.password + "\n");
	CredentialsStorage::save(&credentials, sizeof(credentials));
}
