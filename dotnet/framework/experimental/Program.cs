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
            //Statistics.ExperimentA();
            Statistics.ExperimentB(123);
            Statistics.ExperimentB(456);
            Statistics.ExperimentB(789);
            Statistics.ExperimentB(987);
            Statistics.ExperimentB(654);
            Statistics.ExperimentB(321);
            EratosthenesSieve.Experiment();
            ClockExperiment.Experiment();
            TimeCalculations.Experiment();
            RegularExpression.Experiment();
            FileProperties.Experiment();
        }
    }
}
