using LibUA;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gos.OpcUa
{
    public class Logger : LibUA.ILogger
    {
        public bool HasLevel(LogLevel Level)
        {
            return true;
        }

        public void LevelSet(LogLevel Mask)
        {
        }

        public void Log(LogLevel Level, string Str)
        {
            Console.WriteLine("[{0}] {1}", Level.ToString(), Str);
        }
    }
}
