using System;
using System.IO;
using System.Collections.Generic;
using System.Security.Cryptography;

namespace experimental
{
    internal class CryptoExperiment
    {
        public static void Experiment()
        {
            ExperimentA();
            ExperimentB();
        }

        private static void ExperimentA()
        {
            EncryptingDataA();
            DecryptingDataA();
        }

        private static void EncryptingDataA()
        {
            // Create a file stream
            using (FileStream fileStream = new FileStream("data.txt", FileMode.OpenOrCreate))
            {
                // Create an instance of the Aes class (Symmetric Algorithm
                using (Aes aes = Aes.Create())
                {
                    byte[] key =
                    {
                        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                        0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16
                    };
                    aes.Key = key;

                    byte[] iv = aes.IV;

                    ICryptoTransform encryptor = aes.CreateEncryptor();
                    fileStream.Write(iv, 0, iv.Length);

                    encryptor = aes.CreateEncryptor();
                    using (CryptoStream cryptoStream = new CryptoStream(fileStream, encryptor, CryptoStreamMode.Write))
                    {
                        using (StreamWriter streamWriter = new StreamWriter(cryptoStream))
                        {
                            streamWriter.WriteLine("Hello, World!");
                        }
                    }
                }
            }
        }

        private static void DecryptingDataA()
        {
            using (FileStream fileStream = new FileStream("data.txt", FileMode.Open))
            {
                using (Aes aes = Aes.Create())
                {
                    byte[] iv = new byte[aes.IV.Length];
                    int numBytesToRead = aes.IV.Length;
                    int numBytesRead = 0;
                    while (numBytesToRead > 0)
                    {
                        int n = fileStream.Read(iv, numBytesRead, numBytesToRead);
                        if (n == 0) break;

                        numBytesRead += n;
                        numBytesToRead -= n;
                    }

                    byte[] key =
                    {
                        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                        0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16
                    };

                    ICryptoTransform decryptor = aes.CreateDecryptor(key, iv);
                    using (CryptoStream cryptoStream = new CryptoStream(fileStream, decryptor, CryptoStreamMode.Read))
                    {
                        using (StreamReader streamReader = new StreamReader(cryptoStream))
                        {
                            string plaintext = streamReader.ReadToEnd();
                            Console.WriteLine($"The decrypted original message: {plaintext}");
                        }
                    }
                }
            }
        }

        private static void ExperimentB()
        {
            string msg = EncryptingDataB();
            DecryptingDataB(msg);
        }

        private static string EncryptingDataB()
        {
            using (MemoryStream memoryStream = new MemoryStream(255))
            {
                using (Aes aes = Aes.Create())
                {
                    byte[] key =
                    {
                        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                        0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16
                    };
                    aes.Key = key;

                    byte[] iv = aes.IV;

                    ICryptoTransform encryptor = aes.CreateEncryptor();
                    memoryStream.Write(iv, 0, iv.Length);

                    encryptor = aes.CreateEncryptor();
                    using (CryptoStream cryptoStream = new CryptoStream(memoryStream, encryptor, CryptoStreamMode.Write))
                    {
                        using (StreamWriter streamWriter = new StreamWriter(cryptoStream))
                        {
                            streamWriter.WriteLine("Hello, World!");
                        }
                    }
                }
                byte[] bytes = memoryStream.ToArray();
                return Convert.ToBase64String(bytes);
            }
        }

        private static void DecryptingDataB(string msg)
        {
            byte[] bytes = Convert.FromBase64String(msg);
            using (MemoryStream memoryStream = new MemoryStream(bytes))
            {
                using (Aes aes = Aes.Create())
                {
                    byte[] iv = new byte[aes.IV.Length];
                    int numBytesToRead = aes.IV.Length;
                    int numBytesRead = 0;
                    while (numBytesToRead > 0)
                    {
                        int n = memoryStream.Read(iv, numBytesRead, numBytesToRead);
                        if (n == 0) break;

                        numBytesRead += n;
                        numBytesToRead -= n;
                    }

                    byte[] key =
                    {
                        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                        0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16
                    };

                    ICryptoTransform decryptor = aes.CreateDecryptor(key, iv);
                    using (CryptoStream cryptoStream = new CryptoStream(memoryStream, decryptor, CryptoStreamMode.Read))
                    {
                        using (StreamReader streamReader = new StreamReader(cryptoStream))
                        {
                            string plaintext = streamReader.ReadToEnd();
                            Console.WriteLine($"The decrypted original message: {plaintext}");
                        }
                    }
                }
            }
        }
    }
}
