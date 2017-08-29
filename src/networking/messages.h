#pragma once

#include "client_id.h"
#include "game_id.h"
#include "server_errors.h"

#pragma pack(push, 1)

struct ShootMessage
{
	ClientID m_ClientID;
	GameID m_GameID;
	uint32_t m_X;
	uint32_t m_Y;
};

struct MoveShipMessage
{
	const static short FORWARD = 0;
	const static short BACKWARD = 1;

	ClientID m_ClientID;
	GameID m_GameID;
	uint32_t m_Ship;
	short m_Direction;
};

struct RotateShipMessage
{
	const static short STARBOARD = 0;
	const static short PORT = 1;

	ClientID m_ClientID;
	GameID m_GameID;
	uint32_t m_Ship;
	short m_Direction;
};

struct GameJoinedMessage
{
	ClientID m_ClientID;
	GameID m_GameID;
};

struct JoinGameMessage
{
	ClientID m_ClientID;
};

struct RetrieveGameIDMessage
{
	ClientID m_ClientID;
};

struct GameStartedMessage
{
	ClientID m_ClientID;
	ClientID m_StartingClientID;
};

struct AcknowlegdementMessage
{
	const static uint32_t INVALID = static_cast<uint32_t>(-1);

	ClientID m_ClientID;
	uint32_t m_Value[9];
};

struct GameIsActiveMessage
{
	ClientID m_ClientID;
	bool m_IsActive;
};

struct ServerErrorMessage
{
	ClientID m_ClientID;
	EServerError m_Error;
};

struct GetBoardDataMessage
{
	struct ShipData
	{
		uint32_t m_Ship;
		uint32_t m_X;
		uint32_t m_Y;
		uint32_t m_Direction; // see PlaceShipMessage for directions
		bool m_HitPattern[5]; // 5 sent, but only 1st 'x'(size of ship) are used
	};

	ClientID m_ClientID;
	GameID m_GameID;
	ShipData m_ShipData[5];
};

struct PlaceShipMessage
{
	const static short UP = 0;
	const static short LEFT = 1;
	const static short DOWN = 2;
	const static short RIGHT = 3;

	ClientID m_ClientID;
	GameID m_GameID;
	uint32_t m_Ship;
	uint32_t m_X;
	uint32_t m_Y;
	short m_Direction;
};

struct FinishedShipPlacementMessage
{
	ClientID m_ClientID;
	GameID m_GameID;
};

struct RemoveShipMessage
{
	ClientID m_ClientID;
	GameID m_GameID;
	uint32_t m_Ship;
};

struct GameOverMessage
{
	GameID m_GameID;
	ClientID m_Winner;
	ClientID m_Loser;
};

#pragma pack(pop)
