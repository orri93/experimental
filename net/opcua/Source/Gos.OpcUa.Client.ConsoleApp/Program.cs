using LibUA.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gos.OpcUa.Client.ConsoleApp
{
    class Program
    {
        private const string ApplicationUri = "urn:DemoApplication";
        private const string ProductUri = "uri:DemoApplication";
        private const string ApplicationName = "UA SDK client";
        private const string LocaleId = "en";

        private static string[] LocaleIds = new[] { LocaleId };

        static void Main(string[] args)
        {
            ApplicationDescription applicationDescription = new ApplicationDescription(
                ApplicationUri,
                ProductUri,
                new LocalizedText(ApplicationName),
                ApplicationType.Client,
                null, null, null);

            ApplicationDescription[] applicationDescriptions = null;
            EndpointDescription[] endpointDescriptions = null;

            ExampleClient client = new ExampleClient("127.0.0.1", 7718, 1000);
            client.Connect();
            client.OpenSecureChannel(MessageSecurityMode.None, SecurityPolicy.None, null);
            client.FindServers(out applicationDescriptions, LocaleIds);
            client.GetEndpoints(out endpointDescriptions, LocaleIds);
            client.Disconnect();

            // Check matching message security mode and security policy too
            // Lazy way to find server certificate is just grab any endpoint with one
            byte[] serverCertificate = endpointDescriptions
                .First(e => e.ServerCertificate != null && e.ServerCertificate.Length > 0)
                .ServerCertificate;
            string usernamePolicyDescription = endpointDescriptions
                .First(e => e.UserIdentityTokens.Any(t => t.TokenType == UserTokenType.UserName))
                .UserIdentityTokens.First(t => t.TokenType == UserTokenType.UserName)
                .PolicyId;

            client.Dispose();
        }
    }
}
