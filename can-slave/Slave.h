#include "TestSlave.h"

void TestSlave_heartbeatError(CO_Data* d, uint8_t);

uint8_t TestSlave_canSend(Message *);

void TestSlave_initialisation(CO_Data* d);
void TestSlave_preOperational(CO_Data* d);
void TestSlave_operational(CO_Data* d);
void TestSlave_stopped(CO_Data* d);

void TestSlave_post_sync(CO_Data* d);
void TestSlave_post_TPDO(CO_Data* d);
void TestSlave_storeODSubIndex(CO_Data* d, uint16_t wIndex, uint8_t bSubindex);
void TestSlave_post_emcy(CO_Data* d, uint8_t nodeID, uint16_t errCode, uint8_t errReg);
