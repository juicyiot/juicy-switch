#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <EEPROM.h>

const size_t CREDENTIALS_SIZE = 64;

typedef struct {
	char ssid[CREDENTIALS_SIZE];
	char password[CREDENTIALS_SIZE];
} credentials_t;

class CredentialsStorage {
public:
	void save(void *data_src);
	void load(void *data_dst);
	void clearMemory();
};

#endif
