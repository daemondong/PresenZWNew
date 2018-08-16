/**
 * @file
 * Handler for Command Class Multi Channel Association.
 * @copyright Copyright (c) 2001-2017, Sigma Designs Inc., All Rights Reserved
 */

#ifndef _COMMANDCLASSMULTICHANASSOCIATION_H_
#define _COMMANDCLASSMULTICHANASSOCIATION_H_

/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/

#include <ZW_typedefs.h>
#include <ZW_classcmd.h>
#include <ZW_TransportEndpoint.h>

/****************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                       */
/****************************************************************************/

/**
 * Returns the version of this CC.
 */
#define CmdClassMultiChannelAssociationVersion() MULTI_CHANNEL_ASSOCIATION_VERSION_V3

/****************************************************************************/
/*                              EXPORTED DATA                               */
/****************************************************************************/

// Nothing here.

/****************************************************************************/
/*                           EXPORTED FUNCTIONS                             */
/****************************************************************************/

/**
 * @brief Handler for the Multi Channel Association Command Class.
 * @param[in] rxOpt Receive options.
 * @param[in] pCmd Payload from the received frame.
 * @param[in] cmdLength Length of the given payload.
 * @return receive frame status.
 */
received_frame_status_t handleCommandClassMultiChannelAssociation(
    RECEIVE_OPTIONS_TYPE_EX *rxOpt,
    ZW_APPLICATION_TX_BUFFER *pCmd,
    BYTE cmdLength);

/**
 * @brief Handler for Association Set command.
 * @param[in] ep A given endpoint.
 * @param[in] pCmd A command containing the nodes to save in the association database.
 * @param[in] cmdLength Length of the command.
 */
extern void handleAssociationSet(
    BYTE ep,
    ZW_MULTI_CHANNEL_ASSOCIATION_SET_1BYTE_V2_FRAME* pCmd,
    BYTE cmdLength);

#endif /* _COMMANDCLASSMULTICHANASSOCIATION_H_ */
