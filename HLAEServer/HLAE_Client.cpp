/*

	file: HLAE_Client.cpp
	author: Brett Anthony

	Class implementation file for HLAE Client. Most functions are commented,
	general usage for public functions/vars can be found in HLAE_Client.h

*/

#include "stdafx.h" 
#include <msclr/marshal_cppstd.h>
#include <math.h>

#include "HLAE_Server.h"
#include "HLAE_Client.h"

namespace HLAEServer
{
	/*  Constructors, Destructor, Overloads  */

	// CTOR
	HLAE_WS_Client::HLAE_WS_Client(String^ identifier, HLAE_WS_Server^ server)
	{
		m_socket = server->Socket();
		m_identifier = identifier;
		s_identifier = server->Name();

		Console::WriteLine("Client '" + identifier + "' is establishing a connection with '" + s_identifier + "'...");

		array<Byte>^ HandShakeData = GetAllDataInBuffer();
		DoHandShake(HandShakeData);
		
		Console::WriteLine("Client '" + identifier + "' has connected to '" + s_identifier + "'.");
	}

	// DTOR
	HLAE_WS_Client::~HLAE_WS_Client()
	{
		m_socket->Disconnect(false);
	}

	/*  Public Functions  */

	inline bool HLAE_WS_Client::Connected()
	{
		SendPing(); // We'll ping the server to update Connected.
		return m_socket->Connected;
	}

	void HLAE_WS_Client::SendPing()
	{
		int Code = 0b10001001;
		array<Byte>^ Payload = gcnew array<Byte>(2);
		Payload[0] = (Byte)Code;
		Payload[1] = 0; // Mask = 0, Payload Length = 0.

		SendData(Payload);
	}

	void HLAE_WS_Client::ReadDataWS()
	{

		// catch no client
		if (m_socket->Connected == false)
			return void();
	


		int it = 0; // keep track of where we are in the data

		array<Byte>^ data = GetDataInBuffer(); // get data from buffer

		if (data->Length == 0) // catch an empty buffer
			return void();

		array<Byte>^ WebSocketDataRaw = ReturnWebsocketPayload(data); // get decoded payload from data
		
		if (WebSocketDataRaw == nullptr) // catch an empty payload
			return void();

		String^ WebSocketData = Encoding::UTF8->GetString(WebSocketDataRaw);
		/* currently handles all data that hlae sends as of mirv_pgl ver 2 */
		String^ CMD;
		while (it < WebSocketData->Length)
		{
			// Look for HLAE Command
			CMD = "";
			while ((WebSocketData[it] != '\0') && (it < WebSocketData->Length))
			{
				CMD += WebSocketData[it];
				it++;
			}
			it++;

			// Handle Command
			if (CMD == "hello")
			{
				Console::WriteLine(m_identifier + "->" + s_identifier + " | hello");

				HLAE_Version = FourByteUInt32LE(WebSocketDataRaw, it);
				it += 4;
				Console::WriteLine(m_identifier + "->" + s_identifier + " | version = " + HLAE_Version);

				continue;
			}

			if (CMD == "dataStart")
			{
				Console::WriteLine(m_identifier + "->" + s_identifier + " | dataStart");
				continue;
			}

			if (CMD == "dataStop")
			{
				Console::WriteLine(m_identifier + "->" + s_identifier + " | dataStop");
				continue;
			}

			if (CMD == "levelInit")
			{
				HLAE_Level = "";
				while (WebSocketData[it] != '\0')
				{
					HLAE_Level += WebSocketData[it];
					it++;
				}
				it++;
				continue;
			}

			if (CMD == "levelShutdown")
			{
				HLAE_Level = "";
				Console::WriteLine(m_identifier + "->" + s_identifier + " | levelShutdown");
				continue;
			}

			if (CMD == "cam")
			{
				Console::WriteLine(m_identifier + "->" + s_identifier + " | CAM DATA");
				
				HLAE_Time = FourByteFloatLE(WebSocketDataRaw, it);
				Console::WriteLine("Time: " + HLAE_Time);
				it += 4;

				HLAE_Xpos = FourByteFloatLE(WebSocketDataRaw, it);
				Console::WriteLine("X: " + HLAE_Xpos);
				it += 4;

				HLAE_Ypos = FourByteFloatLE(WebSocketDataRaw, it);
				Console::WriteLine("Y: " + HLAE_Ypos);
				it += 4;

				HLAE_Zpos = FourByteFloatLE(WebSocketDataRaw, it);
				Console::WriteLine("Z: " + HLAE_Zpos);
				it += 4;

				HLAE_Xrot = FourByteFloatLE(WebSocketDataRaw, it);
				Console::WriteLine("X-Rot: " + HLAE_Xrot);
				it += 4;

				HLAE_Yrot = FourByteFloatLE(WebSocketDataRaw, it);
				Console::WriteLine("Y-Rot: " + HLAE_Yrot);
				it += 4;

				HLAE_Zrot = FourByteFloatLE(WebSocketDataRaw, it);
				Console::WriteLine("Z-Rot: " + HLAE_Zrot);
				it += 4;

				HLAE_fov = FourByteFloatLE(WebSocketDataRaw, it);
				Console::WriteLine("FoV [real]: " + HLAE_fov);
				it += 4;
				
				continue;
			}
		}
	}

	void HLAE_WS_Client::SendDataWS(String^ data)
	{
		String^ FormattedData = "exec\0" + data + "\0";
		array<Byte>^ Payload = Encoding::UTF8->GetBytes(FormattedData);
		UInt64 PayloadLength = Payload->Length; // exec\0 + data + \0
		array<Byte>^ ReturnBytes;

		int it = 0; // Keep track of where we are

		if (PayloadLength < 126)
		{
			ReturnBytes = gcnew array<Byte>(2 + *(int*)&PayloadLength);
			ReturnBytes[0] = 0b10000010;
			ReturnBytes[1] = *(Byte*)&PayloadLength;
			it = 2;
		}
		else if (PayloadLength >= 126 && PayloadLength <= 65536)
		{
			ReturnBytes = gcnew array<Byte>(4 + *(int*)&PayloadLength);
			ReturnBytes[0] = 0b10000010;
			ReturnBytes[1] = 126;
			ReturnBytes[3] = *(int*)&PayloadLength % 256;
			ReturnBytes[2] = *(int*)&PayloadLength - ReturnBytes[3];
			it = 4;
		}
		else
		{
			ReturnBytes = gcnew array<Byte>(10 + *(int*)&PayloadLength);
			ReturnBytes[0] = 0b10000010;
			ReturnBytes[1] = 127;

			for (int i = 2; i < 10; i++)
				ReturnBytes[i] = GetByteFromUInt64(PayloadLength, i - 2);

			it = 10;
		}

		int j = 0; // to make sure we don't null reference our string
		while (it < ReturnBytes->Length && j < Payload->Length)
		{
			ReturnBytes[it] = Payload[j];
			it++; j++;
		}
		
		// Send the final data.
		SendData(ReturnBytes);
	}

	/*  Private 'helper' Functions  */
	
	array<Byte>^ HLAE_WS_Client::GetAllDataInBuffer()
	{
		/* Continuously grabs data and appends it to received data
		until the buffer returns no more available data. */
		array<Byte>^ ReceivedData;
		String^ ReceivedDataStr;
		
		while (m_socket->Available > 0)
		{
			ReceivedData = GetDataInBuffer();
			ReceivedDataStr += Encoding::UTF8->GetString(ReceivedData);
		}

		ReceivedData = Encoding::UTF8->GetBytes(ReceivedDataStr);
		return ReceivedData;
	}

	array<Byte>^ HLAE_WS_Client::GetDataInBuffer()
	{
		/* Returns the current data in receive buffer. */
		array<Byte>^ ReceivedData = gcnew array<Byte>(m_socket->Available);
		m_socket->Receive(ReceivedData);

		return ReceivedData;
	}

	array<Byte>^ HLAE_WS_Client::ReturnWebsocketPayload(array<Byte>^ FullFrame)
	{
		Byte OpCode = GetBits(FullFrame[0], 4, 7);

		if (OpCode == 8) // close frame
		{
			m_socket->Disconnect(true);
			Console::WriteLine("Client '" + m_identifier + "' has disconnected from '"
				+ s_identifier + "'.");
		}
		else if (OpCode == 2) // Binary Frame (what we want)
		{
			int it = 0; // keep track of where we are
			// calculate payload length
			UInt32 PayloadLength;

			if (GetBits(FullFrame[1], 1, 7) < 126)
			{
				PayloadLength = GetBits(FullFrame[1], 1, 7);
				it = 2;
			}
			else if (GetBits(FullFrame[1], 1, 7) == 126)
			{
				PayloadLength = TwoByteUInt16BE(FullFrame, 2);
				it = 4;
			}
			else if (GetBits(FullFrame[1], 1, 7) == 127)
			{
				PayloadLength = static_cast<UInt32>(EightByteUInt64BE(FullFrame, 2));
				it = 10;
			}

			// Get mask
			array<Byte>^ Mask = gcnew array<Byte>(4);
			for (int i = 0; i < 4; i++)
			{
				Mask[i] = FullFrame[it];
				it++;
			}

			// Get and decode data
			array<Byte>^ DecodedData = gcnew array<Byte>(PayloadLength);
			/* caveat, creating this array with a size greater than 32 unsigned bits
			causes deprecation of data... cli c++ doesn't allow arrays to be created
			any larger than 32 bits... this shouldn't be an issue with our server
			as no packet should every hold that much data, but still a potential
			problem with larger data (i.e. file transfer) */

			unsigned int j = 0;
			while (j < PayloadLength)
			{
				DecodedData[j] = FullFrame[it] ^ Mask[j % 4];
				it++; j++;
			}

			return DecodedData;
		}
	}

	Byte HLAE_WS_Client::GetBits(Byte byte, int start, int end)
	{
		/* Gets bits from start -> end and pushes them to the far right. */
		Byte MASK = 0;
		for (int i = start; i <= end; i++)
		{
			MASK += (1 << (end - i));
		}

		MASK <<= (7 - end);

		Byte ReturnValue = byte & MASK;
		ReturnValue >>= (7 - end);

		return ReturnValue;
	}

	Byte HLAE_WS_Client::GetByteFromUInt64(UInt64 INT, int byte)
	{
		/* Gets Byte from int and pushes it to the far right. */
		UInt64 MASK = 0b11111111;
		MASK <<= 8 * (7 - byte);

		UInt64 ReturnInt = INT & MASK;
		ReturnInt >>= 8 * (7 - byte);

		Byte *ReturnByte = (Byte*)&ReturnInt;
		
		return *ReturnByte;
	}

	int HLAE_WS_Client::StartAfter(array<Byte>^ data, String^ key)
	{
		/* Searches for first 'key' in 'data' and returns location of first character
		after key.*/
		std::string DataStr = marshal_as<std::string>(Encoding::UTF8->GetString(data));
		std::string KeyStr = marshal_as<std::string>(key);

		std::size_t it = DataStr.find(KeyStr) + KeyStr.length();
		
		return it;
	}

	void HLAE_WS_Client::DoHandShake(array<Byte>^ data)
	{
		/* Performs handshake with client. */

		// Get Key
		int KeyIt = StartAfter(data, "Sec-WebSocket-Key: ");
		
		String^ key;
		String^ DataStr = Encoding::UTF8->GetString(data);

		while (DataStr[KeyIt] != '\r')
		{
			key += DataStr[KeyIt];
			KeyIt++;
		}

		Console::WriteLine(m_identifier + "->" + s_identifier + " | Handshake Request Key: " + key);

		// Calculate Return Key
		String^ KeyReturn;
		String^ MagicString = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

		KeyReturn = key + MagicString;
		array<Byte>^ tmp = Encoding::UTF8->GetBytes(KeyReturn);
		SHA1^ sha = gcnew SHA1CryptoServiceProvider;
		array<Byte>^ tmp2 = sha->ComputeHash(tmp);
		KeyReturn = Convert::ToBase64String(tmp2);
		
		Console::WriteLine(s_identifier + "->" + m_identifier + " | Handshake Accept Key: " + KeyReturn);

		// Create Response
		String^ ReturnData = "HTTP/1.1 101 Switching Protocols" + Environment::NewLine
			+ "Upgrade: websocket" + Environment::NewLine
			+ "Connection: Upgrade" + Environment::NewLine
			+ "Sec-WebSocket-Accept: " + KeyReturn + Environment::NewLine
			+ Environment::NewLine;

		array<Byte>^ ReturnCharData = Encoding::UTF8->GetBytes(ReturnData);

		// Send Response
		SendData(ReturnCharData);
	}

	void HLAE_WS_Client::SendData(array<Byte>^ data)
	{
		/* Sends data to client asynchronously (fuck that word's a mouthfull) */
		SocketAsyncEventArgs^ SocketArgs = gcnew SocketAsyncEventArgs;
		SocketArgs->SetBuffer(data, 0, data->Length);

		m_socket->SendAsync(SocketArgs);
	}

	float HLAE_WS_Client::FourByteFloatLE(array<Byte>^ data, int offset)
	{
		UInt32 ReturnInt = (data[offset]) | (data[offset + 1] << 8)
			| (data[offset + 2] << 16) | (data[offset + 3] << 24);

		float *ReturnFloat = (float *)&ReturnInt;
		return *ReturnFloat;
	}

	UInt32 HLAE_WS_Client::FourByteUInt32LE(array<Byte>^ data, int offset)
	{
		UInt32 ReturnData = 0;
		for (int i = offset; i < offset + 4; i++)
		{
			int powFactor = static_cast<int>(pow(2, 8 * (i - offset)));
			ReturnData += data[i] * powFactor;
		}
		return ReturnData;
	}

	UInt16 HLAE_WS_Client::TwoByteUInt16BE(array<Byte>^ data, int offset)
	{
		UInt16 ReturnData = 0;
		for (int i = offset; i < offset + 2; i++)
		{
			int powFactor = static_cast<int>(pow(2, 8 * (1 - (i - offset))));
			ReturnData += data[i] * powFactor;
		}
		return ReturnData;
	}

	UInt64 HLAE_WS_Client::EightByteUInt64BE(array<Byte>^ data, int offset)
	{
		UInt16 ReturnData = 0;
		for (int i = offset; i < offset + 8; i++)
		{
			int powFactor = static_cast<int>(pow(2, 8 * (7 - (i - offset))));
			ReturnData += data[i] * powFactor;
		}
		return ReturnData;
	}

	/*  Public Variables  */

	inline String^ HLAE_WS_Client::Name() { return m_identifier; }
	void HLAE_WS_Client::Name(String^ name)
	{
		Console::WriteLine("Client '" + m_identifier + "' has been renamed to '" + name + "'.");
		m_identifier = name;
	}

}
