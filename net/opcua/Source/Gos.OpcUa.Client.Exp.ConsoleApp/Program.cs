using LibUA.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gos.OpcUa.Client.Exp.ConsoleApp
{
    class Program
    {
        private const int Success = 0;
        private const int ConnectionFailed = 1;

        private const string ApplicationUri = "urn:DemoApplication";
        private const string ProductUri = "uri:DemoApplication";
        private const string ApplicationName = "UA SDK client";
        private const string LocaleId = "en";

        private const string SessionName = ApplicationUri;

        private const string DefaultTarget = "127.0.0.1";
        private const int DefaultPort = 4840;

        private const int Timeout = 1000;
        private const int SessionTimeout = 120;

        private static string[] LocaleIds = new[] { LocaleId };

        static int Main(string[] args)
        {
            ApplicationDescription applicationDescription = new ApplicationDescription(
                ApplicationUri,
                ProductUri,
                new LocalizedText(ApplicationName),
                ApplicationType.Client,
                null, null, null);

            ApplicationDescription[] applicationDescriptions = null;
            EndpointDescription[] endpointDescriptions = null;

            string username = null;
            string password = null;
            string target = DefaultTarget;
            int port = DefaultPort;
            int timeout = Timeout;

            if (args.Length > 0)
            {
                target = args[0];
            }
            if (args.Length > 1)
            {
                if (!Int32.TryParse(args[1], out port))
                {
                    Console.Error.WriteLine(String.Format(
                        "Trying to parse '{0}' as port failed. Using default port {1}", args[1], port));
                }
            }
            if (args.Length > 3)
            {
                username = args[2];
                password = args[3];
            }
            if (args.Length > 4)
            {
                if (!Int32.TryParse(args[4], out timeout))
                {
                    Console.Error.WriteLine(String.Format(
                        "Trying to parse '{0}' as timeout failed. Using default timeout {1}", args[4], timeout));
                }
            }

            ExampleClient client = new ExampleClient(target, port, timeout);
            if (!GetServerInformation(client, out applicationDescriptions, out endpointDescriptions))
            {
                return ConnectionFailed;
            }

            // Create new client object to reset previous secure channel settings
            client = new ExampleClient(target, port, timeout);

            // Check matching message security mode and security policy too
            // Lazy way to find server certificate is just grab any endpoint with one
            if (endpointDescriptions != null)
            {
                byte[] serverCertificate = endpointDescriptions
                    .First(e => e.ServerCertificate != null && e.ServerCertificate.Length > 0)
                    .ServerCertificate;
                string usernamePolicyDescription = endpointDescriptions
                    .First(e => e.UserIdentityTokens.Any(t => t.TokenType == UserTokenType.UserName))
                    .UserIdentityTokens.First(t => t.TokenType == UserTokenType.UserName)
                    .PolicyId;
                if (!EstablishSecureConnection(
                    client,
                    applicationDescription,
                    username,
                    password,
                    usernamePolicyDescription,
                    serverCertificate))
                {
                    return ConnectionFailed;
                }
            }
            else
            {
                if (!EstablishConnection(client, applicationDescription, username, password))
                {
                    return ConnectionFailed;
                }
            }

            DataValue[] dataValues = null;
            ReadValues(client, out dataValues);

            BrowseResult[] browseResults;
            StatusCode browseResult = client.Browse(new BrowseDescription[]
            {
                new BrowseDescription(
                    new NodeId(2, 0),
                        BrowseDirection.Both,
                        NodeId.Zero,
                        true, 0xFFFFFFFFu, BrowseResultMask.All)
            }, 10000, out browseResults);

            uint[] responseStatuses;

            client.Disconnect();
            client.Dispose();

            return Success;
        }
        private static bool GetServerInformation(
            LibUA.Client client,
            out ApplicationDescription[] applicationDescriptions,
            out EndpointDescription[] endpointDescriptions)
        {
            StatusCode connectResult = client.Connect();
            if (Types.StatusCodeIsGood((uint)connectResult))
            {
                client.OpenSecureChannel(MessageSecurityMode.None, SecurityPolicy.None, null);
                client.FindServers(out applicationDescriptions, LocaleIds);
                client.GetEndpoints(out endpointDescriptions, LocaleIds);
                client.Disconnect();
                return true;
            }
            else
            {
                Console.Error.WriteLine(String.Format(
                    "Failed to connect to server '{0}' code: {1} ({2})",
                    client.Target,
                    connectResult,
                    (uint)connectResult));
            }
            applicationDescriptions = null;
            endpointDescriptions = null;
            return false;
        }

        private static bool EstablishConnection(
            LibUA.Client client,
            ApplicationDescription applicationDescription,
            string username,
            string password)
        {
            try
            {
                StatusCode connectResult = client.Connect();
                // Without security
                StatusCode openResult = client.OpenSecureChannel(
                    MessageSecurityMode.None,
                    SecurityPolicy.None,
                    null);
                return EstablishSession(
                        client,
                        applicationDescription,
                        username,
                        password,
                        "0");
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(String.Format(
                    "Failed to connect to server '{0}' exception: {1}",
                    client.Target,
                    ex.Message));
            }
            return false;
        }

        private static bool EstablishSecureConnection(
            LibUA.Client client,
            ApplicationDescription applicationDescription,
            string username,
            string password,
            string usernamePolicyDescription,
            byte[] serverCertificate)
        {
            try
            {
                StatusCode connectResult = client.Connect();
                StatusCode openResult;
                if (serverCertificate != null && usernamePolicyDescription != null)
                {
                    openResult = client.OpenSecureChannel(
                        MessageSecurityMode.SignAndEncrypt,
                        SecurityPolicy.Basic256Sha256,
                        serverCertificate);
                }
                else
                {
                    // Without security
                    openResult = client.OpenSecureChannel(
                        MessageSecurityMode.None,
                        SecurityPolicy.None,
                        null);
                }
                if (Types.StatusCodeIsGood((uint)openResult))
                {
                    return EstablishSession(
                        client,
                        applicationDescription,
                        username,
                        password,
                        usernamePolicyDescription);
                }
                else
                {
                    Console.Error.WriteLine(String.Format(
                        "Failed to Open Secure Channel to server '{0}' code: {1} ({2})",
                        client.Target,
                        openResult,
                        (uint)openResult));
                }
            }
            catch(Exception ex)
            {
                Console.Error.WriteLine(String.Format(
                    "Failed to connect to server '{0}' exception: {1}",
                    client.Target,
                    ex.Message));
            }
            return false;
        }

        private static bool EstablishSession(
            LibUA.Client client,
            ApplicationDescription applicationDescription,
            string username,
            string password,
            string usernamePolicyDescription)
        {
            try
            {
                StatusCode createResult = client.CreateSession(
                        applicationDescription,
                        SessionName,
                        SessionTimeout);
                Object token;
                if (String.IsNullOrWhiteSpace(username))
                {
                    token = new UserIdentityAnonymousToken("0");
                }
                else
                {
                    UTF8Encoding encoding = new UTF8Encoding();
                    byte[] buffer = encoding.GetBytes(password);
                    token = new UserIdentityUsernameToken(usernamePolicyDescription, username, buffer, Types.SignatureAlgorithmRsa15);
                }
                StatusCode activateResult = client.ActivateSession(token, LocaleIds);
                if (Types.StatusCodeIsGood((uint)activateResult))
                {
                    return true;
                }
                else
                {
                    Console.Error.WriteLine(String.Format(
                        "Failed to Activate Session to server '{0}' code: {1} ({2})",
                        client.Target,
                        activateResult,
                        (uint)activateResult));
                }
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(String.Format(
                    "Failed to establish session with server '{0}' exception: {1}",
                    client.Target,
                    ex.Message));
            }
            return false;
        }

        private static readonly NodeId NodeIdBitPosition = new NodeId(2, "Wbc.rs_bit_position");
        private static readonly NodeId NodeIdBlockHeight = new NodeId(2, "Wbc.rs_block_height");
        private static readonly NodeId NodeIdFlowIn = new NodeId(2, "Wbc.rs_flow_in");

        private static void ReadValues(LibUA.Client client, out DataValue[] dataValues)
        {
            ReadValueId[] readValueIds = new ReadValueId[]
            {
                new ReadValueId(NodeIdFlowIn, NodeAttribute.Value, null, new QualifiedName(0, null))
            };

            StatusCode readResult = client.Read(readValueIds, out dataValues);
            if (Types.StatusCodeIsBad((uint)readResult))
            {
                Console.Error.WriteLine(String.Format(
                    "Failed to Read from server '{0}' code: {1} ({2})",
                    client.Target,
                    readResult,
                    (uint)readResult));
            }
        }
    }
}
