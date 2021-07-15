using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gos.OpcUa.UsingLocalLib.Client.ConsoleApp
{
    class Program
    {
        static int Main(string[] args)
        {
            return Gos.OpcUa.Client.Experimental.Execute(args);
        }
    }
}
