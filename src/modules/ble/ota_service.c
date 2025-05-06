#include "ota_service.h"
#include "config.h"
#include "ota.h"

// Ota Service UUID
const uint8_t otaServUUID[ATT_BT_UUID_SIZE] = {LO_UINT16(OTA_SERVICE_UUID), HI_UINT16(OTA_SERVICE_UUID)};
// Ota UUID
const uint8_t otaUUID[ATT_BT_UUID_SIZE] = {LO_UINT16(OTA_UUID), HI_UINT16(OTA_UUID)};

// Ota Profile Service attribute
static const gattAttrType_t otaService = {ATT_BT_UUID_SIZE, otaServUUID};

// Ota Properties
static uint8_t otaProps = GATT_PROP_WRITE;
// Ota User Description
static uint8_t otaUserDesp[] = "ota\0";

// GATT特征表
static gattAttribute_t otaAttrTbl[] = {
    // Hid Service
    {
        {ATT_BT_UUID_SIZE, primaryServiceUUID}, /* type */
        GATT_PERMIT_READ,                       /* permissions */
        0,                                      /* handle */
        (uint8_t *)&otaService                  /* pValue */
    },

    // Ota Declaration
    {
        {ATT_BT_UUID_SIZE, characterUUID},
        GATT_PERMIT_READ,
        0,
        &otaProps
    },

    // Ota Value
    {
        {ATT_BT_UUID_SIZE, otaUUID},
        GATT_PERMIT_WRITE,
        0,
        NULL
    },

    // Ota User Description
    {
        {ATT_BT_UUID_SIZE, charUserDescUUID},
        GATT_PERMIT_READ,
        0,
        otaUserDesp
    },
};

/**
 * @fn      ota_WriteAttrCB 
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
static bStatus_t ota_WriteAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
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
        case OTA_UUID:
            if (len != OTA_PACKET_SIZE) {
                status = ATT_ERR_INVALID_VALUE_SIZE;
            } else {
                bool success = handleOta((OtaPacket *) pValue);
                status = success? SUCCESS : ATT_ERR_INVALID_VALUE;
            }
            break;

        default:
            status = ATT_ERR_ATTR_NOT_FOUND;
            break;
    }
    return status;
}

/*********************************************************************
 * @fn      addGattOtaService
 *
 * @brief   添加GATT ota服务
 *
 * @param   
 *
 * @return  none
 */
void addGattOtaService() {
    // Hid Service Callbacks
    static gattServiceCBs_t otaCBs = {
        NULL,               // Write callback function pointer
        ota_WriteAttrCB,    // Write callback function pointer
        NULL                // Authorization callback function pointer
    };
    GATTServApp_RegisterService(otaAttrTbl,
                            GATT_NUM_ATTRS(otaAttrTbl),
                            GATT_MAX_ENCRYPT_KEY_SIZE,
                            &otaCBs);
}