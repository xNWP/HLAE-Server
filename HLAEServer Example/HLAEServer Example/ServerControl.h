/*

	file: ServerControl.h
	author: Brett Anthony
	
	Provides definitions for our server.
	
*/

#pragma once
#include <sstream>
#include <msclr\marshal_cppstd.h>

using namespace System;
using namespace System::ComponentModel;

ref class ServerControl
{
public:
	ServerControl()
	{
		WAIT_FOR_CLIENT();
		CONNECT_CLIENT();
	}

private:


	void WAIT_FOR_CLIENT()
	{
		HLAE_SERVER = gcnew HLAEServer::HLAE_WS_Server(true, "HLAE_SERVER", "127.0.0.1", 30000);

		while (!HLAE_SERVER->ClientAvailable())
		{
			// while there is no client connecting we'll simply wait.
		}
	}

	void CONNECT_CLIENT()
	{
		// we got a client! connect them
		HLAE_CLIENT = gcnew HLAEServer::HLAE_WS_Client("HLAE_CLIENT", HLAE_SERVER);
	}

public: void UPDATE_DATA()
	{
		HLAE_CLIENT->ReadDataWS();			

		std::ostringstream s;
		using namespace msclr::interop;
		s << HLAE_CLIENT->HLAE_Version;
		client_version = marshal_as<String^>(s.str());
		s.str("");
		client_map = HLAE_CLIENT->HLAE_Level;
		s << HLAE_CLIENT->HLAE_Time;
		client_time = marshal_as<String^>(s.str());
		s.str("");
		s << HLAE_CLIENT->HLAE_fov;
		client_fov = marshal_as<String^>(s.str());
		s.str("");
		s << HLAE_CLIENT->HLAE_Xpos;
		client_xpos = marshal_as<String^>(s.str());
		s.str("");
		s << HLAE_CLIENT->HLAE_Ypos;
		client_ypos = marshal_as<String^>(s.str());
		s.str("");
		s << HLAE_CLIENT->HLAE_Zpos;
		client_zpos = marshal_as<String^>(s.str());
		s.str("");
		s << HLAE_CLIENT->HLAE_Xrot;
		client_xrot = marshal_as<String^>(s.str());
		s.str("");
		s << HLAE_CLIENT->HLAE_Yrot;
		client_yrot = marshal_as<String^>(s.str());
		s.str("");
		s << HLAE_CLIENT->HLAE_Zrot;
		client_zrot = marshal_as<String^>(s.str());
		s.str("");
	}

private:
	HLAEServer::HLAE_WS_Server^ HLAE_SERVER;
	HLAEServer::HLAE_WS_Client^ HLAE_CLIENT;

public:
	String^ client_version;
	String^ client_map;
	String^ client_time;
	String^ client_fov;
	String^ client_xpos;
	String^ client_ypos;
	String^ client_zpos;
	String^ client_xrot;
	String^ client_yrot;
	String^ client_zrot;

};

