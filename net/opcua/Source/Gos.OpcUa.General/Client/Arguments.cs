using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gos.OpcUa.General.Client
{
    public class Arguments
    {
        private const string DefaultTarget = "127.0.0.1";
        private const int DefaultPort = 4840;
        private const int DefaultTimeout = 1000;

        public bool UseCertificate { get; private set; }

        public string Target { get; private set; }

        public string Username { get; private set; }
        public string Password { get; private set; }

        public int Port { get; private set; }

        public int Timeout { get; private set; }

        public Arguments()
        {
            SetDefault();
        }

        public Arguments(string[] args)
        {
            Parse(args);
        }

        public void Parse(string[] args)
        {
            int at = 0;

            SetDefault();

            // First argument is special
            if (args.Length > 0)
            {
                if(String.Equals(args[0], "ssl", StringComparison.InvariantCultureIgnoreCase))
                {
                    this.UseCertificate = true;
                    at++;
                }                
            }

            if (args.Length > at)
            {
                Target = args[at++];
            }

            if (args.Length > at)
            {
                int port;
                if (Int32.TryParse(args[at++], out port))
                {
                    this.Port = port;
                }
                else
                {
                    Console.Error.WriteLine(String.Format(
                        "Trying to parse '{0}' as port failed. Using default port {1}", args[1], this.Port));
                }
            }
            if (args.Length > at + 1)
            {
                this.Username = args[at++];
                this.Password = args[at++];
            }
            if (args.Length > at)
            {
                int timeout;
                if (Int32.TryParse(args[at], out timeout))
                {
                    this.Timeout = timeout;
                }
                else
                {
                    Console.Error.WriteLine(String.Format(
                        "Trying to parse '{0}' as timeout failed. Using default timeout {1}", args[4], this.Timeout));
                }
            }
        }

        private void SetDefault()
        {
            this.UseCertificate = false;
            this.Target = DefaultTarget;
            this.Username = null;
            this.Password = null;
            this.Port = DefaultPort;
            this.Timeout = DefaultTimeout;
        }
    }
}
