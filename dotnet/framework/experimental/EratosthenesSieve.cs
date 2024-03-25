using System;
using System.Collections.Generic;
namespace experimental
{
    internal class EratosthenesSieve
    {
        public static void Experiment()
        {
            List<int> primes = new List<int>();
            Sieve(1000000, primes);

            for (int i = 0; i < 100; i++)
            {
                Console.WriteLine(primes[i]);
                if (primes[i] > 100)
                {
                    break;
                }
            }
        }

        public static void Sieve(int number, List<int> primes)
        {
            bool[] sieve = new bool[number];

            for (int i = 2; i < number; i++)
            {
                if (!sieve[i])
                {
                    for (int j = i * 2; j < number; j += i)
                    {
                        sieve[j] = true;
                    }
                }
            }

            for (int i = 2; i < number; i++)
            {
                if (!sieve[i])
                {
                    primes.Add(i);
                }
            }
        }
    }
}
