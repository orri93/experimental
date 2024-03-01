using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace experimental
{
    internal class TimeCalculations
    {
        public static void Experiment()
        {
            const int number = 14;

            const int packetTick = 2000000;

            const int deltaTick = packetTick / number;

            TimeSpan packetSpan = new TimeSpan(packetTick);
            Debug.Assert(packetSpan.TotalMilliseconds == 200);

            TimeSpan deltaSpan = new TimeSpan(deltaTick);

            for (int order = 0; order < number; order++)
            {
                int correctionOrder = number - order - 1;

                TimeSpan correctionSpan = new TimeSpan(correctionOrder * deltaTick);
            }
        }
    }
}
