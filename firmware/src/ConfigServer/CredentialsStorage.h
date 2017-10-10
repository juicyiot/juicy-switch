#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <EEPROM.h>

const size_t CREDENTIAL_SIZE = 64;

typedef struct {
	char ssid[CREDENTIAL_SIZE];
	char password[CREDENTIAL_SIZE];
} credentials_t;

class CredentialsStorage {
public:
	static void save(void *data_src, size_t size);
	static void load(void *data_dst, size_t size);
	static void clearMemory(size_t size);
};

#endif
