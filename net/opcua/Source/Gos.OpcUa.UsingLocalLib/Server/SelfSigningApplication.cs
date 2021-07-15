using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace Gos.OpcUa.Server
{
    public class SelfSigningApplication : LibUA.Server.Application
    {
        private General.Cryptography cryptography;

        public override X509Certificate2 ApplicationCertificate
        {
            get { return this.cryptography.Certificate; }
        }

        public override RSACryptoServiceProvider ApplicationPrivateKey
        {
            get { return this.cryptography.PrivateKey; }
        }

        public SelfSigningApplication()
        {
            this.cryptography = new General.Cryptography();
            LoadCertificateAndPrivateKey();
        }

        private void LoadCertificateAndPrivateKey()
        {
            try
            {
                // Try to load existing (public key) and associated private key
                string serverKeyText = this.cryptography.LoadCertificateAndPrivateKey(
                    General.Server.Constants.CertificateFileName,
                    General.Server.Constants.PrivateKeyFileName);
                RSAParameters rsaPrivateParameters = LibUA.UASecurity.ImportRSAPrivateKey(serverKeyText);
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
