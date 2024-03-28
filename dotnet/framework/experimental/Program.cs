using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace experimental
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Statistics.Experiment();
            EratosthenesSieve.Experiment();
            ClockExperiment.Experiment();
            TimeCalculations.Experiment();
            RegularExpression.Experiment();
            FileProperties.Experiment();
        }
    }
}
