#include "hid_service.h"
#include "config.h"
#include "hid.h"

// Hid Service UUID
const uint8_t hidServUUID[ATT_BT_UUID_SIZE] = {LO_UINT16(HID_SERVICE_UUID), HI_UINT16(HID_SERVICE_UUID)};
// Keyboard Report UUID
const uint8_t keyboardReportUUID[ATT_BT_UUID_SIZE] = {LO_UINT16(KEYBOARD_REPORT_UUID), HI_UINT16(KEYBOARD_REPORT_UUID)};
// Mouse Report UUID
const uint8_t mouseReportUUID[ATT_BT_UUID_SIZE] = {LO_UINT16(MOUSE_REPORT_UUID), HI_UINT16(MOUSE_REPORT_UUID)};

// Hid Profile Service attribute
static const gattAttrType_t hidService = {ATT_BT_UUID_SIZE, hidServUUID};

// Keyboard Report Properties
static uint8_t keyboardReportProps = GATT_PROP_WRITE;
// Keyboard Report Value
static uint8_t keyboardReport[sizeof(KeyboardReport)] = {0};
// Keyboard Report User Description
static uint8_t keyboardReportUserDesp[] = "keyboard report\0";

// Mouse Report Properties
static uint8_t mouseReportProps = GATT_PROP_WRITE;
// Mouse Report Value
static uint8_t mouseReport[sizeof(MouseReport)] = {0};
// Keyboard Report User Description
static uint8_t mouseReportUserDesp[] = "mouse report\0";

// GATT特征表
static gattAttribute_t hidAttrTbl[] = {
    // Hid Service
    {
        {ATT_BT_UUID_SIZE, primaryServiceUUID}, /* type */
        GATT_PERMIT_READ,                       /* permissions */
        0,                                      /* handle */
        (uint8_t *)&hidService                  /* pValue */
    },

    // Keyboard Report Declaration
    {
        {ATT_BT_UUID_SIZE, characterUUID},
        GATT_PERMIT_READ,
        0,
        &keyboardReportProps
    },

    // Keyboard Report Value
    {
        {ATT_BT_UUID_SIZE, keyboardReportUUID},
        GATT_PERMIT_WRITE,
        0,
        keyboardReport
    },

    // Keyboard Report User Description
    {
        {ATT_BT_UUID_SIZE, charUserDescUUID},
        GATT_PERMIT_READ,
        0,
        keyboardReportUserDesp
    },

    // Mouse Report Declaration
    {
        {ATT_BT_UUID_SIZE, characterUUID},
        GATT_PERMIT_READ,
        0,
        &mouseReportProps
    },

    // Mouse Report Value
    {
        {ATT_BT_UUID_SIZE, mouseReportUUID},
        GATT_PERMIT_WRITE,
        0,
        mouseReport
    },

    // Mouse Report User Description
    {
        {ATT_BT_UUID_SIZE, charUserDescUUID},
        GATT_PERMIT_READ,
        0,
        mouseReportUserDesp
    },
};

/**
 * @fn      hid_WriteAttrCB 
 *
 * @param   connHandle - connection request was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   pLen - length of data
 * @param   offset - offset of the first octet to be written
 * @param   method - type of write message
 *
 * @return  SUCCESS: Write was successfully.<BR>
 *          blePending: A response is pending for this client.<BR>
 *          Error, otherwise: ref ATT_ERR_CODE_DEFINES.<BR>
 */
static bStatus_t hid_WriteAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                 uint8_t *pValue, uint16_t len, uint16_t offset,
                                 uint8_t method)
{
    if (pAttr->type.len != ATT_BT_UUID_SIZE) {
        return (ATT_ERR_INVALID_HANDLE);
    }
    if (offset != 0) {
        return (ATT_ERR_ATTR_NOT_LONG);
    }

    bStatus_t status = SUCCESS;
    uint16 uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch (uuid) {
        case KEYBOARD_REPORT_UUID:
            if (len != sizeof(KeyboardReport)) {
                status = ATT_ERR_INVALID_VALUE_SIZE;
            } else {
                sendKeyboardReport((KeyboardReport *)pValue);
            }
            break;

        case MOUSE_REPORT_UUID:
            if (len != sizeof(MouseReport)) {
                status = ATT_ERR_INVALID_VALUE_SIZE;
            } else {
                sendMouseReport((MouseReport *)pValue);
            }
            break;

        default:
            status = ATT_ERR_ATTR_NOT_FOUND;
            break;
    }
    return status;
}

/*********************************************************************
 * @fn      addGattHidService
 *
 * @brief   添加GATT hid服务
 *
 * @param   
 *
 * @return  none
 */
void addGattHidService() {
    // Hid Service Callbacks
    static gattServiceCBs_t hidCBs = {
        NULL,               // Write callback function pointer
        hid_WriteAttrCB,    // Write callback function pointer
        NULL                // Authorization callback function pointer
    };
    GATTServApp_RegisterService(hidAttrTbl,
                            GATT_NUM_ATTRS(hidAttrTbl),
                            GATT_MAX_ENCRYPT_KEY_SIZE,
                            &hidCBs);
}