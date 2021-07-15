using LibUA.Core;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace Gos.OpcUa
{
    public class Client : LibUA.Client
    {
        private General.Cryptography cryptography;

        public override X509Certificate2 ApplicationCertificate
        {
            get { return cryptography.Certificate; }
        }

        public override RSACryptoServiceProvider ApplicationPrivateKey
        {
            get { return cryptography.PrivateKey; }
        }

        public Client(string Target, int Port, int Timeout) : base(Target, Port, Timeout)
        {
            LoadCertificateAndPrivateKey();
        }

        public override void NotifyDataChangeNotifications(
            uint subscrId,
            uint[] clientHandles,
            DataValue[] notifications)
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

        public override void NotifyEventNotifications(
            uint subscrId,
            uint[] clientHandles,
            object[][] notifications)
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
                string clientKeyText = this.cryptography.LoadCertificateAndPrivateKey(
                    General.Client.Constants.CertificateFileName,
                    General.Client.Constants.PrivateKeyFileName);
                RSAParameters rsaPrivateParameters = LibUA.UASecurity.ImportRSAPrivateKey(clientKeyText);
                this.cryptography.ImportPrivateKey(rsaPrivateParameters);
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
                General.Client.Constants.DefaultName,
                X500DistinguishedNameFlags.UseSemicolons);
            Uri uri = new Uri(General.Client.Constants.DefaultUri);
            RSAParameters rsaPrivateParameters =
                this.cryptography.CreateCertificateAndPrivateKey(name, uri);
            File.WriteAllText(
                General.Client.Constants.CertificateFileName,
                LibUA.UASecurity.ExportPEM(this.cryptography.Certificate));
            File.WriteAllText(
                General.Client.Constants.PrivateKeyFileName,
                LibUA.UASecurity.ExportRSAPrivateKey(rsaPrivateParameters));
        }
    }
}
