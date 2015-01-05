#ifndef __GPIO_H__
#define __GPIO_H__

#define GPIO_BASE	0x40E00000

typedef volatile struct 
{
    DWORD gplr0;	// Level Reg. Bank 0
    DWORD gplr1;	// Level Reg. Bank 1
    DWORD gplr2;	// Level Reg. Bank 2
    DWORD gpdr0;	// Direction Reg. Bank 0
    DWORD gpdr1;	// Direction Reg. Bank 1
    DWORD gpdr2;	// Direction Reg. Bank 2
    DWORD gpsr0;	// Output Set Reg. Bank 0
    DWORD gpsr1;	// Output Set Reg. Bank 1
    DWORD gpsr2;	// Output Set Reg. Bank 2
    DWORD gpcr0;	// Output Clr Reg. Bank 0
    DWORD gpcr1;	// Output Clr Reg. Bank 1
    DWORD gpcr2;	// Output Clr Reg. Bank 2
    DWORD grer0;	// Ris. Edge Detect Enable Reg. Bank 0
    DWORD grer1;	// Ris. Edge Detect Enable Reg. Bank 1
    DWORD grer2;	// Ris. Edge Detect Enable Reg. Bank 2
    DWORD gfer0;	// Fal. Edge Detect Enable Reg. Bank 0
    DWORD gfer1;	// Fal. Edge Detect Enable Reg. Bank 1
    DWORD gfer2;	// Fal. Edge Detect Enable Reg. Bank 2
    DWORD gedr0;	// Edge Detect Status Reg. Bank 0
    DWORD gedr1;	// Edge Detect Status Reg. Bank 1
    DWORD gedr2;	// Edge Detect Status Reg. Bank 2
    DWORD gafr0_l;	// Alt. Function Select Reg.[  0:15 ]
    DWORD gafr0_u;	// Alt. Function Select Reg.[ 16:31 ]
    DWORD gafr1_l;	// Alt. Function Select Reg.[ 32:47 ]
    DWORD gafr1_u;	// Alt. Function Select Reg.[ 48:63 ]
    DWORD gafr2_l;	// Alt. Function Select Reg.[ 64:79 ]
    DWORD gafr2_u;	// Alt. Function Select Reg.[ 80:95 ]
    DWORD gafr3_l;	// Alt. Function Select Reg.[ 96:111]
    DWORD gafr3_u;	// Alt. Function Select Reg.[112:120]
    DWORD res1[35];
    DWORD gplr3;	// Level Detect Reg. Bank 3
    DWORD res2[2];
    DWORD gpdr3;	// Data Direction Reg. Bank 3
    DWORD res3[2];
    DWORD gpsr3;	// Pin Output Set Reg. Bank 3
    DWORD res4[2];
    DWORD gpcr3;	// Pin Output Clr Reg. Bank 3
    DWORD res5[2];
    DWORD grer3;	// Ris. Edge Detect Enable Reg. Bank 3
    DWORD res6[2];
    DWORD gfer3;	// Fal. Edge Detect Enable Reg. Bank 3
    DWORD res7[2];
    DWORD gedr3;	// Edge Detect Status Reg. Bank 3

} GPIOREG;


#endif


