using System;
using System.Collections.Generic;

namespace experimental
{
    internal class MovingStatistics
    {
        private Queue<double> samples;

        public int WindowSize { get; private set; }
        public int Count { get { return samples.Count; } }
        public bool IsFull { get { return samples.Count >= WindowSize; } }
        public double Accumulator { get; private set; }
        public double? Mean { get; private set; }

        public MovingStatistics(int windowSize)
        {
            if (windowSize <= 0)
            {
                throw new ArgumentOutOfRangeException("windowSize", "Window size must be greater than zero.");
            }

            WindowSize = windowSize;
            Accumulator = 0.0;
            samples = new Queue<double>(windowSize);
        }

        public void AddSample(double sample)
        {
            Accumulator += sample;
            samples.Enqueue(sample);

            if (samples.Count > WindowSize)
            {
                Accumulator -= samples.Dequeue();
            }

            Mean = Accumulator / samples.Count;
        }

        public double StandardDeviation()
        {
            if (Mean.HasValue)
            {
                double sdmSum = 0.0;
                foreach (double sample in samples)
                {
                    double diff = sample - Mean.Value;
                    sdmSum += diff * diff;
                }

                return Math.Sqrt(sdmSum / (samples.Count - 1));

            }
            else
            {
                return Double.NaN; 
            }
        }

        public void Clear()
        {
            Accumulator = 0.0;
            samples.Clear();
            Mean = null;
        }
    }
}
