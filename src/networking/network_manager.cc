#include "network_manager.h"

#include <iostream>

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	NetworkManager::NetworkManager()
	{
		
	}

	//------------------------------------------------------------------------------------------------------
	NetworkManager::~NetworkManager()
	{
		
	}

	//------------------------------------------------------------------------------------------------------
	NetworkManager* NetworkManager::Instance()
	{
		static NetworkManager* instance = nullptr;
		static memory::StackAllocator* allocator = nullptr;

		if (instance == nullptr)
		{
			allocator = memory::MemoryManager::Instance()->ConstructAllocator<memory::StackAllocator>(sizeof(NetworkManager*) * 10);
			instance = memory::MemoryManager::Allocate<NetworkManager>(allocator);
		}

		return instance;
	}

	//------------------------------------------------------------------------------------------------------
	void NetworkManager::DeleteInstance()
	{
		//NetworkManager* instance = Instance();
		//memory::MemoryManager::Deallocate<NetworkManager>(allocator_, instance);
		//memory::MemoryManager::Instance()->DestructAllocator(allocator_);
	}

	//------------------------------------------------------------------------------------------------------
	void NetworkManager::Startup()
	{
		//peer_ = RakNet::RakPeerInterface::GetInstance();
        //
		//RakNet::SocketDescriptor sd;
		//peer_->Startup(1, &sd, 1);
        //
		//peer_->Connect("127.0.0.1", 60000, 0, 0);
		//RakNet::SystemAddress server_address;
        //
		//RakNet::Packet* packet;
        //
		//bool exit = false;
        //
		//while (!exit)
		//{
		//	for (packet = peer_->Receive(); packet; peer_->DeallocatePacket(packet), packet = peer_->Receive())
		//	{
		//		switch (packet->data[0])
		//		{
		//		case ID_CONNECTION_REQUEST_ACCEPTED:
		//			address_ = packet->systemAddress;
		//			exit = true;
		//			break;
		//		}
		//	}
		//}
	}
	
	//------------------------------------------------------------------------------------------------------
	void NetworkManager::RequestClientId()
	{
		//RakNet::BitStream bs;
        //
		//bs.Write((RakNet::MessageID)EMessages_SendRetrieveClientID);
		//peer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, address_, false);
        //
		//RakNet::Packet* packet;
        //
		//bool exit = false;
		//while (!exit)
		//{
		//	for (packet = peer_->Receive(); packet; peer_->DeallocatePacket(packet), packet = peer_->Receive())
		//	{
		//		switch (packet->data[0])
		//		{
		//		case EMessages_RecvClientIDGenerated:
		//		{
		//			RakNet::BitStream bsIn(packet->data, packet->length, false);
        //
		//			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		//			bsIn.Read(client_id_);
        //
		//			std::cout << "Received client id: " << client_id_ << std::endl;
        //
		//			exit = true;
		//		}
		//		break;
		//		default:
		//		{
		//			std::cout << "Expected client id, received something else." << std::endl;
		//		}
		//		break;
		//		}
		//	}
		//}
	}

	//------------------------------------------------------------------------------------------------------
	void NetworkManager::RequestJoinGame()
	{
		//std::cout << "Sending request for game.." << std::endl;
		//RakNet::BitStream bs;
        //
		//JoinGameMessage msg;
        //
		//msg.m_ClientID = client_id_;
        //
		//bs.Write((RakNet::MessageID)EMessages_SendJoinGame);
		//bs.Write(msg.m_ClientID);
		//peer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, address_, false);
	}
	
	//------------------------------------------------------------------------------------------------------
	void NetworkManager::WaitForGameStart()
	{
		//std::cout << "Waiting for game start..." << std::endl;
        //
		//RakNet::Packet* packet;
        //
		//bool exit = false;
		//while (!exit)
		//{
		//	for (packet = peer_->Receive(); packet; peer_->DeallocatePacket(packet), packet = peer_->Receive())
		//	{
		//		switch (packet->data[0])
		//		{
		//		case EMessages_RecvGameJoined: {
		//			RakNet::BitStream bsIn(packet->data, packet->length, false);
        //
		//			GameJoinedMessage msg;
        //
		//			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		//			bsIn.Read(msg);
        //
		//			client_id_ = msg.m_ClientID;
		//			game_id_ = msg.m_GameID;
        //
		//			std::cout << "Joined game " << msg.m_GameID << " with client id " << msg.m_ClientID << std::endl;
		//			exit = true;
		//		} break;
		//		}
		//	}
		//}
	}
	
	void NetworkManager::PlaceShip(Ship* ship, unsigned int col, unsigned int row, Ship::ShipDirection direction)
	{
		//PlaceShipMessage psm;
        //
		//psm.m_ClientID = client_id_;
		//psm.m_GameID = game_id_;
		//
		//if (direction == Ship::ShipDirection::UP)
		//{
		//	psm.m_Direction = psm.RIGHT;
		//}
		//else if (direction == Ship::ShipDirection::DOWN)
		//{
		//	psm.m_Direction = psm.LEFT;
		//}
		//else if (direction == Ship::ShipDirection::RIGHT)
		//{
		//	psm.m_Direction = psm.DOWN;
		//}
		//else if (direction == Ship::ShipDirection::LEFT)
		//{
		//	psm.m_Direction = psm.UP;
		//}
        //
		//switch (ship->GetType())
		//{
		//case Ship::ShipTypes::Carrier:
		//	psm.m_Ship = EShip_AircraftCarrier;
		//	break;
		//case Ship::ShipTypes::Battleship:
		//	psm.m_Ship = EShip_Battleship;
		//	break;
		//case Ship::ShipTypes::Destroyer:
		//	psm.m_Ship = EShip_Destroyer;
		//	break;
		//case Ship::ShipTypes::Submarine:
		//	psm.m_Ship = EShip_Submarine;
		//	break;
		//case Ship::ShipTypes::Patrol:
		//	psm.m_Ship = EShip_PatrolBoat;
		//	break;
		//}
        //
		//psm.m_X = 9 - row;
		//psm.m_Y = col;
        //
		//std::cout << "Sending ship placement.." << std::endl;
		//RakNet::BitStream bs;
        //
		//bs.Write((RakNet::MessageID)EMessages_SendPlaceShip);
		//bs.Write(psm);
		//peer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, address_, false);
	}
	
	//------------------------------------------------------------------------------------------------------
	bool NetworkManager::WaitForShipPlaceResponse()
	{
		//std::cout << "Waiting for ship place response..." << std::endl;
        //
		//RakNet::Packet* packet;
        //
		//bool exit = false;
		//while (!exit)
		//{
		//	for (packet = peer_->Receive(); packet; peer_->DeallocatePacket(packet), packet = peer_->Receive())
		//	{
		//		switch (packet->data[0])
		//		{
		//		case EMessages_RecvAcknowledgement: {
		//			RakNet::BitStream bsIn(packet->data, packet->length, false);
        //
		//			AcknowlegdementMessage msg;
        //
		//			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		//			bsIn.Read(msg);
        //
		//			return true;
		//		} break;
		//		case EMessages_RecvServerError: {
		//			RakNet::BitStream bsIn(packet->data, packet->length, false);
        //
		//			ServerErrorMessage msg;
        //
		//			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		//			bsIn.Read(msg);
        //
		//			std::cout << "Received server error: " << msg.m_Error << std::endl;
		//			
		//			return false;
		//		} break;
		//		}
		//	}
		//}

        return true;
	}
	
	//------------------------------------------------------------------------------------------------------
	void NetworkManager::FinishedShipPlacement()
	{
		//std::cout << "Sending ship placement finished..." << std::endl;
        //
		//FinishedShipPlacementMessage fsp;
        //
		//fsp.m_ClientID = client_id_;
		//fsp.m_GameID = game_id_;
        //
		//RakNet::BitStream bs;
        //
		//bs.Write((RakNet::MessageID)EMessages_SendFinishedShipPlacement);
		//bs.Write(fsp);
		//peer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, address_, false);
	}

	//------------------------------------------------------------------------------------------------------
	bool NetworkManager::WaitForAcknowledgement()
	{
		//std::cout << "Waiting for acknowledgement of finishing ship placement.." << std::endl;
        //
		//RakNet::Packet* packet;
        //
		//bool exit = false;
		//while (!exit)
		//{
		//	for (packet = peer_->Receive(); packet; peer_->DeallocatePacket(packet), packet = peer_->Receive())
		//	{
		//		switch (packet->data[0])
		//		{
		//		case EMessages_RecvAcknowledgement: {
		//			RakNet::BitStream bsIn(packet->data, packet->length, false);
        //
		//			AcknowlegdementMessage msg;
        //
		//			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		//			bsIn.Read(msg);
        //
		//			std::cout << "Received acknowledgement for finishing ship placement " << std::endl;
        //
		//			return true;
		//		} break;
		//		case EMessages_RecvServerError: {
		//			RakNet::BitStream bsIn(packet->data, packet->length, false);
        //
		//			ServerErrorMessage msg;
        //
		//			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		//			bsIn.Read(msg);
        //
		//			std::cout << "Received server error: " << msg.m_Error << std::endl;
        //
		//			return false;
		//		} break;
		//		}
		//	}
		//}

        return true;
	}
	
	//------------------------------------------------------------------------------------------------------
	void NetworkManager::RequestStartGame()
	{

		//std::cout << "Waiting for game start..." << std::endl;
        //
		//RakNet::Packet* packet;
        //
		//bool exit = false;
		//while (!exit)
		//{
		//	for (packet = peer_->Receive(); packet; peer_->DeallocatePacket(packet), packet = peer_->Receive())
		//	{
		//		switch (packet->data[0])
		//		{
		//		case EMessages_RecvGameStarted: {
		//			std::cout << "Received game started.." << std::endl;
		//			
		//			RakNet::BitStream bsIn(packet->data, packet->length, false);
        //
		//			GameStartedMessage msg;
        //
		//			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		//			bsIn.Read(msg);
        //
		//			if (msg.m_StartingClientID == client_id_)
		//			{
		//				my_turn_ = true;
		//			}
		//			else
		//			{
		//				my_turn_ = false;
		//			}
		//			return;
		//		} break;
		//		case EMessages_RecvServerError: {
		//			RakNet::BitStream bsIn(packet->data, packet->length, false);
        //
		//			ServerErrorMessage msg;
        //
		//			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		//			bsIn.Read(msg);
        //
		//			std::cout << "Received server error: " << ServErrorToString(msg.m_Error).c_str() << std::endl;
		//		} break;
		//		default:
		//		{
		//			std::cout << "unkown" << std::endl;
		//		} break;
		//		}
		//	}
        //
		//	std::cout << "Sending request for game start.." << std::endl;
        //
		//	RakNet::BitStream bs;
        //
		//	bs.Write((RakNet::MessageID)EMessages_SendStartGame);
		//	bs.Write(client_id_);
		//	peer_->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, address_, false);
        //
		//	Sleep(50);
		//}
	}
	
	//------------------------------------------------------------------------------------------------------
	bool NetworkManager::WaitForStartGame()
	{
		return true;
	}
	
	//------------------------------------------------------------------------------------------------------
	std::string NetworkManager::ServErrorToString(EServerError error)
	{
		switch (error)
		{
		case EServerError_InvalidClientIDAddressCombination:
			return "Invalid client id/address combination.";

		case EServerError_PlayerAlreadyJoinedGame:
			return "Already joined game.";

		case EServerError_NoGameFoundForPlayer:
			return "No game found for player.";

		case EServerError_GameNotActive:
			return "Game is not active";

		case EServerError_GameAlreadyStarted:
			return "Game has already started";

		case EServerError_ShipPlacementNotDone:
			return "Ship placement is not done yet.";

		case EServerError_ShipPlacementInvalid:
			return "Invalid ship placement";

		case EServerError_ShipAlreadyPlaced:
			return "Ship is already placed";

		case EServerError_InvalidShip:
			return "Ship is invalid";

		case EServerError_ShipRemovalInvalid:
			return "Invalid ship removal";

		case EServerError_ShipPlacementAlreadyDone:
			return "Ship placement is already finished";

		case EServerError_PlayedBeforeYourTurn:
			return "Played before your turn";

		case EServerError_ConnectionClosedByServer:
			return "Server closed the connection";

		case EServerError_ServerShutDown:
			return "Server is shut down";

		case EServerError_UnknownCommand:
			return "Unknown command";

		case EServerError_GeneralError:
			return "General error => something bad happened";
		}
	}
}