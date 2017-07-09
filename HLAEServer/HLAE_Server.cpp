/*

	file: HLAE_Server.cpp
	author: Brett Anthony

	Class implementation file for HLAE Server. Most functions are commented,
	general usage for public functions/vars can be found in HLAE_Server.h

*/

#include "stdafx.h"
#include <string>

#include "HLAE_Server.h"

namespace HLAEServer
{
	/*  Constructors, Destructor, Overloads  */

	// CTOR
	HLAE_WS_Server::HLAE_WS_Server(bool LISTEN_ON_INIT, String^ identifier, String^ IPADDRESS, int PORT)
	{
		// Set Member Functions
		m_identifier = identifier;
		m_ipaddress = IPADDRESS;
		m_port = PORT;
		m_listening = false;

		if (LISTEN_ON_INIT)
			StartListening();
	}

	// DTOR
	HLAE_WS_Server::~HLAE_WS_Server() { StopListening(); }

	/*  Public Functions  */

	inline bool HLAE_WS_Server::ClientAvailable() { return m_server->Pending(); }

	void HLAE_WS_Server::StartListening()
	{
		if (!m_listening)
		{
			// Setup Variables
			IPAddress^ serveraddr = IPAddress::Parse(m_ipaddress);
			m_server = gcnew TcpListener(serveraddr, m_port);

			// Start Server
			m_server->Start();
			m_listening = true;
			Console::WriteLine("Server '" + m_identifier + "' has started listening on "
				+ m_ipaddress + ":" + m_port + "...");
		}
		else
			throw;
	}

	void HLAE_WS_Server::StopListening()
	{
		if (m_listening == true)
		{
			m_server->Stop();
			m_listening = false;
			Console::WriteLine("Server '" + m_identifier + "' has stopped listening.");
		}
		else
		{
			Console::WriteLine("Server '" + m_identifier + "' isn't listening, cannot stop an already stopped server.");
		}
	}

	/*  Public Variables  */

	inline Socket^ HLAE_WS_Server::Socket() { return m_server->AcceptSocket(); }

	inline String^ HLAE_WS_Server::Name() { return m_identifier; }
	void HLAE_WS_Server::Name(String^ name)
	{
		Console::WriteLine("Server '" + m_identifier + "' has been renamed to '" + name + "'.");
		m_identifier = name;
	}


}