using LibUA.Core;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace Gos.OpcUa.Client.ConsoleApp
{
    public class ExampleClient : LibUA.Client
    {
        private const string CertificateFileName = "Client.der";
        private const string PrivateKeyFileName = "Client.pem";
        private const string DefaultName = "CN=Client certificate;OU=Demo organization";
        private const string DefaultUri = "urn:DemoApplication";

        private X509Certificate2 applicationCertificate = null;
        private RSACryptoServiceProvider applicationPrivateKey = null;

        public override X509Certificate2 ApplicationCertificate
        {
            get { return applicationCertificate; }
        }

        public override RSACryptoServiceProvider ApplicationPrivateKey
        {
            get { return applicationPrivateKey; }
        }

        public ExampleClient(string Target, int Port, int Timeout) : base(Target, Port, Timeout)
        {
            LoadCertificateAndPrivateKey();
        }

        public override void NotifyDataChangeNotifications(uint subscrId, uint[] clientHandles, DataValue[] notifications)
        {
            for (int i = 0; i < clientHandles.Length; i++)
            {
                Console.WriteLine(
                    "subscrId {0} handle {1}: {2}",
                    subscrId,
                    clientHandles[i],
                    notifications[i].Value.ToString());
            }
        }

        public override void NotifyEventNotifications(uint subscrId, uint[] clientHandles, object[][] notifications)
        {
            for (int i = 0; i < clientHandles.Length; i++)
            {
                Console.WriteLine(
                    "subscrId {0} handle {1}: {2}",
                    subscrId,
                    clientHandles[i],
                    string.Join(",", notifications[i]));
            }
        }

        private void LoadCertificateAndPrivateKey()
        {
            try
            {
                // Try to load existing (public key) and associated private key
                this.applicationCertificate = new X509Certificate2(CertificateFileName);
                this.applicationPrivateKey = new RSACryptoServiceProvider();
                string clientKeyText = File.ReadAllText(PrivateKeyFileName);
                RSAParameters rsaPrivateParameters = LibUA.UASecurity.ImportRSAPrivateKey(clientKeyText);
                this.applicationPrivateKey.ImportParameters(rsaPrivateParameters);
            }
            catch
            {
                // Make a new certificate (public key) and associated private key
                CreateCertificateAndPrivateKey();
            }
        }

        private void CreateCertificateAndPrivateKey()
        {
            X500DistinguishedName name = new X500DistinguishedName(
                DefaultName,
                X500DistinguishedNameFlags.UseSemicolons);
            SubjectAlternativeNameBuilder builder = new SubjectAlternativeNameBuilder();
            builder.AddUri(new Uri(DefaultUri));
            using (RSA rsa = RSA.Create(2048))
            {
                CertificateRequest request = new CertificateRequest(
                    name,
                    rsa,
                    HashAlgorithmName.SHA256,
                    RSASignaturePadding.Pkcs1);
                request.CertificateExtensions.Add(builder.Build());
                X509Certificate2 certificate = request.CreateSelfSigned(
                    new DateTimeOffset(DateTime.UtcNow.AddDays(-1)),
                    new DateTimeOffset(DateTime.UtcNow.AddDays(3650)));
                this.applicationCertificate = new X509Certificate2(
                    certificate.Export(X509ContentType.Pfx, ""),
                    "",
                    X509KeyStorageFlags.DefaultKeySet);
                RSAParameters rsaPrivateParameters = rsa.ExportParameters(true);
                File.WriteAllText(
                    CertificateFileName,
                    LibUA.UASecurity.ExportPEM(this.applicationCertificate));
                File.WriteAllText(
                    PrivateKeyFileName,
                    LibUA.UASecurity.ExportRSAPrivateKey(rsaPrivateParameters));
            }
        }
    }
}
