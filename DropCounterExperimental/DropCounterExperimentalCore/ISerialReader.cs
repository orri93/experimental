using System;
using System.IO.Ports;

namespace DropCounterExperimentalCore
{
    public interface ISerialReader
    {
        void Initialize(
            string portName,
            int baudRate,
            Parity parity,
            int dataBits,
            StopBits stopBits
            );
        void Close();
    }
}
