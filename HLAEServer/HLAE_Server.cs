using System;
using System.Net;
using System.Net.Sockets;

namespace HLAEServer
{
    public class HLAE_WS_Server
    {
        // Constructors, Destructor, Overloads

		/// <summary>Creates a listen server on specified IP and Port.</summary>
		/// <param name="LISTEN_ON_INIT">If set to true - start the server after initializing it.</param>
		/// <param name="identifier">Identifier for easy identification of server.</param>
		/// <param name="IPADDRESS">IP Address of type IPv4/IPv6 to listen on.</param>
		/// <param name="PORT">Port to listen on.</param>
		public HLAE_WS_Server(bool LISTEN_ON_INIT, string identifier, string IPADDRESS, int PORT)
        {
            // Set Member Functions
            m_identifier = identifier;
            m_ipaddress = IPADDRESS;
            m_port = PORT;
            m_listening = false;

            if (LISTEN_ON_INIT)
                StartListening();
        }

		~HLAE_WS_Server() { StopListening(); }

        // Public Functions

		/// <summary>Returns true if incoming TCP request(s).
		/// Returns false otherwise.</summary>
		public bool ClientAvailable() { return m_server.Pending(); }

        /// <summary>Start Listening<para/>
        /// If called while server is already listening
        /// will cause an exception.</summary>
        public void StartListening()
        {
            if (!m_listening)
            {
                // Setup Variables
                IPAddress serveraddr = IPAddress.Parse(m_ipaddress);
                m_server = new TcpListener(serveraddr, m_port);

                // Start Server
                m_server.Start();
                m_listening = true;
                Console.WriteLine("Server '" + m_identifier + "' has started listening on "
                    + m_ipaddress + ":" + m_port + "...");
            }
        }
        /// <summary>Stops Listening.</summary>
        public void StopListening()
        {
            if (m_listening == true)
            {
                m_server.Stop();
                m_listening = false;
                Console.WriteLine("Server '" + m_identifier + "' has stopped listening.");
            }
            else
            {
                Console.WriteLine("Server '" + m_identifier + "' isn't listening, cannot stop an already stopped server.");
            }
        }

		// Public Variables

		/// <summary>Returns Socket of incoming connection.<para/>
		/// Use when initializing HLAE_Client.</summary>
		public Socket Socket() { return m_server.AcceptSocket(); }
        /// <summary>Get identifier.</summary>
        public string Name() { return m_identifier; }
        /// <summary>Set identifier.</summary>
        public void Name(string name)
        {
            Console.WriteLine("Server '" + m_identifier + "' has been renamed to '" + name + "'.");
            m_identifier = name;
        }

        // Private Variables
		private bool m_listening;
        private int m_port;
        private string m_identifier;
        private string m_ipaddress;
		private TcpListener m_server;
    }
}
