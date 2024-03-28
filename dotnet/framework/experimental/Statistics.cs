using MathNet.Numerics.Distributions;
using System;
using System.Collections.Generic;

namespace experimental
{
    internal class Statistics
    {
        public static void Experiment()
        {
            const int CycleCount = 100000;

            double[] data = new double[CycleCount];

            Normal normal = new Normal(193.0, 13.0);

            double min = Double.MaxValue, max = Double.MinValue, sum = 0.0;
            for (int i = 0; i < CycleCount; i++)
            {
                double sample = normal.Sample();
                if (sample < min)
                {
                    min = sample;
                }
                if (sample > max)
                {
                    max = sample;
                }
                data[i] = sample;
                sum += sample;
            }
            double mean = sum / CycleCount;

            double sdmSum = 0.0;
            for (int i = 0; i < CycleCount; i++)
            {
                double diff = data[i] - mean;
                sdmSum += diff * diff;
            }
            double standardDeviation = Math.Sqrt(sdmSum / CycleCount);

            Console.WriteLine($"Min: {min}");
            Console.WriteLine($"Max: {max}");
            Console.WriteLine($"Mean: {mean}");
            Console.WriteLine($"Standard deviation: {standardDeviation}");
        }
    }
}
