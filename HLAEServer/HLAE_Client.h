/*

	file: HLAE_Client.h
	author: Brett Anthony

	Class definition file for HLAE Client. Public Functions/Variables
	are summarised to understand usage.

*/

#ifndef HLAE_WS_CLIENT_H
#define HLAE_WS_CLIENT_H

using namespace System::Text;
using namespace System::Security::Cryptography;
using namespace msclr::interop;

namespace HLAEServer
{
	public ref class HLAE_WS_Client
	{
		// Constructors, Destructor, Overloads
	public:
		/// <summary>Creates an HLAE Client from incoming TCP request to HLAESERVER.</summary>
		/// <param name="identifier">Identifier for easy identification of client</param>
		/// <param name="server">Server we are accepting the client from.</param>
		HLAE_WS_Client(String^ identifier, HLAE_WS_Server^ server);
	private:
		~HLAE_WS_Client();

		// Public Functions
	public:
		/// <summary>Returns true if there is a connected client.
		/// Returns false otherwise.</summary>
		inline bool Connected();
		/// <summary>Reads current data in the client buffer and updates
		/// variables accordingly.<para/>
		/// Should the client send a close connection request, you must
		/// reinitialize the client.</summary>
		void ReadDataWS();
		/// <summary>Send 'data' as a command to the client.</summary>
		void SendDataWS(String^ data);

		// Private 'helper' Functions
	private:
		array<Byte>^ GetAllDataInBuffer();
		array<Byte>^ GetDataInBuffer();
		array<Byte>^ ReturnWebsocketPayload(array<Byte>^ FullFrame);
		Byte GetBits(Byte byte, int start, int end);
		Byte GetByteFromUInt64(UInt64 INT, int byte);
		int StartAfter(array<Byte>^ data, String^ key);
		void DoHandShake(array<Byte>^ data);
		void SendData(array<Byte>^ data);
		void SendPing();
		float FourByteFloatLE(array<Byte>^ data, int offset);
		UInt32 FourByteUInt32LE(array<Byte>^ data, int offset);
		UInt16 TwoByteUInt16BE(array<Byte>^ data, int offset);
		UInt64 EightByteUInt64BE(array<Byte>^ data, int offset);

		// Public Variables
	public:
		/// <summary>Gets identifier.</summary>
		inline String^ Name();
		/// <summary>Sets identifier.</summary>
		void Name(String^ name);

		// Client Values
		/// <summary>Client mirv_pgl version as last reported by client.</summary>
		UInt32 HLAE_Version;
		/// <summary>Client level as last reported by client.</summary>
		String^ HLAE_Level;

		/// <summary>Client time as last reported by client.</summary>
		float HLAE_Time;
		/// <summary>Client X Position as last reported by client.</summary>
		float HLAE_Xpos;
		/// <summary>Client Y Position as last reported by client.</summary>
		float HLAE_Ypos;
		/// <summary>Client Z Position as last reported by client.</summary>
		float HLAE_Zpos;
		/// <summary>Client X Rotation as last reported by client.</summary>
		float HLAE_Xrot;
		/// <summary>Client Y Rotation as last reported by client.</summary>
		float HLAE_Yrot;
		/// <summary>Client Z Rotation as last reported by client.</summary>
		float HLAE_Zrot;
		/// <summary>Client Field of View as last reported by client.</summary>
		float HLAE_fov;

		// Private Variables
	private:
		Socket^ m_socket;
		String^ m_identifier;
		String^ s_identifier;

	};
}

#endif