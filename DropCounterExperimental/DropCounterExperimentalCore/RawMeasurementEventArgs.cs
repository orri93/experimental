using System;
namespace DropCounterExperimentalCore
{
    public class RawMeasurementEventArgs : EventArgs
    {
        public RawMeasurementEventArgs(UInt16[] rawMeasurement)
        {
            this.RawMeasurement = rawMeasurement;
        }
        public UInt16[] RawMeasurement { get; private set; }
    }
}
