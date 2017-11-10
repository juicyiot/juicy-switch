#include "WiFiConnection.h"

#include <ESP8266WiFi.h>

WiFiConnection::WiFiConnection() {
	CredentialsStorage::load(&credentials, sizeof(credentials));

	Serial.print("Loaded credentials: ");
	Serial.print(String() + credentials.ssid + " ");
	Serial.print(String() + credentials.password + "\n");
}

WiFiConnection::WiFiConnection(const char *ssid, const char *password) {
	strncpy(credentials.ssid, ssid, CREDENTIAL_SIZE);
	strncpy(credentials.password, password, CREDENTIAL_SIZE);
}

bool WiFiConnection::connect(bool shouldPersistCredentials) const {
	// Don't automatically persist credentials. We'll do it ourselves.
	WiFi.persistent(false);

	WiFi.begin(credentials.ssid, credentials.password);
	uint8_t status = WiFi.waitForConnectResult();

	if (shouldPersistCredentials) {
		persistCredentials();
	}

	return isConnected();
}

void WiFiConnection::disconnect(bool shouldClearCredentials) {
	if (isConnected()) {
		WiFi.disconnect();
	}

	if (shouldClearCredentials) {
		clearCredentials();
	}
}

bool WiFiConnection::isConnected() const {
	wl_status_t status = WiFi.status();

	if (status != WL_CONNECTED) {
		return false;
	}

	return true;
}

String WiFiConnection::getIP() const {
	return WiFi.localIP().toString();
}

void WiFiConnection::persistCredentials() const {
	CredentialsStorage::save(&credentials, sizeof(credentials));

	Serial.print("Persisting credentials: ");
	Serial.print(String() + credentials.ssid + " ");
	Serial.print(String() + credentials.password + "\n");
}

void WiFiConnection::clearCredentials() {
	CredentialsStorage::clearMemory(sizeof(credentials));

	strncpy(credentials.ssid, "", CREDENTIAL_SIZE);
	strncpy(credentials.password, "", CREDENTIAL_SIZE);
}
