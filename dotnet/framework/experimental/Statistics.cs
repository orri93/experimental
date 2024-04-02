using MathNet.Numerics.Distributions;
using System;
using System.Collections.Generic;

namespace experimental
{
    internal class Statistics
    {
        public static void ExperimentA()
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

        public static void ExperimentB(int seed)
        {
            const int CycleCount = 100000;

            double[] data = new double[CycleCount];

            // A random generator with a seed value
            Random random = new Random(seed);

            // A normal distribution with a mean of 193.0 and a standard deviation of 13.0 using the random generator
            Normal normal = new Normal(193.0, 13.0, random);

            // A moving statistics object with a window size of 100
            MovingStatistics movingStatistics = new MovingStatistics(100);

            double min = Double.MaxValue, max = Double.MinValue, sum = 0.0;
            for (int i = 0; i < CycleCount; i++)
            {
                double sample = normal.Sample();
                movingStatistics.AddSample(sample);
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

            double meandiff = mean - movingStatistics.Mean.Value;


            Console.WriteLine($"Count: {CycleCount}");

            Console.WriteLine($"Min: {min}");
            Console.WriteLine($"Max: {max}");
            Console.WriteLine($"Mean: {mean}");
            Console.WriteLine($"Standard deviation: {standardDeviation}");

            Console.WriteLine($"Moving statistics count: {movingStatistics.Count}");
            Console.WriteLine($"Moving statistics mean: {movingStatistics.Mean}");
            Console.WriteLine($"Moving statistics standard deviation: {movingStatistics.StandardDeviation()}");

            Console.WriteLine($"Mean difference: {meandiff}");
            Console.WriteLine();
        }
    }
}
