#include "config.h"
#include "devinfo_service.h"

// 设备信息
// Device information service
const uint8_t devInfoServUUID[ATT_BT_UUID_SIZE] = {LO_UINT16(DEVINFO_SERV_UUID), HI_UINT16(DEVINFO_SERV_UUID)};
// Manufacturer Name String
const uint8_t devInfoMfrNameUUID[ATT_BT_UUID_SIZE] = {LO_UINT16(MANUFACTURER_NAME_UUID), HI_UINT16(MANUFACTURER_NAME_UUID)};
// Model Number String
const uint8_t devInfoModelNumberUUID[ATT_BT_UUID_SIZE] = {LO_UINT16(MODEL_NUMBER_UUID), HI_UINT16(MODEL_NUMBER_UUID)};
// Firmware Revision String
const uint8_t devInfoFirmwareRevUUID[ATT_BT_UUID_SIZE] = {LO_UINT16(FIRMWARE_REV_UUID), HI_UINT16(FIRMWARE_REV_UUID)};

// Device Information Service attribute
static const gattAttrType_t devInfoService = {ATT_BT_UUID_SIZE, devInfoServUUID};
// Manufacturer Name String characteristic
static uint8_t       devInfoMfrNameProps = GATT_PROP_READ;
static const uint8_t devInfoMfrName[] = DEV_MANUFACTURER;
// Model Number String characteristic
static uint8_t       devInfoModelNumberProps = GATT_PROP_READ;
static const uint8_t devInfoModelNumber[] = DEV_MODEL_NUM;
// Firmware Revision String characteristic
static uint8_t       devInfoFirmwareRevProps = GATT_PROP_READ;
static const uint8_t devInfoFirmwareRev[] = DEV_FIRMWARE_REV;

// GATT特征表
static gattAttribute_t devInfoAttrTbl[] = {
    // Device Information Service
    {
        {ATT_BT_UUID_SIZE, primaryServiceUUID}, /* type */
        GATT_PERMIT_READ,                       /* permissions */
        0,                                      /* handle */
        (uint8_t *)&devInfoService              /* pValue */
    },

    // Manufacturer Name String Declaration
    {
        {ATT_BT_UUID_SIZE, characterUUID},
        GATT_PERMIT_READ,
        0,
        &devInfoMfrNameProps
    },

    // Manufacturer Name Value
    {
        {ATT_BT_UUID_SIZE, devInfoMfrNameUUID},
        GATT_PERMIT_READ,
        0,
        (uint8_t *)devInfoMfrName
    },

    // Model Number String Declaration
    {
        {ATT_BT_UUID_SIZE, characterUUID},
        GATT_PERMIT_READ,
        0,
        &devInfoModelNumberProps
    },

    // Model Number Value
    {
        {ATT_BT_UUID_SIZE, devInfoModelNumberUUID},
        GATT_PERMIT_READ,
        0,
        (uint8_t *)devInfoModelNumber},

    // Firmware Revision String Declaration
    {
        {ATT_BT_UUID_SIZE, characterUUID},
        GATT_PERMIT_READ,
        0,
        &devInfoFirmwareRevProps
    },

    // Firmware Revision Value
    {
        {ATT_BT_UUID_SIZE, devInfoFirmwareRevUUID},
        GATT_PERMIT_READ,
        0,
        (uint8_t *)devInfoFirmwareRev
    },
};

/*********************************************************************
 * @fn          devInfo_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 *
 * @return      Success or Failure
 */
static bStatus_t devInfo_ReadAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                    uint8_t *pValue, uint16_t *pLen, uint16_t offset, uint16_t maxLen, uint8_t method)
{
    bStatus_t status = SUCCESS;
    uint16_t  uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);

    switch(uuid)
    {  
        case MANUFACTURER_NAME_UUID:
            // verify offset
            if(offset >= (sizeof(devInfoMfrName) - 1))
            {
                status = ATT_ERR_INVALID_OFFSET;
            }
            else
            {
                // determine read length (exclude null terminating character)
                *pLen = MIN(maxLen, ((sizeof(devInfoMfrName) - 1) - offset));

                // copy data
                tmos_memcpy(pValue, &devInfoMfrName[offset], *pLen);
            }
            break;

        case MODEL_NUMBER_UUID:
            // verify offset
            if(offset >= (sizeof(devInfoModelNumber) - 1))
            {
                status = ATT_ERR_INVALID_OFFSET;
            }
            else
            {
                // determine read length (exclude null terminating character)
                *pLen = MIN(maxLen, ((sizeof(devInfoModelNumber) - 1) - offset));

                // copy data
                tmos_memcpy(pValue, &devInfoModelNumber[offset], *pLen);
            }
            break;

        case FIRMWARE_REV_UUID:
            // verify offset
            if(offset >= (sizeof(devInfoFirmwareRev) - 1))
            {
                status = ATT_ERR_INVALID_OFFSET;
            }
            else
            {
                // determine read length (exclude null terminating character)
                *pLen = MIN(maxLen, ((sizeof(devInfoFirmwareRev) - 1) - offset));

                // copy data
                tmos_memcpy(pValue, &devInfoFirmwareRev[offset], *pLen);
            }
            break;

        default:
            *pLen = 0;
            status = ATT_ERR_ATTR_NOT_FOUND;
            break;
    }

    return (status);
}

/*********************************************************************
 * @fn      addGattDevinfoService
 *
 * @brief   添加GATT设备信息服务
 *
 * @param   
 *
 * @return  none
 */
void addGattDevinfoService() {
    // Device Info Service Callbacks
    static gattServiceCBs_t devInfoCBs = {
        devInfo_ReadAttrCB, // Read callback function pointer
        NULL,               // Write callback function pointer
        NULL                // Authorization callback function pointer
    };
    GATTServApp_RegisterService(devInfoAttrTbl,
                                GATT_NUM_ATTRS(devInfoAttrTbl),
                                GATT_MAX_ENCRYPT_KEY_SIZE,
                                &devInfoCBs);
}