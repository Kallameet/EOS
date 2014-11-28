#ifndef EPROCESSDATA_H
#define EPROCESSDATA_H

enum ProcessDataType {Speed, Temp, Voltage};

struct ProcessData
{
	ProcessDataType type;
	int value;
};

int const RPMLow = 500;
int const RPMHigh = 3000;
size_t const RPMIntervall = 1000;

int const TempLow = 20;
int const TempHigh = 125;
size_t const TempIntervall = 5000;

int const VoltageLow = -500;
int const VoltageHigh = 500;
size_t const VoltageIntervall = 500;

#endif