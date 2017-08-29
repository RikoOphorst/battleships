#pragma once

#include "../memory/memory_manager.h"
#include "../memory/stack_allocator.h"

#include "../networking/client_id.h"
#include "../networking/collision_types.h"
#include "../networking/game_id.h"
#include "../networking/message_types.h"
#include "../networking/messages.h"
#include "../networking/move_results.h"
#include "../networking/server_errors.h"
#include "../networking/ship_types.h"
#include "../networking/shot_results.h"
#include "../networking/raknet/RakPeerInterface.h"
#include "../networking/raknet/MessageIdentifiers.h"
#include "../networking/raknet/PacketLogger.h"
#include "../networking/raknet/BitStream.h"
#include "../gameplay/ship.h"

#include <string>

namespace igad
{
	class NetworkManager
	{
	public:
		NetworkManager();
		~NetworkManager();

		static NetworkManager* Instance();
		static void DeleteInstance();

		void Startup();
		void RequestClientId();
		void RequestJoinGame();
		void WaitForGameStart();

		void PlaceShip(Ship* ship, unsigned int col, unsigned int row, Ship::ShipDirection direction);
		bool WaitForShipPlaceResponse();

		void FinishedShipPlacement();
		bool WaitForAcknowledgement();

		void RequestStartGame();
		bool WaitForStartGame();

		bool IsItMyTurn();

		void Shoot(unsigned int col, unsigned int row);
		
		std::string ServErrorToString(EServerError error);
	private:

		RakNet::RakPeerInterface* peer_;
		RakNet::SystemAddress address_;
		ClientID client_id_;

		bool my_turn_;

		GameID game_id_;
	};
}