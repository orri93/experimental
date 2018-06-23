using DropCounterExperimentalCore;
using System;
using System.IO.Ports;

namespace DropCounterExperimentalConsoleApplication
{
    class Program
    {
        static void Main(string[] args)
        {
            System.Console.WriteLine("Select a serial port:");
            String[] portNames = SerialPort.GetPortNames();
            for (int i = 1; i <= portNames.Length; i++)
            {
                String portName = portNames[i - 1];
                Console.Out.WriteLine(i.ToString() + ": " + portName);
            }
            String selectedPortText = System.Console.ReadLine();
            int selectedPortIndex = Int32.Parse(selectedPortText);
            String selectedPort = portNames[selectedPortIndex - 1];
            TestSimpleSerialReader(selectedPort);
            System.Console.WriteLine("Press any key to stop");
            System.Console.ReadKey();
        }

        static ISerialReader TestSimpleSerialReader(string serialPort)
        {
            SimpleSerialReader simpleSerialReader = new SimpleSerialReader();
            simpleSerialReader.Initialize(serialPort);
            return simpleSerialReader;
        }

        static ISerialReader TestRawMeasurementSerialReader(string serialPort)
        {
            RawMeasurementSerialReader serialReader = new RawMeasurementSerialReader();
            serialReader.RawMeasurementRecieved += SerialReaderRawMeasurementRecieved;
            serialReader.Initialize(serialPort);
            return serialReader;
        }

        static void SerialReaderRawMeasurementRecieved(object sender, RawMeasurementEventArgs e)
        {
            for (int i = 0; i < e.RawMeasurement.Length; i++)
            {
                System.Console.WriteLine(e.RawMeasurement[i]);
            }
        }
    }
}
