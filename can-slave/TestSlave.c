
/* File generated by gen_cfile.py. Should not be modified. */

#include "TestSlave.h"


/**************************************************************************/
/* Declaration of the mapped variables                                    */
/**************************************************************************/
uint8_t SlaveMap1 = 0x0;		/* Mapped at index 0x2000, subindex 0x00 */
uint8_t SlaveMap2 = 0x0;		/* Mapped at index 0x2001, subindex 0x00 */
uint8_t SlaveMap3 = 0x0;		/* Mapped at index 0x2002, subindex 0x00 */
uint8_t SlaveMap4 = 0x0;		/* Mapped at index 0x2003, subindex 0x00 */
uint8_t SlaveMap5 = 0x0;		/* Mapped at index 0x2004, subindex 0x00 */
uint8_t SlaveMap6 = 0x0;		/* Mapped at index 0x2005, subindex 0x00 */
uint8_t SlaveMap7 = 0x0;		/* Mapped at index 0x2006, subindex 0x00 */
uint8_t SlaveMap8 = 0x0;		/* Mapped at index 0x2007, subindex 0x00 */
uint8_t SlaveMap9 = 0x0;		/* Mapped at index 0x2008, subindex 0x00 */
uint32_t SlaveMap10 = 0x0;		/* Mapped at index 0x2009, subindex 0x00 */
uint16_t SlaveMap11 = 0x0;		/* Mapped at index 0x200A, subindex 0x00 */
INTEGER16 SlaveMap12 = 0x1512;		/* Mapped at index 0x200B, subindex 0x00 */
INTEGER16 SlaveMap13 = 0x4D2;		/* Mapped at index 0x200C, subindex 0x00 */

/**************************************************************************/
/* Declaration of the value range types                                   */
/**************************************************************************/

#define valueRange_EMC 0x9F /* Type for index 0x1003 subindex 0x00 (only set of value 0 is possible) */
uint32_t TestSlave_valueRangeTest (uint8_t typeValue, void * value)
{
  switch (typeValue) {
    case valueRange_EMC:
      if (*(uint8_t*)value != (uint8_t)0) return OD_VALUE_RANGE_EXCEEDED;
      break;
  }
  return 0;
}

uint8_t Write_Outputs_8_Bit[] =		/* Mapped at index 0x6200, subindex 0x01 - 0x01 */
  {
    0x0	/* 0 */
  };//zxb add this

/**************************************************************************/
/* The node id                                                            */
/**************************************************************************/
/* node_id default value.*/
uint8_t TestSlave_bDeviceNodeId = 0x0;
//#define NodeId  0x2

/**************************************************************************/
/* Array of message processing information */

const uint8_t TestSlave_iam_a_slave = 1;

TIMER_HANDLE TestSlave_heartBeatTimers[1];

/*
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

                               OBJECT DICTIONARY

$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/

/* index 0x1000 :   Device Type. */
                    uint32_t TestSlave_obj1000 = 0x12D;	/* 301 */
                    subindex TestSlave_Index1000[] = 
                     {
                       { RO, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1000 }
                     };

/* index 0x1001 :   Error Register. */
                    uint8_t TestSlave_obj1001 = 0x0;	/* 0 */
                    subindex TestSlave_Index1001[] = 
                     {
                       { RO, uint8, sizeof (uint8_t), (void*)&TestSlave_obj1001 }
                     };

/* index 0x1003 :   Pre-defined Error Field. */
                    uint8_t TestSlave_highestSubIndex_obj1003 = 0; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1003[] = 
                    {
                      0x0,	/* 0 */
                      0x0,	/* 0 */
                      0x0,	/* 0 */
                      0x0,	/* 0 */
                      0x0,	/* 0 */
                      0x0,	/* 0 */
                      0x0,	/* 0 */
                      0x0	/* 0 */
                    };
                    ODCallback_t TestSlave_Index1003_callbacks[] = 
                     {
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                     };
                    subindex TestSlave_Index1003[] = 
                     {
                       { RW, valueRange_EMC, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1003 },
                       { RO, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1003[0] },
                       { RO, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1003[1] },
                       { RO, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1003[2] },
                       { RO, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1003[3] },
                       { RO, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1003[4] },
                       { RO, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1003[5] },
                       { RO, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1003[6] },
                       { RO, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1003[7] }
                     };

/* index 0x1005 :   SYNC COB ID. */
                    uint32_t TestSlave_obj1005 = 0x80;	/* 128 */
                    ODCallback_t TestSlave_Index1005_callbacks[] = 
                     {
                       NULL,
                     };
                    subindex TestSlave_Index1005[] = 
                     {
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1005 }
                     };

/* index 0x1006 :   Communication / Cycle Period. */
                    uint32_t TestSlave_obj1006 = 0x0;	/* 0 */
                    ODCallback_t TestSlave_Index1006_callbacks[] = 
                     {
                       NULL,
                     };
                    subindex TestSlave_Index1006[] = 
                     {
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1006 }
                     };

/* index 0x1010 :   Store parameters. */
                    uint8_t TestSlave_highestSubIndex_obj1010 = 4; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1010_Save_All_Parameters = 0x0;	/* 0 */
                    uint32_t TestSlave_obj1010_Save_Communication_Parameters = 0x0;	/* 0 */
                    uint32_t TestSlave_obj1010_Save_Application_Parameters = 0x0;	/* 0 */
                    uint32_t TestSlave_obj1010_Save_Manufacturer_Parameters = 0x0;	/* 0 */
                    ODCallback_t TestSlave_Index1010_callbacks[] = 
                     {
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                     };
                    subindex TestSlave_Index1010[] = 
                     {
                       { RO, uint8, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1010 },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1010_Save_All_Parameters },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1010_Save_Communication_Parameters },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1010_Save_Application_Parameters },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1010_Save_Manufacturer_Parameters }
                     };

/* index 0x1011 :   Restore Default Parameters. */
                    uint8_t TestSlave_highestSubIndex_obj1011 = 4; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1011_Restore_All_Default_Parameters = 0x0;	/* 0 */
                    uint32_t TestSlave_obj1011_Restore_Communication_Default_Parameters = 0x0;	/* 0 */
                    uint32_t TestSlave_obj1011_Restore_Application_Default_Parameters = 0x0;	/* 0 */
                    uint32_t TestSlave_obj1011_Restore_Manufacturer_Default_Parameters = 0x0;	/* 0 */
                    ODCallback_t TestSlave_Index1011_callbacks[] = 
                     {
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                     };
                    subindex TestSlave_Index1011[] = 
                     {
                       { RO, uint8, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1011 },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1011_Restore_All_Default_Parameters },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1011_Restore_Communication_Default_Parameters },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1011_Restore_Application_Default_Parameters },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1011_Restore_Manufacturer_Default_Parameters }
                     };

/* index 0x1014 :   Emergency COB ID. */
                    uint32_t TestSlave_obj1014 = 0x80;	/* 128 */
                    subindex TestSlave_Index1014[] = 
                     {
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1014 }
                     };

/* index 0x1016 :   Consumer Heartbeat Time */
                    uint8_t TestSlave_highestSubIndex_obj1016 = 0;
                    uint32_t TestSlave_obj1016[]={0};

/* index 0x1017 :   Producer Heartbeat Time. */
                    uint16_t TestSlave_obj1017 = 0x0;	/* 0 */
                    ODCallback_t TestSlave_Index1017_callbacks[] = 
                     {
                       NULL,
                     };
                    subindex TestSlave_Index1017[] = 
                     {
                       { RW, uint16, sizeof (uint16_t), (void*)&TestSlave_obj1017 }
                     };

/* index 0x1018 :   Identity. */
                    uint8_t TestSlave_highestSubIndex_obj1018 = 4; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1018_Vendor_ID = 0x12345678;	/* 305419896 */
                    uint32_t TestSlave_obj1018_Product_Code = 0x90123456;	/* 2417112150 */
                    uint32_t TestSlave_obj1018_Revision_Number = 0x78901234;	/* 2022707764 */
                    uint32_t TestSlave_obj1018_Serial_Number = 0x56789012;	/* 1450741778 */
                    subindex TestSlave_Index1018[] = 
                     {
                       { RO, uint8, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1018 },
                       { RO, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1018_Vendor_ID },
                       { RO, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1018_Product_Code },
                       { RO, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1018_Revision_Number },
                       { RO, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1018_Serial_Number }
                     };

/* index 0x1200 :   Server SDO Parameter. */
                    uint8_t TestSlave_highestSubIndex_obj1200 = 2; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1200_COB_ID_Client_to_Server_Receive_SDO = 0x600 ;	/* 1536 */
                    uint32_t TestSlave_obj1200_COB_ID_Server_to_Client_Transmit_SDO = 0x580;	/* 1408 */
                    subindex TestSlave_Index1200[] = 
                     {
                       { RO, uint8, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1200 },
                       { RO, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1200_COB_ID_Client_to_Server_Receive_SDO },
                       { RO, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1200_COB_ID_Server_to_Client_Transmit_SDO }
                     };


										 
/* index 0x1400 :   Receive PDO 1 Parameter. */
                    uint8_t TestSlave_highestSubIndex_obj1400 = 5; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1400_COB_ID_used_by_PDO = 0x200;	/* 512 */
                    uint8_t TestSlave_obj1400_Transmission_Type = 0x1;	/* 1 */
                    uint16_t TestSlave_obj1400_Inhibit_Time = 0x0;	/* 0 */
                    uint8_t TestSlave_obj1400_Compatibility_Entry = 0x0;	/* 0 */
                    uint16_t TestSlave_obj1400_Event_Timer = 0x0;	/* 0 */
                    subindex TestSlave_Index1400[] = 
                     {
                       { RO, uint8, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1400 },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1400_COB_ID_used_by_PDO },
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_obj1400_Transmission_Type },
                       { RW, uint16, sizeof (uint16_t), (void*)&TestSlave_obj1400_Inhibit_Time },
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_obj1400_Compatibility_Entry },
                       { RW, uint16, sizeof (uint16_t), (void*)&TestSlave_obj1400_Event_Timer }
                     };										 

/* index 0x1600 :   Receive PDO 1 Mapping. */
                    uint8_t TestSlave_highestSubIndex_obj1600 = 1; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1600[] = 
                    {
                      0x62000008	/* 1644167432 */
                    };
                    subindex TestSlave_Index1600[] = 
                     {
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1600 },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1600[0] }
                     };
										 
										 

/* index 0x1800 :   Transmit PDO 1 Parameter. */
                    uint8_t TestSlave_highestSubIndex_obj1800 = 5; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1800_COB_ID_used_by_PDO = 0x180;	/* 384 */
                    uint8_t TestSlave_obj1800_Transmission_Type = 0x0;	/* 0 */
                    uint16_t TestSlave_obj1800_Inhibit_Time = 0x0;	/* 0 */
                    uint8_t TestSlave_obj1800_Compatibility_Entry = 0x0;	/* 0 */
                    uint16_t TestSlave_obj1800_Event_Timer = 0x0;	/* 0 */
                    ODCallback_t TestSlave_Index1800_callbacks[] = 
                     {
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                     };
                    subindex TestSlave_Index1800[] = 
                     {
                       { RO, uint8, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1800 },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1800_COB_ID_used_by_PDO },
                       { RW|TO_BE_SAVE, uint8, sizeof (uint8_t), (void*)&TestSlave_obj1800_Transmission_Type },
                       { RW|TO_BE_SAVE, uint16, sizeof (uint16_t), (void*)&TestSlave_obj1800_Inhibit_Time },
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_obj1800_Compatibility_Entry },
                       { RW, uint16, sizeof (uint16_t), (void*)&TestSlave_obj1800_Event_Timer }
                     };

/* index 0x1801 :   Transmit PDO 2 Parameter. */
                    uint8_t TestSlave_highestSubIndex_obj1801 = 5; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1801_COB_ID_used_by_PDO = 0x280;	/* 640 */
                    uint8_t TestSlave_obj1801_Transmission_Type = 0x0;	/* 0 */
                    uint16_t TestSlave_obj1801_Inhibit_Time = 0x0;	/* 0 */
                    uint8_t TestSlave_obj1801_Compatibility_Entry = 0x0;	/* 0 */
                    uint16_t TestSlave_obj1801_Event_Timer = 0x0;	/* 0 */
                    ODCallback_t TestSlave_Index1801_callbacks[] = 
                     {
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                     };
                    subindex TestSlave_Index1801[] = 
                     {
                       { RO, uint8, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1801 },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1801_COB_ID_used_by_PDO },
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_obj1801_Transmission_Type },
                       { RW, uint16, sizeof (uint16_t), (void*)&TestSlave_obj1801_Inhibit_Time },
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_obj1801_Compatibility_Entry },
                       { RW, uint16, sizeof (uint16_t), (void*)&TestSlave_obj1801_Event_Timer }
                     };

/* index 0x1802 :   Transmit PDO 3 Parameter. */
                    uint8_t TestSlave_highestSubIndex_obj1802 = 5; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1802_COB_ID_used_by_PDO = 0x380;	/* 896 */
                    uint8_t TestSlave_obj1802_Transmission_Type = 0xFF;	/* 255 */
                    uint16_t TestSlave_obj1802_Inhibit_Time = 0x1388;	/* 5000 */
                    uint8_t TestSlave_obj1802_Compatibility_Entry = 0x0;	/* 0 */
                    uint16_t TestSlave_obj1802_Event_Timer = 0xEEE;	/* 1000 */   //修改循环发送的周期
                    ODCallback_t TestSlave_Index1802_callbacks[] = 
                     {
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                     };
                    subindex TestSlave_Index1802[] = 
                     {
                       { RO, uint8, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1802 },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1802_COB_ID_used_by_PDO },
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_obj1802_Transmission_Type },
                       { RW, uint16, sizeof (uint16_t), (void*)&TestSlave_obj1802_Inhibit_Time },
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_obj1802_Compatibility_Entry },
                       { RW, uint16, sizeof (uint16_t), (void*)&TestSlave_obj1802_Event_Timer }
                     };

/* index 0x1803 :   Transmit PDO 4 Parameter. */
                    uint8_t TestSlave_highestSubIndex_obj1803 = 5; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1803_COB_ID_used_by_PDO = 0x480;	/* 1152 */
                    uint8_t TestSlave_obj1803_Transmission_Type = 0xFC;	/* 252 */
                    uint16_t TestSlave_obj1803_Inhibit_Time = 0x0;	/* 0 */
                    uint8_t TestSlave_obj1803_Compatibility_Entry = 0x0;	/* 0 */
                    uint16_t TestSlave_obj1803_Event_Timer = 0x0;	/* 0 */
                    ODCallback_t TestSlave_Index1803_callbacks[] = 
                     {
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                     };
                    subindex TestSlave_Index1803[] = 
                     {
                       { RO, uint8, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1803 },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1803_COB_ID_used_by_PDO },
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_obj1803_Transmission_Type },
                       { RW, uint16, sizeof (uint16_t), (void*)&TestSlave_obj1803_Inhibit_Time },
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_obj1803_Compatibility_Entry },
                       { RW, uint16, sizeof (uint16_t), (void*)&TestSlave_obj1803_Event_Timer }
                     };

/* index 0x1804 :   Transmit PDO 5 Parameter. */
                    uint8_t TestSlave_highestSubIndex_obj1804 = 5; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1804_COB_ID_used_by_PDO = 0x401;	/* 1025 */
                    uint8_t TestSlave_obj1804_Transmission_Type = 0xFD;	/* 253 */
                    uint16_t TestSlave_obj1804_Inhibit_Time = 0x0;	/* 0 */
                    uint8_t TestSlave_obj1804_Compatibility_Entry = 0x0;	/* 0 */
                    uint16_t TestSlave_obj1804_Event_Timer = 0x0;	/* 0 */
                    ODCallback_t TestSlave_Index1804_callbacks[] = 
                     {
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                     };
                    subindex TestSlave_Index1804[] = 
                     {
                       { RO, uint8, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1804 },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1804_COB_ID_used_by_PDO },
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_obj1804_Transmission_Type },
                       { RW, uint16, sizeof (uint16_t), (void*)&TestSlave_obj1804_Inhibit_Time },
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_obj1804_Compatibility_Entry },
                       { RW, uint16, sizeof (uint16_t), (void*)&TestSlave_obj1804_Event_Timer }
                     };

/* index 0x1A00 :   Transmit PDO 1 Mapping. */
                    uint8_t TestSlave_highestSubIndex_obj1A00 = 10; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1A00[] = 
                    {
                      0x20000001,	/* 536870913 */
                      0x20010001,	/* 536936449 */
                      0x20020001,	/* 537001985 */
                      0x20030001,	/* 537067521 */
                      0x20040001,	/* 537133057 */
                      0x20050001,	/* 537198593 */
                      0x20060001,	/* 537264129 */
                      0x20070001,	/* 537329665 */
                      0x20080008,	/* 537395208 */
                      0x20090020	/* 537460768 */
                    };
                    subindex TestSlave_Index1A00[] = 
                     {
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1A00 },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1A00[0] },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1A00[1] },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1A00[2] },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1A00[3] },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1A00[4] },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1A00[5] },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1A00[6] },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1A00[7] },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1A00[8] },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1A00[9] }
                     };

/* index 0x1A01 :   Transmit PDO 2 Mapping. */
                    uint8_t TestSlave_highestSubIndex_obj1A01 = 1; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1A01[] = 
                    {
                      0x200A0010	/* 537526288 */
                    };
                    subindex TestSlave_Index1A01[] = 
                     {
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1A01 },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1A01[0] }
                     };

/* index 0x1A02 :   Transmit PDO 3 Mapping. */
                    uint8_t TestSlave_highestSubIndex_obj1A02 = 1; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1A02[] = 
                    {
                      0x200B0010	/* 537591824 */
                    };
                    subindex TestSlave_Index1A02[] = 
                     {
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1A02 },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1A02[0] }
                     };

/* index 0x1A03 :   Transmit PDO 4 Mapping. */
                    uint8_t TestSlave_highestSubIndex_obj1A03 = 1; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1A03[] = 
                    {
                      0x200C0010	/* 537657360 */
                    };
                    subindex TestSlave_Index1A03[] = 
                     {
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1A03 },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1A03[0] }
                     };

/* index 0x1A04 :   Transmit PDO 5 Mapping. */
                    uint8_t TestSlave_highestSubIndex_obj1A04 = 1; /* number of subindex - 1*/
                    uint32_t TestSlave_obj1A04[] = 
                    {
                      0x200C0010	/* 537657360 */
                    };
                    subindex TestSlave_Index1A04[] = 
                     {
                       { RW, uint8, sizeof (uint8_t), (void*)&TestSlave_highestSubIndex_obj1A04 },
                       { RW, uint32, sizeof (uint32_t), (void*)&TestSlave_obj1A04[0] }
                     };

/* index 0x2000 :   Mapped variable SlaveMap1 */
                    subindex TestSlave_Index2000[] = 
                     {
                       { RW, boolean, sizeof (uint8_t), (void*)&SlaveMap1 }
                     };

/* index 0x2001 :   Mapped variable SlaveMap2 */
                    subindex TestSlave_Index2001[] = 
                     {
                       { RW, boolean, sizeof (uint8_t), (void*)&SlaveMap2 }
                     };

/* index 0x2002 :   Mapped variable SlaveMap3 */
                    subindex TestSlave_Index2002[] = 
                     {
                       { RW, boolean, sizeof (uint8_t), (void*)&SlaveMap3 }
                     };

/* index 0x2003 :   Mapped variable SlaveMap4 */
                    subindex TestSlave_Index2003[] = 
                     {
                       { RW, boolean, sizeof (uint8_t), (void*)&SlaveMap4 }
                     };

/* index 0x2004 :   Mapped variable SlaveMap5 */
                    subindex TestSlave_Index2004[] = 
                     {
                       { RW, boolean, sizeof (uint8_t), (void*)&SlaveMap5 }
                     };

/* index 0x2005 :   Mapped variable SlaveMap6 */
                    subindex TestSlave_Index2005[] = 
                     {
                       { RW, boolean, sizeof (uint8_t), (void*)&SlaveMap6 }
                     };

/* index 0x2006 :   Mapped variable SlaveMap7 */
                    subindex TestSlave_Index2006[] = 
                     {
                       { RW, boolean, sizeof (uint8_t), (void*)&SlaveMap7 }
                     };

/* index 0x2007 :   Mapped variable SlaveMap8 */
                    subindex TestSlave_Index2007[] = 
                     {
                       { RW, boolean, sizeof (uint8_t), (void*)&SlaveMap8 }
                     };

/* index 0x2008 :   Mapped variable SlaveMap9 */
                    subindex TestSlave_Index2008[] = 
                     {
                       { RW, uint8, sizeof (uint8_t), (void*)&SlaveMap9 }
                     };

/* index 0x2009 :   Mapped variable SlaveMap10 */
                    subindex TestSlave_Index2009[] = 
                     {
                       { RW, uint32, sizeof (uint32_t), (void*)&SlaveMap10 }
                     };

/* index 0x200A :   Mapped variable SlaveMap11 */
                    subindex TestSlave_Index200A[] = 
                     {
                       { RW, uint16, sizeof (uint16_t), (void*)&SlaveMap11 }
                     };

/* index 0x200B :   Mapped variable SlaveMap12 */
                    subindex TestSlave_Index200B[] = 
                     {
                       { RW, int16, sizeof (INTEGER16), (void*)&SlaveMap12 }
                     };

/* index 0x200C :   Mapped variable SlaveMap13 */
                    subindex TestSlave_Index200C[] = 
                     {
                       { RW, int16, sizeof (INTEGER16), (void*)&SlaveMap13 }
                     };
										 
/* index 0x6200 :   Mapped variable Write Outputs 8 Bit */
                    uint8_t ObjDict_highestSubIndex_obj6200 = 0; /* number of subindex - 1*/
                    subindex TestSlave_Index6200[] = 
                     {
                       { RW, uint8, sizeof (uint8_t), (void*)&ObjDict_highestSubIndex_obj6200 }
					   //{ RW, uint8, sizeof (uint8_t), (void*)&Write_Outputs_8_Bit[0] }
           
                     };										 										 

const indextable TestSlave_objdict[] = 
{
  { (subindex*)TestSlave_Index1000,sizeof(TestSlave_Index1000)/sizeof(TestSlave_Index1000[0]), 0x1000},
  { (subindex*)TestSlave_Index1001,sizeof(TestSlave_Index1001)/sizeof(TestSlave_Index1001[0]), 0x1001},
  { (subindex*)TestSlave_Index1003,sizeof(TestSlave_Index1003)/sizeof(TestSlave_Index1003[0]), 0x1003},
  { (subindex*)TestSlave_Index1005,sizeof(TestSlave_Index1005)/sizeof(TestSlave_Index1005[0]), 0x1005},
  { (subindex*)TestSlave_Index1006,sizeof(TestSlave_Index1006)/sizeof(TestSlave_Index1006[0]), 0x1006},
  { (subindex*)TestSlave_Index1010,sizeof(TestSlave_Index1010)/sizeof(TestSlave_Index1010[0]), 0x1010},
  { (subindex*)TestSlave_Index1011,sizeof(TestSlave_Index1011)/sizeof(TestSlave_Index1011[0]), 0x1011},
  { (subindex*)TestSlave_Index1014,sizeof(TestSlave_Index1014)/sizeof(TestSlave_Index1014[0]), 0x1014},
  { (subindex*)TestSlave_Index1017,sizeof(TestSlave_Index1017)/sizeof(TestSlave_Index1017[0]), 0x1017},
  { (subindex*)TestSlave_Index1018,sizeof(TestSlave_Index1018)/sizeof(TestSlave_Index1018[0]), 0x1018},
  { (subindex*)TestSlave_Index1200,sizeof(TestSlave_Index1200)/sizeof(TestSlave_Index1200[0]), 0x1200},
  { (subindex*)TestSlave_Index1400,sizeof(TestSlave_Index1400)/sizeof(TestSlave_Index1400[0]), 0x1400},
  { (subindex*)TestSlave_Index1600,sizeof(TestSlave_Index1600)/sizeof(TestSlave_Index1600[0]), 0x1600},
  { (subindex*)TestSlave_Index1800,sizeof(TestSlave_Index1800)/sizeof(TestSlave_Index1800[0]), 0x1800},
  { (subindex*)TestSlave_Index1801,sizeof(TestSlave_Index1801)/sizeof(TestSlave_Index1801[0]), 0x1801},
  { (subindex*)TestSlave_Index1802,sizeof(TestSlave_Index1802)/sizeof(TestSlave_Index1802[0]), 0x1802},
  { (subindex*)TestSlave_Index1803,sizeof(TestSlave_Index1803)/sizeof(TestSlave_Index1803[0]), 0x1803},
  { (subindex*)TestSlave_Index1804,sizeof(TestSlave_Index1804)/sizeof(TestSlave_Index1804[0]), 0x1804},
  { (subindex*)TestSlave_Index1A00,sizeof(TestSlave_Index1A00)/sizeof(TestSlave_Index1A00[0]), 0x1A00},
  { (subindex*)TestSlave_Index1A01,sizeof(TestSlave_Index1A01)/sizeof(TestSlave_Index1A01[0]), 0x1A01},
  { (subindex*)TestSlave_Index1A02,sizeof(TestSlave_Index1A02)/sizeof(TestSlave_Index1A02[0]), 0x1A02},
  { (subindex*)TestSlave_Index1A03,sizeof(TestSlave_Index1A03)/sizeof(TestSlave_Index1A03[0]), 0x1A03},
  { (subindex*)TestSlave_Index1A04,sizeof(TestSlave_Index1A04)/sizeof(TestSlave_Index1A04[0]), 0x1A04},
  { (subindex*)TestSlave_Index2000,sizeof(TestSlave_Index2000)/sizeof(TestSlave_Index2000[0]), 0x2000},
  { (subindex*)TestSlave_Index2001,sizeof(TestSlave_Index2001)/sizeof(TestSlave_Index2001[0]), 0x2001},
  { (subindex*)TestSlave_Index2002,sizeof(TestSlave_Index2002)/sizeof(TestSlave_Index2002[0]), 0x2002},
  { (subindex*)TestSlave_Index2003,sizeof(TestSlave_Index2003)/sizeof(TestSlave_Index2003[0]), 0x2003},
  { (subindex*)TestSlave_Index2004,sizeof(TestSlave_Index2004)/sizeof(TestSlave_Index2004[0]), 0x2004},
  { (subindex*)TestSlave_Index2005,sizeof(TestSlave_Index2005)/sizeof(TestSlave_Index2005[0]), 0x2005},
  { (subindex*)TestSlave_Index2006,sizeof(TestSlave_Index2006)/sizeof(TestSlave_Index2006[0]), 0x2006},
  { (subindex*)TestSlave_Index2007,sizeof(TestSlave_Index2007)/sizeof(TestSlave_Index2007[0]), 0x2007},
  { (subindex*)TestSlave_Index2008,sizeof(TestSlave_Index2008)/sizeof(TestSlave_Index2008[0]), 0x2008},
  { (subindex*)TestSlave_Index2009,sizeof(TestSlave_Index2009)/sizeof(TestSlave_Index2009[0]), 0x2009},
  { (subindex*)TestSlave_Index200A,sizeof(TestSlave_Index200A)/sizeof(TestSlave_Index200A[0]), 0x200A},
  { (subindex*)TestSlave_Index200B,sizeof(TestSlave_Index200B)/sizeof(TestSlave_Index200B[0]), 0x200B},
  { (subindex*)TestSlave_Index200C,sizeof(TestSlave_Index200C)/sizeof(TestSlave_Index200C[0]), 0x200C},
  { (subindex*)TestSlave_Index6200,sizeof(TestSlave_Index6200)/sizeof(TestSlave_Index6200[0]), 0x6200},//zxb add this
};

const indextable * TestSlave_scanIndexOD (uint16_t wIndex, uint32_t * errorCode, ODCallback_t **callbacks)	
{
	int i;
	*callbacks = NULL;
	switch(wIndex){
		case 0x1000: i = 0;break;
		case 0x1001: i = 1;break;
		case 0x1003: i = 2;*callbacks = TestSlave_Index1003_callbacks; break;
		case 0x1005: i = 3;*callbacks = TestSlave_Index1005_callbacks; break;
		case 0x1006: i = 4;*callbacks = TestSlave_Index1006_callbacks; break;
		case 0x1010: i = 5;*callbacks = TestSlave_Index1010_callbacks; break;
		case 0x1011: i = 6;*callbacks = TestSlave_Index1011_callbacks; break;
		case 0x1014: i = 7;break;
		case 0x1017: i = 8;*callbacks = TestSlave_Index1017_callbacks; break;
		case 0x1018: i = 9;break;
		case 0x1200: i = 10;break;
		case 0x1400: i = 11;break;//zxb add
		case 0x1600: i = 12;break;//zxb add
		case 0x1800: i = 13;*callbacks = TestSlave_Index1800_callbacks; break;
		case 0x1801: i = 14;*callbacks = TestSlave_Index1801_callbacks; break;
		case 0x1802: i = 15;*callbacks = TestSlave_Index1802_callbacks; break;
		case 0x1803: i = 16;*callbacks = TestSlave_Index1803_callbacks; break;
		case 0x1804: i = 17;*callbacks = TestSlave_Index1804_callbacks; break;
		case 0x1A00: i = 18;break;
		case 0x1A01: i = 19;break;
		case 0x1A02: i = 20;break;
		case 0x1A03: i = 21;break;
		case 0x1A04: i = 22;break;
		case 0x2000: i = 23;break;
		case 0x2001: i = 24;break;
		case 0x2002: i = 25;break;
		case 0x2003: i = 26;break;
		case 0x2004: i = 27;break;
		case 0x2005: i = 28;break;
		case 0x2006: i = 29;break;
		case 0x2007: i = 30;break;
		case 0x2008: i = 31;break;
		case 0x2009: i = 32;break;
		case 0x200A: i = 33;break;
		case 0x200B: i = 34;break;
		case 0x200C: i = 35;break;
		case 0x6200: i = 36;break;//zxb add this
		default:
			*errorCode = OD_NO_SUCH_OBJECT;
			return NULL;
	}
	*errorCode = OD_SUCCESSFUL;
	return &TestSlave_objdict[i];
}

/* 
 * To count at which received SYNC a PDO must be sent.
 * Even if no pdoTransmit are defined, at least one entry is computed
 * for compilations issues.
 */
s_PDO_status TestSlave_PDO_status[5] = {s_PDO_status_Initializer,s_PDO_status_Initializer,s_PDO_status_Initializer,s_PDO_status_Initializer,s_PDO_status_Initializer};

quick_index TestSlave_firstIndex = {
  10, /* SDO_SVR */
  0, /* SDO_CLT */
  11, /* PDO_RCV */  //zxb old 0
  12, /* PDO_RCV_MAP */  //zxb old 0
  13, /* PDO_TRS */
  18 /* PDO_TRS_MAP */
};

quick_index TestSlave_lastIndex = {
  10, /* SDO_SVR */
  0, /* SDO_CLT */
  11, /* PDO_RCV */ //zxb old 0
  12, /* PDO_RCV_MAP */  //zxb old 0
  17, /* PDO_TRS */
  22 /* PDO_TRS_MAP */
};

uint16_t TestSlave_ObjdictSize = sizeof(TestSlave_objdict)/sizeof(TestSlave_objdict[0]); 

CO_Data TestSlave_Data = CANOPEN_NODE_DATA_INITIALIZER(TestSlave);

