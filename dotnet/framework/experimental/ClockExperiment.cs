using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Security;

namespace experimental
{
    internal class ClockExperiment
    {
        [StructLayout(LayoutKind.Sequential)]
        struct FILE_TIME
        {
            public UInt32 ftTimeLow;
            public UInt32 ftTimeHigh;
        }

        struct Record
        {
            public long ticks;
            public DateTime time;
            public FILE_TIME asFileTime;
            public FILE_TIME preciseAsFileTime;
        }

        struct ElapsedRecord
        {
            public long ticks;
            public TimeSpan timeSpan;
            public UInt64 asFileTimeDifference;
            public UInt64 asPreciseFileTimeDifference;
        }

        [SuppressUnmanagedCodeSecurity, DllImport("kernel32.dll")]
        static extern void GetSystemTimeAsFileTime(out FILE_TIME lpSystemTimeAsFileTime);

        [SuppressUnmanagedCodeSecurity, DllImport("kernel32.dll")]
        static extern void GetSystemTimePreciseAsFileTime(out FILE_TIME lpSystemTimeAsFileTime);


        public static void Experiment()
        {
            const int CycleCount = 1000000;
            const int ReducedCycleCount = CycleCount - 1;

            Record[] records = new Record[CycleCount];
            ElapsedRecord[] elapsedRecords = new ElapsedRecord[ReducedCycleCount];

            Stopwatch stopwatch = new Stopwatch();

            // Taking the measurements
            for (int i = 0; i < CycleCount; i++)
            {
                stopwatch.Restart();
                records[i].time = DateTime.UtcNow;
                GetSystemTimeAsFileTime(out records[i].asFileTime);
                GetSystemTimePreciseAsFileTime(out records[i].preciseAsFileTime);
                //EratosthenesSieve.Sieve(10000, new List<int>());
                records[i].ticks = stopwatch.ElapsedTicks;
            }

            // Calculate the elapsed time
            for (int i = 1; i < CycleCount; i++)
            {
                elapsedRecords[i - 1].ticks = records[i].ticks;  // Ticks are already an elapsed time measurement
                elapsedRecords[i - 1].timeSpan = records[i].time - records[i - 1].time;
                elapsedRecords[i - 1].asFileTimeDifference = FileTimeToUInt64(records[i].asFileTime) - FileTimeToUInt64(records[i - 1].asFileTime);
                elapsedRecords[i - 1].asPreciseFileTimeDifference = FileTimeToUInt64(records[i].preciseAsFileTime) - FileTimeToUInt64(records[i - 1].preciseAsFileTime);
            }

            // Calculate the sum, min and max
            long minTick = long.MaxValue, maxTick = long.MinValue, minTimeTick = long.MaxValue, maxTimeTick = long.MinValue;
            UInt64 minAsFileTimeDifference = UInt64.MaxValue, maxAsFileTimeDifference = UInt64.MinValue,
                minAsPreciseFileTimeDifference = UInt64.MaxValue, maxAsPreciseFileTimeDifference = UInt64.MinValue;
            UInt64 tickSum = 0, timeTickSum = 0, asFileTimeDifferenceSum = 0, asPreciseFileTimeDifference = 0;
            for (int i = 0; i < ReducedCycleCount; i++)
            {
                tickSum += (UInt64)(elapsedRecords[i].ticks);
                if (elapsedRecords[i].ticks < minTick)
                {
                    minTick = elapsedRecords[i].ticks;
                }
                if (elapsedRecords[i].ticks > maxTick)
                {
                    maxTick = elapsedRecords[i].ticks;
                }

                timeTickSum += (UInt64)(elapsedRecords[i].timeSpan.Ticks);
                if (elapsedRecords[i].timeSpan.Ticks < minTimeTick)
                {
                    minTimeTick = elapsedRecords[i].timeSpan.Ticks;
                }
                if (elapsedRecords[i].timeSpan.Ticks > maxTimeTick)
                {
                    maxTimeTick = elapsedRecords[i].timeSpan.Ticks;
                }

                asFileTimeDifferenceSum += elapsedRecords[i].asFileTimeDifference;
                if (elapsedRecords[i].asFileTimeDifference < minAsFileTimeDifference)
                {
                    minAsFileTimeDifference = elapsedRecords[i].asFileTimeDifference;
                }
                if (elapsedRecords[i].asFileTimeDifference > maxAsFileTimeDifference)
                {
                    maxAsFileTimeDifference = elapsedRecords[i].asFileTimeDifference;
                }

                asPreciseFileTimeDifference += elapsedRecords[i].asPreciseFileTimeDifference;
                if (elapsedRecords[i].asPreciseFileTimeDifference < minAsPreciseFileTimeDifference)
                {
                    minAsPreciseFileTimeDifference = elapsedRecords[i].asPreciseFileTimeDifference;
                }
                if (elapsedRecords[i].asPreciseFileTimeDifference > maxAsPreciseFileTimeDifference)
                {
                    maxAsPreciseFileTimeDifference = elapsedRecords[i].asPreciseFileTimeDifference;
                }
            }

            long tickRange = maxTick - minTick;
            long timeTickRange = maxTimeTick - minTimeTick;
            UInt64 asFileTimeDifferenceRange = maxAsFileTimeDifference - minAsFileTimeDifference;
            UInt64 asPreciseFileTimeDifferenceRange = maxAsPreciseFileTimeDifference - minAsPreciseFileTimeDifference;

            double averageTick = tickSum / ReducedCycleCount;
            double averageTimeTick = timeTickSum / ReducedCycleCount;
            double averageAsFileTimeDifference = asFileTimeDifferenceSum / ReducedCycleCount;
            double averageAsPreciseFileTimeDifference = asPreciseFileTimeDifference / ReducedCycleCount;

            double tickSdmSum = 0, timeTickSdmSum = 0, asFileTimeDifferenceSdmSum = 0, asPreciseFileTimeDifferenceSdmSum = 0;
            for (int i = 0; i < ReducedCycleCount; i++)
            {
                tickSdmSum += (elapsedRecords[i].ticks - averageTick) * (Int64)(elapsedRecords[i].ticks - averageTick);
                timeTickSdmSum += (UInt64)(elapsedRecords[i].timeSpan.Ticks - averageTimeTick) * (UInt64)(elapsedRecords[i].timeSpan.Ticks - averageTimeTick);
                asFileTimeDifferenceSdmSum += (elapsedRecords[i].asFileTimeDifference - averageAsFileTimeDifference) * (elapsedRecords[i].asFileTimeDifference - averageAsFileTimeDifference);
                asPreciseFileTimeDifferenceSdmSum += (elapsedRecords[i].asPreciseFileTimeDifference - averageAsPreciseFileTimeDifference) * (elapsedRecords[i].asPreciseFileTimeDifference - averageAsPreciseFileTimeDifference);
            }

            double tickSd = Math.Sqrt(tickSdmSum / ReducedCycleCount);
            double timeTickSd = Math.Sqrt(timeTickSdmSum / ReducedCycleCount);
            double asFileTimeDifferenceSd = Math.Sqrt(asFileTimeDifferenceSdmSum / ReducedCycleCount);
            double asPreciseFileTimeDifferenceSd = Math.Sqrt(asPreciseFileTimeDifferenceSdmSum / ReducedCycleCount);

            Console.WriteLine("Stopwatch measurments");
            Console.WriteLine("  mean {0} ticks or {1} ns", averageTick, 100 * averageTick);
            Console.WriteLine("  SD {0:0.00} ticks or {1:0.00} ns", tickSd, 100 * tickSd);
            Console.WriteLine("  min {0} ticks", minTick);
            Console.WriteLine("  max {0} ticks", maxTick);
            Console.WriteLine("  range {0} ticks", tickRange);
            Console.WriteLine();
            Console.WriteLine("DateTime UtcNow measurments");
            Console.WriteLine("  mean {0} ticks", averageTimeTick);
            Console.WriteLine("  SD {0} ticks", timeTickSd);
            Console.WriteLine("  min {0} ticks", minTimeTick);
            Console.WriteLine("  max {0} ticks", maxTimeTick);
            Console.WriteLine("  range {0} ticks", timeTickRange);
            Console.WriteLine();
            Console.WriteLine("GetSystemTimeAsFileTime measurments");
            Console.WriteLine("  mean {0} ticks", averageAsFileTimeDifference);
            Console.WriteLine("  SD {0} ticks", asFileTimeDifferenceSd);
            Console.WriteLine("  min {0} ticks", minAsFileTimeDifference);
            Console.WriteLine("  max {0} ticks", maxAsFileTimeDifference);
            Console.WriteLine("  range {0} ticks", asFileTimeDifferenceRange);
            Console.WriteLine();
            Console.WriteLine("GetSystemTimePreciseAsFileTime measurments");
            Console.WriteLine("  mean {0} ticks", averageAsPreciseFileTimeDifference);
            Console.WriteLine("  SD {0} ticks", asPreciseFileTimeDifferenceSd);
            Console.WriteLine("  min {0} ticks", minAsPreciseFileTimeDifference);
            Console.WriteLine("  max {0} ticks", maxAsPreciseFileTimeDifference);
            Console.WriteLine("  range {0} ticks", asPreciseFileTimeDifferenceRange);
            Console.WriteLine();

        }

        static private UInt64 FileTimeToUInt64(FILE_TIME fileTime)
        {
            return ((UInt64)fileTime.ftTimeHigh << 32) + fileTime.ftTimeLow;
        }
    }
}
