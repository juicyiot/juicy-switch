#include "HttpHandler.h"

HttpHandler::HttpHandler(ESP8266WebServer& server, int numNetworks) : server(server), numNetworks(numNetworks) { }

void HttpHandler::handleRoot() {
	if (captivePortal()) {
		return;
	}
	initializeConnection();
	server.sendContent(
		"<html><head></head><body>"
		"<h1>Welcome to your Juicy Socket</h1>"
		"<p>Click <a href='/config'>here</a> to set up the socket's WiFi connection.</p>"
		"</body></html>"
	);
	server.client().stop();
}

void HttpHandler::handleConfig() {
	server.send(200, "text/plain", "juicy config: config");
}

void HttpHandler::handleConfigSave() {
	server.send(200, "text/plain", "juicy config: config save");
}

void HttpHandler::handleConfigSuccess() {
	server.send(200, "text/plain", "juicy config: config success");
}

void HttpHandler::handleNotFound() {
	if(captivePortal()) {
		return;
	}
	server.send(200, "text/plain", "juicy config: not found");
}

void HttpHandler::initializeConnection() {
	server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
	server.sendHeader("Pragma", "no-cache");
	server.sendHeader("Expires", "-1");
	server.setContentLength(CONTENT_LENGTH_UNKNOWN);
	server.send(200, "text/html", "");
}

bool HttpHandler::captivePortal() {
	bool isRequestForIPAddress = isIPAddress(server.hostHeader());
	bool isRequestForConfigPage = server.hostHeader() == "juicy.local";
	if (!isRequestForIPAddress && !isRequestForConfigPage) {
		server.sendHeader("Location", String("http://") + toString(server.client().localIP()), true);
		server.send(302, "text/plain", "");
		server.client().stop();
		return true;
	}

	return false;
}

bool HttpHandler::isIPAddress(String str) {
	for (int i = 0; i < str.length(); i++) {
    	int c = str.charAt(i);
    	if (c != '.' && (c < '0' || c > '9')) {
      		return false;
    	}
  	}
  	return true;
}

String HttpHandler::toString(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
	  res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}
