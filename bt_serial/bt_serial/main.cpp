#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SerialPort.h"

using std::cout;
using std::endl;

char port_name[9] = "\\\\.\\5";

bool Sync_After = false;
byte Packet_TX_Index = 0;
byte Data_Prev = 0;
byte PUD0 = 0;
byte CRD_PUD2_PCDT = 0;
byte PUD1 = 0;
byte PacketCount = 0;
byte PacketCyclicData = 0;
byte psd_idx = 0;
byte PacketStreamData[10000];

int main(void)
{
	SerialPort serialPort(port_name);

	char buffer[100];
	if (serialPort.isConnected)
		serialPort.readSerialPort(buffer, sizeof(buffer));
	for (int i = 0; i < 100; i++) {
		Parsing_LXSDFT2(buffer[i]);
	}
}

int Parsing_LXSDFT2(byte data_crnt)
{
	int retv = 0;
	if (Data_Prev == 255 && data_crnt == 254)
	{
		Sync_After = true;
		Packet_TX_Index = 0;
	}

	Data_Prev = data_crnt;

	if (Sync_After == true) {
		Packet_TX_Index++;
		if (Packet_TX_Index > 1) {
			if (Packet_TX_Index == 2)
				PUD0 = data_crnt;
			else if (Packet_TX_Index == 3)
				CRD_PUD2_PCDT = data_crnt;
			else if (Packet_TX_Index == 4)
				PacketCount = data_crnt;
			else if (Packet_TX_Index == 5)
				PUD1 = data_crnt;
			else if (Packet_TX_Index == 6)
				PacketCyclicData = data_crnt;
			else if (Packet_TX_Index > 6) {
				psd_idx = (byte)(Packet_TX_Index - 7);
				PacketStreamData[psd_idx] = data_crnt;

				if (Packet_TX_Index == 70) {
					Sync_After = false;
					retv = 1;
				}
			}
		}
	}
}