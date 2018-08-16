/**
 * @file config_app.h
 * @brief Configuration file for Power Strip sample application.
 * @copyright Copyright (c) 2001-2017
 * Sigma Designs, Inc.
 * All Rights Reserved
 * @details This file contains definitions for the Z-Wave+ Framework as well for the sample app.
 *
 * NOTICE: The file name must not be changed and the two definitions APP_VERSION and APP_REVISION
 * must not be changed since they are used by the build environment.
 */
#ifndef _CONFIG_APP_H_
#define _CONFIG_APP_H_

#ifdef __C51__
#include <ZW_product_id_enum.h>
#include <commandClassManufacturerSpecific.h>
#include <agi.h>
#endif
#include <app_config_common.h>

/**
 * Defines device generic and specific types
 */
//@ [GENERIC_TYPE_ID]
#define GENERIC_TYPE  GENERIC_TYPE_SWITCH_BINARY
#define SPECIFIC_TYPE SPECIFIC_TYPE_POWER_STRIP
//@ [GENERIC_TYPE_ID]

/**
 * See ZW_basic_api.h for ApplicationNodeInformation field deviceOptionMask
 */
//@ [DEVICE_OPTIONS_MASK_ID]
#define DEVICE_OPTIONS_MASK   APPLICATION_NODEINFO_LISTENING | APPLICATION_NODEINFO_OPTIONAL_FUNCTIONALITY
//@ [DEVICE_OPTIONS_MASK_ID]

/**
 * Defines used to initialize the Z-Wave Plus Info Command Class.
 */
//@ [APP_TYPE_ID]
#define APP_ROLE_TYPE ZWAVEPLUS_INFO_REPORT_ROLE_TYPE_SLAVE_ALWAYS_ON
#define APP_NODE_TYPE ZWAVEPLUS_INFO_REPORT_NODE_TYPE_ZWAVEPLUS_NODE
#define APP_ICON_TYPE ICON_TYPE_GENERIC_POWER_STRIP
#define APP_USER_ICON_TYPE ICON_TYPE_GENERIC_POWER_STRIP
#define ENDPOINT_ICONS \
 {ICON_TYPE_GENERIC_ON_OFF_POWER_SWITCH, ICON_TYPE_GENERIC_ON_OFF_POWER_SWITCH},\
 {ICON_TYPE_GENERIC_ON_OFF_POWER_SWITCH, ICON_TYPE_GENERIC_ON_OFF_POWER_SWITCH}
// {ICON_TYPE_GENERIC_LIGHT_DIMMER_SWITCH, ICON_TYPE_GENERIC_LIGHT_DIMMER_SWITCH}
//@ [APP_TYPE_ID]


/**
 * Defines used to initialize the Manufacturer Specific Command Class.
 */
#define APP_MANUFACTURER_ID     MFG_ID_SIGMA_DESIGNS

#define APP_PRODUCT_TYPE_ID     PRODUCT_TYPE_ID_ZWAVE_PLUS
#define APP_PRODUCT_ID          PRODUCT_ID_PowerStrip

#define APP_FIRMWARE_ID         APP_PRODUCT_ID | (APP_PRODUCT_TYPE_ID << 8)

#define APP_DEVICE_ID_TYPE      DEVICE_ID_TYPE_PSEUDO_RANDOM
#define APP_DEVICE_ID_FORMAT    DEVICE_ID_FORMAT_BIN

/**
 * Defines used to initialize the Association Group Information (AGI)
 * Command Class.
 */
#define NUMBER_OF_INDIVIDUAL_ENDPOINTS    2
#define NUMBER_OF_AGGREGATED_ENDPOINTS    0
#define NUMBER_OF_ENDPOINTS         NUMBER_OF_INDIVIDUAL_ENDPOINTS + NUMBER_OF_AGGREGATED_ENDPOINTS
#define MAX_ASSOCIATION_GROUPS      5
#define MAX_ASSOCIATION_IN_GROUP    4

//@ [AGI_TABLE_ID]
#define AGITABLE_LIFELINE_GROUP \
 {COMMAND_CLASS_BASIC, BASIC_SET},\
 {COMMAND_CLASS_DEVICE_RESET_LOCALLY, DEVICE_RESET_LOCALLY_NOTIFICATION}

#define AGITABLE_LIFELINE_GROUP_EP1_2_3_4 {COMMAND_CLASS_BASIC, BASIC_SET}

#define  AGITABLE_ROOTDEVICE_GROUPS \
 AGITABLE_ENDPOINT_1_GROUPS, \
 AGITABLE_ENDPOINT_2_GROUPS, \
 AGITABLE_ENDPOINT_3_GROUPS, \
 AGITABLE_ENDPOINT_4_GROUPS

/*
 * Endpoint groups.
 */
#define  AGITABLE_ENDPOINT_1_GROUPS \
 {ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL, ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY01, {COMMAND_CLASS_BASIC, BASIC_SET},"set EP 1"}
#define  AGITABLE_ENDPOINT_2_GROUPS \
 {ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL, ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY02, {COMMAND_CLASS_BASIC, BASIC_SET},"set EP 2"}
#define  AGITABLE_ENDPOINT_3_GROUPS  \
 {ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL, ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY03, {COMMAND_CLASS_BASIC, BASIC_SET},"set EP 3"}
#define  AGITABLE_ENDPOINT_4_GROUPS  \
 {ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL, ASSOCIATION_GROUP_INFO_REPORT_PROFILE_CONTROL_KEY04, {COMMAND_CLASS_BASIC, BASIC_SET},"set EP 4"}
//@ [AGI_TABLE_ID]

/**
 * Life-line end-point naming
 */
#define EP1_LIFELINE_NAME {'E','P',' ','1',' ','s','w','i','t','c','h','\0'}
#define EP2_LIFELINE_NAME {'E','P',' ','2',' ','s','w','i','t','c','h','\0'}
#define EP3_LIFELINE_NAME {'E','P',' ','3',' ','s','w','i','t','c','h','\0'}
#define EP4_LIFELINE_NAME {'E','P',' ','4',' ','s','w','i','t','c','h','\0'}


/**
 *  Mapping root group to endpoint group:
 *  Root group -> endpoint, endpoint-group
 */
#define ASSOCIATION_ROOT_GROUP_MAPPING_CONFIG \
  {ASS_GRP_ID_2, ENDPOINT_1, ASS_GRP_ID_2}, \
  {ASS_GRP_ID_3, ENDPOINT_2, ASS_GRP_ID_2}, \
  {ASS_GRP_ID_4, ENDPOINT_3, ASS_GRP_ID_2}, \
  {ASS_GRP_ID_5, ENDPOINT_4, ASS_GRP_ID_2}


#define FIRMWARE_UPGRADABLE        0xFF  /**< 0x00 = Not upgradable, 0xFF = Upgradable*/

/**
 * Configuration for ApplicationUtilities/notification.h + .c
 */
#define MAX_NOTIFICATIONS 3

/**
 * configuration for ApplicationUtilities/multilevel_switch.h + .c
 */
#define SWITCH_MULTI_ENDPOINTS  1

/**
 * Number of Total KEYs 
 */
#define PINOUTNO   4

//@ [SECURITY_AUTHENTICATION_ID]
/*
 * This definition must be set in order for the application to handle CSA. It is used only in the
 * application.
 */
// #define APP_SUPPORTS_CLIENT_SIDE_AUTHENTICATION

/*
 * This definition tells the protocol whether the application uses CSA or not.
 * It can be set to one of the two following values:
 * - SECURITY_AUTHENTICATION_SSA
 * - SECURITY_AUTHENTICATION_CSA
 */
#define REQUESTED_SECURITY_AUTHENTICATION SECURITY_AUTHENTICATION_SSA
//@ [SECURITY_AUTHENTICATION_ID]

/**
 * Security keys
 */
//@ [REQUESTED_SECURITY_KEYS_ID]
#define REQUESTED_SECURITY_KEYS (SECURITY_KEY_S0_BIT | SECURITY_KEY_S2_UNAUTHENTICATED_BIT)
//@ [REQUESTED_SECURITY_KEYS_ID]

#endif /* _CONFIG_APP_H_ */

