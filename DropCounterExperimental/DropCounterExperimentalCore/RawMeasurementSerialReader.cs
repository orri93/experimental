using System;
using System.IO.Ports;

namespace DropCounterExperimentalCore
{
    public class RawMeasurementSerialReader : ISerialReader
    {
        private const int defaultBaudRate = 115200;
        private const int syncPatternLength = 2;
        private static readonly byte[] syncPattern =
            new byte[syncPatternLength] { 0xd5, 0xea };

        private static object threadLock = new object();
        private SerialPort serialPort;
        private bool synced;
        private bool firstSync;
        private int dataLengthAvailable;
        private int dataRead;
        private byte[] buffer;
        private int[] rawMeasurement;
        private Nullable<byte> lastLow;

        public event EventHandler<RawMeasurementEventArgs> RawMeasurementRecieved;

        public RawMeasurementSerialReader()
        {
            this.synced = false;
            this.firstSync = false;
            this.lastLow = null;
        }

        public void Initialize(
            string portName,
            int baudRate = defaultBaudRate,
            Parity parity = Parity.None,
            int dataBits = 8,
            StopBits stopBits = StopBits.One
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

            UInt16[] rawMeasurement = null;

            if (this.dataRead > 0)
            {
                if (this.synced)
                {
                    rawMeasurement = ProcessSyncedData(buffer, 0, this.dataRead);
                }
                else
                {
                    if (this.dataRead != 1)
                    {
                        if (this.lastLow.HasValue && this.lastLow.Value == this.buffer[0])
                        {
                            if (this.buffer[0] == syncPattern[1])
                            {
                                this.synced = true;
                            }
                        }
                    }
                    else if (this.buffer[0] == syncPattern[0])
                    {
                        this.lastLow = this.buffer[0];
                    }
                    for (int i = 0; i < this.dataRead; i++)
                    {
                        if (this.buffer[i] == 0xd5)
                        {
                            this.firstSync = true;
                        }
                    }
                }
            }

            EventHandler<RawMeasurementEventArgs> local = this.RawMeasurementRecieved;
            if (rawMeasurement != null && local != null)
            {
                local.Invoke(this, new RawMeasurementEventArgs(rawMeasurement));
            }
        }

        private UInt16[] ProcessSyncedData(byte[] buffer, int index, int length)
        {
            if (length - index == 0)
            {
                return new UInt16[0];
            }
            else if (length - index == 1)
            {
                this.lastLow = buffer[index];
                return new UInt16[0];
            }
            int rawMeasurementIndex = 0;
            int processLenght;
            UInt16[] rawMeasurement;
            if ((length - index) % 2 == 0)
            {
                // Will fit evenly into result
                processLenght = length;
                rawMeasurement = new UInt16[(length - index) / 2];
            }
            else
            {
                processLenght = length - 1;
                rawMeasurement = new UInt16[(length - index - 1) / 2];
                this.lastLow = buffer[length - 1];
            }
            for (int i = index; i < processLenght; i += 2)
            {
                byte low = buffer[i];
                byte high = buffer[i + 1];
                int tempValueHolder = high << 8 | low;
                UInt16 value = (UInt16)tempValueHolder;
                rawMeasurement[rawMeasurementIndex++] = value;
            }
            return rawMeasurement;
        }
    }
}
