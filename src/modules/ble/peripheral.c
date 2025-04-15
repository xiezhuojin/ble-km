#include "HAL.h"
#include "peripheral.h"

// Task ID for internal task/event processing
static uint8_t Peripheral_TaskID = INVALID_TASK_ID;

__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];

/*********************************************************************
 * @fn      peripheralStateNotificationCB
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void peripheralStateNotificationCB(gapRole_States_t newState, gapRoleEvent_t *pEvent)
{
    switch(newState)
    {
        case GAPROLE_WAITING:
            // 如果是连接断开事件
            if(pEvent->gap.opcode == GAP_LINK_TERMINATED_EVENT)
            {
                // 重新启动广播
                uint8_t advertising_enable = TRUE;
                GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &advertising_enable);
            }
            break;

        default:
            break;
    }
}

/*********************************************************************
 * @fn      Peripheral_ProcessEvent
 *
 * @brief   Peripheral Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id - The TMOS assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
uint16_t Peripheral_ProcessEvent(uint8_t task_id, uint16_t events)
{
    if(events & SBP_START_DEVICE_EVT)
    {
        // Start the Device
        // GAP Role Callbacks
        static gapRolesCBs_t Peripheral_PeripheralCBs = {
            peripheralStateNotificationCB, // Profile State Change Callbacks
            NULL, // When a valid RSSI is read from controller (not used by application)
            NULL
        };
        // GAP Bond Manager Callbacks
        static gapBondCBs_t Peripheral_BondMgrCBs = {
            NULL, // Passcode callback (not used by application)
            NULL  // Pairing / Bonding state Callback (not used by application)
        };
        GAPRole_PeripheralStartDevice(Peripheral_TaskID, &Peripheral_BondMgrCBs, &Peripheral_PeripheralCBs);
        return (events ^ SBP_START_DEVICE_EVT);
    }

    // Discard unknown events
    return 0;
}

/*********************************************************************
 * @fn      initBlePeripheral
 *
 * @brief   初始化蓝牙外设
 *
 * @param   
 *
 * @return  none
 */
void initBlePeripheral() {
    CH57X_BLEInit();
    HAL_Init();
    GAPRole_PeripheralInit();

    // Setup the GAP Peripheral Role Profile
    // GAP - SCAN RSP data (max size = 31 bytes)
    static uint8_t scanRspData[] = {
        // complete name
        PERIPHERIAL_MAGIC_SIZE, // length of this data
        GAP_ADTYPE_LOCAL_NAME_COMPLETE,
        PERIPHERIAL_MAGIC
    };
    GAPRole_SetParameter(GAPROLE_SCAN_RSP_DATA, sizeof(scanRspData), scanRspData);

    // Setup the GAP Bond Manager
    uint32_t passkey = 0; // passkey "000000"
    uint8_t  pairMode = GAPBOND_PAIRING_MODE_WAIT_FOR_REQ;
    uint8_t  mitm = TRUE;
    uint8_t  bonding = TRUE;
    uint8_t  ioCap = GAPBOND_IO_CAP_DISPLAY_ONLY;
    GAPBondMgr_SetParameter(GAPBOND_PERI_DEFAULT_PASSCODE, sizeof(uint32_t), &passkey);
    GAPBondMgr_SetParameter(GAPBOND_PERI_PAIRING_MODE, sizeof(uint8_t), &pairMode);
    GAPBondMgr_SetParameter(GAPBOND_PERI_MITM_PROTECTION, sizeof(uint8_t), &mitm);
    GAPBondMgr_SetParameter(GAPBOND_PERI_IO_CAPABILITIES, sizeof(uint8_t), &ioCap);
    GAPBondMgr_SetParameter(GAPBOND_PERI_BONDING_ENABLED, sizeof(uint8_t), &bonding);

    // Initialize GATT attributes
    GGS_AddService(GATT_ALL_SERVICES);           // GAP
    GATTServApp_AddService(GATT_ALL_SERVICES);   // GATT attributes

    // Set the GAP Characteristics
    GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, DEVICE_NAME);
}

/*********************************************************************
 * @fn      startBlePeripheral
 *
 * @brief   开始化蓝牙外设
 *
 * @param   
 *
 * @return  none
 */
void startBlePeripheral() {
    // Setup a delayed profile startup
    Peripheral_TaskID = TMOS_ProcessEventRegister(Peripheral_ProcessEvent);
    tmos_set_event(Peripheral_TaskID, SBP_START_DEVICE_EVT);
}