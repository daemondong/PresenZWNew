/**
 * @file
 * Defines variables whose values must be kept during powerdown.
 * @copyright Copyright (c) 2001-2017, Sigma Designs Inc., All Rights Reserved
 */

#ifdef __C51__
#pragma userclass (xdata = NON_ZERO_VARS_APP)
#endif
/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/

#include <ZW_typedefs.h>

/**
 * This variable contains the Supervision session ID which must me kept during powerdown.
 */
XBYTE supervision_session_id;

/**
 * This variable is used by the Battery Monitor.
 */
XBYTE lowBattReportAcked;

/**
 * This variable is used by the Battery Monitor.
 */
XBYTE st_battery;
