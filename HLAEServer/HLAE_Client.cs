using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Security.Cryptography;
using System.Text;

namespace HLAEServer
{
    public class HLAE_WS_Client
    {
        // Constructors, Destructor, Overloads

        /// <summary>Creates an HLAE Client from incoming TCP request to HLAESERVER.</summary>
        /// <param name="identifier">Identifier for easy identification of client</param>
        /// <param name="server">Server we are accepting the client from.</param>
        public HLAE_WS_Client(string identifier, HLAE_WS_Server server)
        {
            m_socket = server.Socket();
            m_identifier = identifier;
            s_identifier = server.Name();

            Console.WriteLine("Client '" + identifier + "' is establishing a connection with '" + s_identifier + "'...");

            byte[] HandShakeData = GetAllDataInBuffer();
            DoHandShake(HandShakeData);

            Console.WriteLine("Client '" + identifier + "' has connected to '" + s_identifier + "'.");
        }

        ~HLAE_WS_Client() { m_socket.Disconnect(false); }

        // Public Functions

        /// <summary>Returns true if there is a connected client.
        /// Returns false otherwise.</summary>
        public bool Connected()
        {
            SendPing(); // We'll ping the server to update Connected.
            return m_socket.Connected;
        }

        /// <summary>Reads current data in the client buffer and updates
        /// variables accordingly.<para/>
        /// Should the client send a close connection request, you must
        /// reinitialize the client.</summary>
        public void ReadDataWS()
        {
            // catch no client
            if (m_socket.Connected == false)
                return;



            int it = 0; // keep track of where we are in the data

            byte[] data = GetDataInBuffer(); // get data from buffer

            if (data.Length == 0) // catch an empty buffer
                return;

            byte[] WebSocketDataRaw = ReturnWebsocketPayload(data); // get decoded payload from data

            if (WebSocketDataRaw.Length == 0) // catch an empty payload
                return;

            string WebSocketData = Encoding.UTF8.GetString(WebSocketDataRaw);
            /* currently handles all data that hlae sends as of mirv_pgl ver 2 */
            string CMD;
            while (it < WebSocketData.Length)
            {
                // Look for HLAE Command
                CMD = "";
                while ((WebSocketData[it] != '\0') && (it < WebSocketData.Length))
                {
                    CMD += WebSocketData[it];
                    it++;
                }
                it++;

                // Handle Command
                if (CMD == "hello")
                {
                    Console.WriteLine(m_identifier + "->" + s_identifier + " | hello");

                    HLAE_Version = FourByteUInt32LE(WebSocketDataRaw, it);
                    it += 4;
                    Console.WriteLine(m_identifier + "->" + s_identifier + " | version = " + HLAE_Version);

                    continue;
                }

                if (CMD == "dataStart")
                {
                    Console.WriteLine(m_identifier + "->" + s_identifier + " | dataStart");
                    continue;
                }

                if (CMD == "dataStop")
                {
                    Console.WriteLine(m_identifier + "->" + s_identifier + " | dataStop");
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
                    Console.WriteLine(m_identifier + "->" + s_identifier + " | levelShutdown");
                    continue;
                }

                if (CMD == "cam")
                {
                    Console.WriteLine(m_identifier + "->" + s_identifier + " | CAM DATA");

                    HLAE_Time = FourByteFloatLE(WebSocketDataRaw, it);
                    Console.WriteLine("Time: " + HLAE_Time);
                    it += 4;

                    HLAE_Xpos = FourByteFloatLE(WebSocketDataRaw, it);
                    Console.WriteLine("X: " + HLAE_Xpos);
                    it += 4;

                    HLAE_Ypos = FourByteFloatLE(WebSocketDataRaw, it);
                    Console.WriteLine("Y: " + HLAE_Ypos);
                    it += 4;

                    HLAE_Zpos = FourByteFloatLE(WebSocketDataRaw, it);
                    Console.WriteLine("Z: " + HLAE_Zpos);
                    it += 4;

                    HLAE_Xrot = FourByteFloatLE(WebSocketDataRaw, it);
                    Console.WriteLine("X-Rot: " + HLAE_Xrot);
                    it += 4;

                    HLAE_Yrot = FourByteFloatLE(WebSocketDataRaw, it);
                    Console.WriteLine("Y-Rot: " + HLAE_Yrot);
                    it += 4;

                    HLAE_Zrot = FourByteFloatLE(WebSocketDataRaw, it);
                    Console.WriteLine("Z-Rot: " + HLAE_Zrot);
                    it += 4;

                    HLAE_fov = FourByteFloatLE(WebSocketDataRaw, it);
                    Console.WriteLine("FoV [real]: " + HLAE_fov);
                    it += 4;

                    continue;
                }
            }
        }

        /// <summary>Send 'data' as a command to the client.</summary>
        void SendDataWS(string data)
        {
            string FormattedData = "exec\0" + data + "\0";
            byte[] Payload = Encoding.UTF8.GetBytes(FormattedData);
            ulong PayloadLength = (ulong)Payload.Length; // exec\0 + data + \0
            byte[] ReturnBytes;

            int it = 0; // Keep track of where we are

            if (PayloadLength < 126)
            {
                ReturnBytes = new byte[2 + (int)PayloadLength];
                ReturnBytes[0] = 0b10000010;
                ReturnBytes[1] = (byte)PayloadLength;
                it = 2;
            }
            else if (PayloadLength >= 126 && PayloadLength <= 65536)
            {
                ReturnBytes = new byte[4 + (int)PayloadLength];
                ReturnBytes[0] = 0b10000010;
                ReturnBytes[1] = 126;
                ReturnBytes[3] = (byte)(PayloadLength % 256);
                ReturnBytes[2] = (byte)(PayloadLength - ReturnBytes[3]);
                it = 4;
            }
            else
            {
                ReturnBytes = new byte[10 + (int)PayloadLength];
                ReturnBytes[0] = 0b10000010;
                ReturnBytes[1] = 127;

                for (int i = 2; i < 10; i++)
                    ReturnBytes[i] = GetByteFromUInt64(PayloadLength, i - 2);

                it = 10;
            }

            int j = 0; // to make sure we don't null reference our string
            while (it < ReturnBytes.Length && j < Payload.Length)
            {
                ReturnBytes[it] = Payload[j];
                it++; j++;
            }

            // Send the final data.
            SendData(ReturnBytes);
        }

        // Private 'helper' Functions
        private byte[] GetAllDataInBuffer()
        {
            /* Returns the current data in receive buffer. */
            byte[] ReceivedData = new byte[m_socket.Available];
            m_socket.Receive(ReceivedData);

            return ReceivedData;
        }

        private byte[] GetDataInBuffer()
        {
            /* Returns the current data in receive buffer. */
            byte[] ReceivedData = new byte[m_socket.Available];
            m_socket.Receive(ReceivedData);

            return ReceivedData;
        }

        private byte[] ReturnWebsocketPayload(byte[] FullFrame)
        {
            byte OpCode = GetBits(FullFrame[0], 4, 7);

            if (OpCode == 8) // close frame
            {
                m_socket.Disconnect(true);
                Console.WriteLine("Client '" + m_identifier + "' has disconnected from '"
                    + s_identifier + "'.");
            }
            else if (OpCode == 2) // Binary Frame (what we want)
            {
                int it = 0; // keep track of where we are
                            // calculate payload length
                uint PayloadLength = 0;

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
                    PayloadLength = (uint)(EightByteUInt64BE(FullFrame, 2));
                    it = 10;
                }

                // Get mask
                byte[] Mask = new byte[4];
                for (int i = 0; i < 4; i++)
                {
                    Mask[i] = FullFrame[it];
                    it++;
                }

                // Get and decode data
                byte[] DecodedData = new byte[PayloadLength];
                /* caveat, creating this array with a size greater than 32 unsigned bits
                causes deprecation of data... cli c++ doesn't allow arrays to be created
                any larger than 32 bits... this shouldn't be an issue with our server
                as no packet should every hold that much data, but still a potential
                problem with larger data (i.e. file transfer) */

                uint j = 0;
                while (j < PayloadLength)
                {
                    DecodedData[j] = (byte)(FullFrame[it] ^ Mask[j % 4]);
                    it++; j++;
                }

                return DecodedData;
            }

            // bad design but we should hopefully never reach this return ¯\_(ツ)_/¯
            return null;
        }

        private byte GetBits(byte Byte, int start, int end)
        {
            /* Gets bits from start -> end and pushes them to the far right. */
            byte MASK = 0;
            for (int i = start; i <= end; i++)
            {
                MASK += (byte)(1 << (end - i));
            }

            MASK <<= (7 - end);

            byte ReturnValue = (byte)(Byte & MASK);
            ReturnValue >>= (7 - end);

            return ReturnValue;
        }

        private byte GetByteFromUInt64(ulong INT, int Byte)
        {
            /* Gets Byte from int and pushes it to the far right. */
            ulong MASK = 0b11111111;
            MASK <<= 8 * (7 - Byte);

            ulong ReturnInt = INT & MASK;
            ReturnInt >>= 8 * (7 - Byte);

            byte ReturnByte = (byte)ReturnInt;

            return ReturnByte;
        }

        private int StartAfter(byte[] data, string key)
        {
            /* Searches for first 'key' in 'data' and returns location of first character
		after key.*/
            string DataStr = Encoding.UTF8.GetString(data);
            string KeyStr = key;

            int it = DataStr.IndexOf(KeyStr) + KeyStr.Length;

            return it;
        }

        private void DoHandShake(byte[] data)
        {
            /* Performs handshake with client. */

            // Get Key
            int KeyIt = StartAfter(data, "Sec-WebSocket-Key: ");

            string key = "";
            string DataStr = Encoding.UTF8.GetString(data);

            while (DataStr[KeyIt] != '\r')
            {
                key += DataStr[KeyIt];
                KeyIt++;
            }

            Console.WriteLine(m_identifier + "->" + s_identifier + " | Handshake Request Key: " + key);

            // Calculate Return Key
            string KeyReturn;
            string MagicString = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

            KeyReturn = key + MagicString;
            byte[] tmp = Encoding.UTF8.GetBytes(KeyReturn);
            SHA1 sha = new SHA1CryptoServiceProvider();
            byte[] tmp2 = sha.ComputeHash(tmp);
            KeyReturn = Convert.ToBase64String(tmp2);

            Console.WriteLine(s_identifier + "->" + m_identifier + " | Handshake Accept Key: " + KeyReturn);

            // Create Response
            string ReturnData = "HTTP/1.1 101 Switching Protocols" + Environment.NewLine
                + "Upgrade: websocket" + Environment.NewLine
                + "Connection: Upgrade" + Environment.NewLine
                + "Sec-WebSocket-Accept: " + KeyReturn + Environment.NewLine
                + Environment.NewLine;

            byte[] ReturnCharData = Encoding.UTF8.GetBytes(ReturnData);

            // Send Response
            SendData(ReturnCharData);
        }

        private void SendData(byte[] data)
        {
            /* Sends data to client asynchronously (fuck that word's a mouthfull) */
            SocketAsyncEventArgs SocketArgs = new SocketAsyncEventArgs();
            SocketArgs.SetBuffer(data, 0, data.Length);

            m_socket.SendAsync(SocketArgs);
        }

        private void SendPing()
        {
            int Code = 0b10001001;
            byte[] Payload = new byte[2];
            Payload[0] = (byte)Code;
            Payload[1] = 0; // Mask = 0, Payload Length = 0.

            SendData(Payload);
        }

        private float FourByteFloatLE(byte[] data, int offset)
        {
            uint ReturnInt = (uint)((data[offset]) | (data[offset + 1] << 8)
            | (data[offset + 2] << 16) | (data[offset + 3] << 24));

            float ReturnFloat = (float)ReturnInt;
            return ReturnFloat;
        }

        private uint FourByteUInt32LE(byte[] data, int offset)
        {
            uint ReturnData = 0;
            for (int i = offset; i < offset + 4; i++)
            {
                int powFactor = (int)(Math.Pow(2, 8 * (i - offset)));
                ReturnData += (uint)(data[i] * powFactor);
            }
            return ReturnData;
        }

        private ushort TwoByteUInt16BE(byte[] data, int offset)
        {
            ushort ReturnData = 0;
            for (int i = offset; i < offset + 2; i++)
            {
                int powFactor = (int)(Math.Pow(2, 8 * (1 - (i - offset))));
                ReturnData += (ushort)(data[i] * powFactor);
            }
            return ReturnData;
        }

        private ulong EightByteUInt64BE(byte[] data, int offset)
        {
            ulong ReturnData = 0;
            for (int i = offset; i < offset + 8; i++)
            {
                int powFactor = (int)(Math.Pow(2, 8 * (7 - (i - offset))));
                ReturnData += (ulong)(data[i] * powFactor);
            }
            return ReturnData;
        }

        // Public Variables

		/// <summary>Gets identifier.</summary>
		public string Name() { return m_identifier; }

        /// <summary>Sets identifier.</summary>
        public void Name(string name)
        {
            Console.WriteLine("Client '" + m_identifier + "' has been renamed to '" + name + "'.");
            m_identifier = name;
        }

        // Client Values
        /// <summary>Client mirv_pgl version as last reported by client.</summary>
        public uint HLAE_Version;
        /// <summary>Client level as last reported by client.</summary>
        public string HLAE_Level;

        /// <summary>Client time as last reported by client.</summary>
        public float HLAE_Time;
        /// <summary>Client X Position as last reported by client.</summary>
        public float HLAE_Xpos;
        /// <summary>Client Y Position as last reported by client.</summary>
        public float HLAE_Ypos;
        /// <summary>Client Z Position as last reported by client.</summary>
        public float HLAE_Zpos;
        /// <summary>Client X Rotation as last reported by client.</summary>
        public float HLAE_Xrot;
        /// <summary>Client Y Rotation as last reported by client.</summary>
        public float HLAE_Yrot;
        /// <summary>Client Z Rotation as last reported by client.</summary>
        public float HLAE_Zrot;
        /// <summary>Client Field of View as last reported by client.</summary>
        public float HLAE_fov;

        // Private Variables
		private Socket m_socket;
		private string m_identifier;
		private string s_identifier;
    }
}
