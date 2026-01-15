#include "CNetwork.h"
#include <stdint.h>

#include "util/CJavaWrapper.h"

const char* g_szServerNames[MAX_SERVERS] = {
	"BLACK MOSCOW | RED",
	"BLACK RUSSIA | TEST"
};

const CSetServer::CServerInstanceEncrypted g_sEncryptedAddresses[MAX_SERVERS] = {
	CSetServer::create("104.234.180.156", 364, 11, 7777, true),
	CSetServer::create("104.234.180.156", 364, 11, 7777, true)
};
