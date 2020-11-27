#include "TestMaster.h"

void TestMaster_heartbeatError(CO_Data* d, uint8_t);

uint8_t TestMaster_canSend(Message *);

void TestMaster_initialisation(CO_Data* d);
void TestMaster_preOperational(CO_Data* d);
void TestMaster_operational(CO_Data* d);
void TestMaster_stopped(CO_Data* d);

void TestMaster_post_sync(CO_Data* d);
void TestMaster_post_TPDO(CO_Data* d);
void TestMaster_post_emcy(CO_Data* d, uint8_t nodeID, uint16_t errCode, uint8_t errReg);
void TestMaster_post_SlaveBootup(CO_Data* d, uint8_t nodeid);
