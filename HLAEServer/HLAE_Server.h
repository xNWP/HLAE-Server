/*
	
	file: HLAE_Server.h
	author: Brett Anthony
	
	Class definition file for HLAE Server. Public Functions/Variables
	are summarised to understand usage.

*/

#ifndef HLAE_WS_SERVER_H
#define HLAE_WS_SERVER_H

using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;

namespace HLAEServer
{
	public ref class HLAE_WS_Server
	{
		// Constructors, Destructor, Overloads
	public:
		/// <summary>Creates a listen server on specified IP and Port.</summary>
		/// <param name="LISTEN_ON_INIT">If set to true - start the server after initializing it.</param>
		/// <param name="identifier">Identifier for easy identification of server.</param>
		/// <param name="IPADDRESS">IP Address of type IPv4/IPv6 to listen on.</param>
		/// <param name="PORT">Port to listen on.</param>
		HLAE_WS_Server(bool LISTEN_ON_INIT, String^ identifier, String^ IPADDRESS, int PORT);
	private:
		~HLAE_WS_Server();

		// Public Functions
	public:
		/// <summary>Returns true if incoming TCP request(s).
		/// Returns false otherwise.</summary>
		inline bool ClientAvailable();

		/// <summary>Start Listening<para/>
		/// If called while server is already listening
		/// will cause an exception.</summary>
		void StartListening();
		/// <summary>Stops Listening.</summary>
		void StopListening();

		// Private 'helper' Functions
	private:

		// Public Variables
	public:
		/// <summary>Returns Socket of incoming connection.<para/>
		/// Use when initializing HLAE_Client.</summary>
		inline Socket^ Socket();
		/// <summary>Get identifier.</summary>
		inline String^ Name();
		/// <summary>Set identifier.</summary>
		void Name(String^ name);

		// Private Variables
	private:
		bool m_listening;
		int m_port;
		String^ m_identifier;
		String^ m_ipaddress;
		TcpListener^ m_server;

	};
}

#endif