enum ProcessDataType {Speed, Temp, Voltage};

struct ProcessData
{
	ProcessDataType type;
	int value;
};