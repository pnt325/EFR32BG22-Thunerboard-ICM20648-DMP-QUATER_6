/*
* ________________________________________________________________________________________________________
* Copyright � 2014-2015 InvenSense Inc. Portions Copyright � 2014-2015 Movea. All rights reserved.
* This software, related documentation and any modifications thereto (collectively �Software�) is subject
* to InvenSense and its licensors' intellectual property rights under U.S. and international copyright and
* other intellectual property rights laws.
* InvenSense and its licensors retain all intellectual property and proprietary rights in and to the Software
* and any use, reproduction, disclosure or distribution of the Software without an express license
* agreement from InvenSense is strictly prohibited.
* ________________________________________________________________________________________________________
*/

#ifndef INV_ICM20648_EMS_SELF_TEST_H__
#define INV_ICM20648_EMS_SELF_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "EmbUtils/InvExport.h"

#define INV_ICM20648_GYR_SELF_TEST_OK  (0x01 << 0)
#define INV_ICM20648_ACC_SELF_TEST_OK  (0x01 << 1)
#define INV_ICM20648_MAG_SELF_TEST_OK  (0x01 << 2)
#define INV_ICM20648_SELF_TEST_OK      ( INV_ICM20648_GYR_SELF_TEST_OK | \
                                               INV_ICM20648_ACC_SELF_TEST_OK | \
                                               INV_ICM20648_MAG_SELF_TEST_OK )

#define INV_ICM20648_SELF_TEST_AG_OK   ( INV_ICM20648_GYR_SELF_TEST_OK | \
												INV_ICM20648_ACC_SELF_TEST_OK )

/* forward declaration */
struct inv_icm20648;
/**
*  @brief      Perform hardware self-test for Accel, Gyro and Compass.
*  @param[in]  None
*  @return     COMPASS_SUCESS<<2 | ACCEL_SUCCESS<<1 | GYRO_SUCCESS so 7 if all devices pass the self-test.
*/
int INV_EXPORT inv_icm20648_run_selftest(struct inv_icm20648 * s, int gyro_bias_st[], int accel_bias_st[]);

#ifdef __cplusplus
}
#endif

#endif // INV_v_EMS_SELF_TEST_H__
