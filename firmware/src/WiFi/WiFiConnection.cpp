#include "WiFiConnection.h"

#include <ESP8266WiFi.h>

credentials_t WiFiConnection::credentials;

WiFiConnection::WiFiConnection() {
	CredentialsStorage::load(&credentials, sizeof(credentials));
}

WiFiConnection::WiFiConnection(const char *ssid, const char *password) {
	strncpy(credentials.ssid, ssid, CREDENTIAL_SIZE);
	strncpy(credentials.password, password, CREDENTIAL_SIZE);
}

bool WiFiConnection::connect() const {
	// Don't automatically persist credentials. We'll do it ourselves.
	WiFi.persistent(false);

	if (WiFi.status() == WL_CONNECTED) {
		return true;
	}

	WiFi.begin(credentials.ssid, credentials.password);

	return WiFi.waitForConnectResult() == WL_CONNECTED;
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

String WiFiConnection::getSSID() const {
	return String() + credentials.ssid;
}

void WiFiConnection::persistCredentials() const {
	CredentialsStorage::save(&credentials, sizeof(credentials));
}

void WiFiConnection::clearCredentials() {
	CredentialsStorage::clearMemory(sizeof(credentials));

	strncpy(credentials.ssid, "", CREDENTIAL_SIZE);
	strncpy(credentials.password, "", CREDENTIAL_SIZE);
}
