#pragma once

#include <stdint.h>

typedef uint16_t ClientID;

inline ClientID InvalidClientID()
{
	return static_cast<ClientID>(-1);
}
