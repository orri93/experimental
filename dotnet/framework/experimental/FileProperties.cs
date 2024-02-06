using System;
using System.IO;
using System.Threading;

namespace experimental
{
    internal class FileProperties
    {
        private const string Path = "file.txt";
        private const string NonePath = "file.xml";

        public static void Experiment()
        {
            DateTime minValue = DateTime.MinValue;
            DateTime nautct = File.GetLastAccessTimeUtc(NonePath);
            TimeSpan nts = DateTime.UtcNow.Subtract(nautct);

            if (!File.Exists(Path))
            {
                CreateFile(Path);

                Thread.Sleep(1000);
                ReportFileTimes(Path);

                AccessFile(Path);
                Thread.Sleep(1000);
                ReportFileTimes(Path);

                ModifyFile(Path);
                Thread.Sleep(1000);
                ReportFileTimes(Path);

                DeleteFile(Path);
            }
            else
            {
                Console.Error.WriteLine("The file '{0}' already exists", Path);
            }
        }

        private static void CreateFile(string path)
        {
            Console.WriteLine("Creating the file '{0}'", path);

            using (StreamWriter writer = new StreamWriter(path))
            {
                writer.WriteLine("This is a test file");
            }
        }

        private static void AccessFile(string path)
        {
            Console.WriteLine("Reading the file '{0}'", path);

            using (StreamReader reader = new StreamReader(path))
            {
                string line = reader.ReadLine();
                Console.WriteLine("File content is '{0}'", line);
            }
        }

        private static void ModifyFile(string path)
        {
            Console.WriteLine("Adding content to file '{0}", path);

            using (StreamWriter writer = new StreamWriter(path, true))
            {
                writer.WriteLine("This is another line");
            }
        }

        private static void DeleteFile(string path)
        {
            Console.WriteLine("Deleting the file '{0}'", path);

            File.Delete(path);
        }

        private static void ReportFileTimes(string path)
        {
            DateTime cutct = File.GetCreationTimeUtc(Path);
            DateTime mutct = File.GetLastWriteTimeUtc(Path);
            DateTime autct = File.GetLastAccessTimeUtc(Path);
            DateTime ct = DateTime.UtcNow;

            TimeSpan cts = ct - cutct;
            TimeSpan mts = ct - mutct;
            TimeSpan ats = ct - autct;

            Console.WriteLine("Files creation time: {0} or {1} ago", cutct, cts);
            Console.WriteLine("Files last modified time: {0} or {1} ago", mutct, mts);
            Console.WriteLine("Files last access time: {0} or {1} ago", autct, ats);
        }
    }
}
