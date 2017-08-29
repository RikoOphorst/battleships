#pragma once

#include <stdint.h>

typedef uint16_t GameID;

inline GameID InvalidGameID()
{
	return static_cast<GameID>(-1);
}
