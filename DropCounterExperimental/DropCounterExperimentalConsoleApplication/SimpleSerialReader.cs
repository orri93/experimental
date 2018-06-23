using DropCounterExperimentalCore;
using System;
using System.IO.Ports;

namespace DropCounterExperimentalConsoleApplication
{
    public class SimpleSerialReader : ISerialReader
    {
        private const int defaultBaudRate = 115200;
        private const int maximumColumns = 40;
        
        private int dataLengthAvailable;
        private int dataRead;
        private byte[] buffer;
        private int column;

        private SerialPort serialPort;

        public SimpleSerialReader()
        {
            this.column = 0;
        }

        public void Initialize(
            string portName,
            int baudRate = defaultBaudRate,
            Parity parity = Parity.None,
            int dataBits = 8,
            StopBits stopBits = StopBits.None
            )
        {
            this.serialPort = new SerialPort(
                portName,
                baudRate,
                parity,
                dataBits,
                stopBits);
            this.serialPort.DataReceived += SerialPortDataReceived;
            this.serialPort.Open();
        }

        public void Close()
        {
            this.serialPort.Close();
        }

        private void SerialPortDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            this.dataLengthAvailable = this.serialPort.BytesToRead;
            this.buffer = new byte[this.dataLengthAvailable];
            this.dataRead = serialPort.Read(this.buffer, 0, this.dataLengthAvailable);
            for(int i = 0; i < this.dataRead; i++)
            {
                Console.Out.Write(this.buffer[i].ToString("X2"));
                this.column++;
                if(this.column > maximumColumns)
                {
                    Console.Out.WriteLine();
                    this.column = 0;
                }
            }
        }
    }
}
