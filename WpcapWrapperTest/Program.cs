using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace WpcapWrapperTest
{
    class Program
    {
        static int Main(string[] args)
        {
            string targetDeviceName = null;
            string targetDeviceDesc = null;
            // parse command line arguments
            for(int i = 0; i < args.Length; i++)
            {
                // --name
                if (args[i].Equals("--name", StringComparison.CurrentCultureIgnoreCase)) {
                    if (i + 1 <= args.Length)
                    {
                        targetDeviceName = args[i + 1].Replace("\\\\", "\\");
                        i++;
                    }
                }
                // --description
                if (args[i].Equals("--description", StringComparison.CurrentCultureIgnoreCase)) {
                    if (i + 1 <= args.Length)
                    {
                        targetDeviceDesc = args[i + 1];
                        i++;
                    }
                }
            }

            if (targetDeviceName == null && targetDeviceDesc == null)
            {
                // TODO: print usage
                Console.WriteLine("command line arguments not present\n");
                return -1;
            }

            // get instance of pcap wrapper
            PcapWrapper pcapWrapper = new PcapWrapper();

            // list devices
            List<PcapDeviceInfo> devices = new List<PcapDeviceInfo>();
            Int32 result = pcapWrapper.PcapFindAllDevs(devices);

            // match device to name and/or description to command line argument
            PcapDeviceInfo targetDevice = null;
            foreach(PcapDeviceInfo device in devices)
            {
                if (targetDeviceName != null)
                {
                    if (device.name.Contains(targetDeviceName))
                    {
                        targetDevice = device;
                        break;
                    }
                }
                if (targetDeviceDesc != null)
                {
                    if (device.description.Contains(targetDeviceDesc))
                    {
                        targetDevice = device;
                        break;
                    }
                }
            }

            if (targetDevice == null)
            {
                Console.WriteLine("target device not found\n");
                return -1;
            }

            // open pcap device for specified device
            IntPtr pcapHandle = pcapWrapper.PcapCreate(targetDevice.name);
            if (pcapHandle == null)
            {
                Console.WriteLine("failed to get pcap handle\n");
                return -1;
            }

            // capture packets in a loop
            PcapPktHdr pktHdr = new PcapPktHdr();
            Byte[] pktData = new Byte[2000];
            result = pcapWrapper.PcapNextEx(pcapHandle, pktHdr, pktData);




            if (pcapHandle != null)
            {
                pcapWrapper.PcapClose(pcapHandle);
            }
            
            return 0;
        }
    }
}
