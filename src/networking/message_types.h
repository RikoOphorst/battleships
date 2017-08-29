#pragma once

#include "RakNet/MessageIdentifiers.h"

enum EMessages
{
	EMessages_Invalid = -1,

	// connectivity commands
	EMessages_SendRetrieveClientID = ID_USER_PACKET_ENUM + 1,
	EMessages_SendRetrieveGameID,
	EMessages_RecvClientIDGenerated,
	EMessages_RecvGameIDRetrieved,

	// server status messages
	EMessages_RecvServerError,
	EMessages_RecvAcknowledgement,

	// game status messages
	EMessages_RecvGameJoined,
	EMessages_RecvWaitingForGameToJoin,
	EMessages_RecvGameStarted,
	EMessages_RecvGameActivated,
	EMessages_RecvGetBoardData,
	EMessages_RecvOpponentMoveFinished,
	EMessages_RecvGameStopped,
	EMessages_RecvGameOver,

	// game commands
	EMessages_SendJoinGame,
	EMessages_SendStartGame,
	EMessages_SendStopGame,
	EMessages_SendGetBoardData,
	EMessages_SendPlaceShip,
	EMessages_SendRemoveShip,
	EMessages_SendFinishedShipPlacement,
	EMessages_SendShoot,
	EMessages_SendMoveShip,
	EMessages_SendRotateShip
};
