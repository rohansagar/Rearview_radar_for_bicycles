#define ONE_US  (uint32_t)(SysCtlClockGet() / (1000*1000)) // One microsecond
#define ONE_MS  (uint32_t)(SysCtlClockGet() / 1000) // One millisecond
#define ONE_SEC (uint32_t)(SysCtlClockGet()) // One second

#define ONE_MS_PIOSC    16000
#define ONE_SEC_PIOSC   16000000
