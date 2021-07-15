using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace Gos.OpcUa.General
{
    public class Cryptography
    {
        public X509Certificate2 Certificate { get; private set; }
        public RSACryptoServiceProvider PrivateKey { get; private set; }

        public string LoadCertificateAndPrivateKey(string CertificateFileName, string PrivateKeyFileName)
        {
            // Try to load existing (public key) and associated private key
            this.Certificate = new X509Certificate2(CertificateFileName);
            this.PrivateKey = new RSACryptoServiceProvider();
            return File.ReadAllText(PrivateKeyFileName);
        }

        public void ImportPrivateKey(RSAParameters Parameters)
        {
            this.PrivateKey.ImportParameters(Parameters);
        }

        public RSAParameters CreateCertificateAndPrivateKey(X500DistinguishedName Name, Uri Uri)
        {
            SubjectAlternativeNameBuilder builder = new SubjectAlternativeNameBuilder();
            builder.AddUri(Uri);
            using (RSA rsa = RSA.Create(2048))
            {
                CertificateRequest request = new CertificateRequest(
                    Name,
                    rsa,
                    HashAlgorithmName.SHA256,
                    RSASignaturePadding.Pkcs1);
                request.CertificateExtensions.Add(builder.Build());
                X509Certificate2 certificate = request.CreateSelfSigned(
                    new DateTimeOffset(DateTime.UtcNow.AddDays(-1)),
                    new DateTimeOffset(DateTime.UtcNow.AddDays(3650)));
                this.Certificate = new X509Certificate2(
                    certificate.Export(X509ContentType.Pfx, ""),
                    "",
                    X509KeyStorageFlags.DefaultKeySet);
                RSAParameters rsaPrivateParameters = rsa.ExportParameters(true);
                this.PrivateKey = new RSACryptoServiceProvider();
                this.PrivateKey.ImportParameters(rsaPrivateParameters);
                return rsaPrivateParameters;
            }
        }
    }
}
