/******************************************************************************
 * Copyright (C) 2017 Broadcom.  The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 *
 * This program is the proprietary software of Broadcom and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to the terms and
 * conditions of a separate, written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 * no license (express or implied), right to use, or waiver of any kind with respect to the
 * Software, and Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 * secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 * LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 * OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 * USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 * LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 * EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 * USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 * ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 * LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 * ANY LIMITED REMEDY.
 *****************************************************************************/

/*******************************************************************************
 *
 * DESCRIPTION:
 *      Contains the definitions for UART Mailbox.
 *
*******************************************************************************/

#ifndef _PC_FAKE_MAILBOX_H
#define _PC_FAKE_MAILBOX_H

/************************* INCLUDES ****************************************************/
#include "bbSysTypes.h"
#include "bbSysFifo.h"
#include "bbHalMailbox.h"

/************************* DEFINITIONS *************************************************/

/**//**
 * \brief The maximum possible length of UART Mailbox message payload.
 */
#define PC_FAKE_MAILBOX_MAX_DATA_LENGTH    HAL_MAILBOX_TXRX_FIFO_CAPACITY // 128 bytes

/************************* TYPES *******************************************************/

/**//**
 * \brief Internal UART Mailbox states declaration.
 */
typedef enum
{
    PC_FAKE_MAILBOX_IDLE = 0x00,
    PC_FAKE_MAILBOX_COMPILE_MESSAGE,
    PC_FAKE_MAILBOX_WAIT_FOR_RX_END,
} PC_FakeMailboxState_t;

/**//**
 * \brief Type declaration of UART FIFO descriptor.
 */
typedef struct _PC_FakeMailboxDescriptor_t PC_FakeMailboxDescriptor_t;

/**//**
 * \brief Type declaration of a offline callback primitive.
 * \note Should be called when the Host system becomes not available.
 * \param[in] descr - the pointer to the UART FIFO descriptor structure.
 */
typedef void (*PC_FakeMailboxOfflineCallback_t)(PC_FakeMailboxDescriptor_t *const descr);

/**//**
 * \brief Type declaration of ready-to-send callback primitive.
 * \note Should be called when the TX FIFO is ready to send new message.
 *       It is not necessary to have TX FIFO completely free, since it is
 *       possible to write new message to the TX FIFO when the previous one is
 *       still there.
 * \param[in] descr - the pointer to the UART FIFO descriptor structure.
 */
typedef void (*PC_FakeMailboxReadyToSendCallback_t)(PC_FakeMailboxDescriptor_t *const descr);

/**//**
 * \brief Type declaration of data-received callback primitive.
 * \note Should be called from the H2Z "full" interrupt routine.
 * \param[in] descr - the pointer to the UART FIFO descriptor structure.
 */
typedef void (*PC_FakeMailboxDataReceivedCallback_t)(PC_FakeMailboxDescriptor_t *const descr);


/**//**
 * \brief Definition of the UART FIFO descriptor.
 */
typedef struct _PC_FakeMailboxDescriptor_t
{
    /* Internal state */
    PC_FakeMailboxState_t state;

    /* Internal buffer */
    SYS_FifoDescriptor_t fifo;
    uint8_t buffer[HAL_MAILBOX_TXRX_FIFO_CAPACITY];

    /* Offline callback. */
    PC_FakeMailboxOfflineCallback_t         offlineCallback;
    /* Ready-to-send callback. */
    PC_FakeMailboxReadyToSendCallback_t     rtsCallback;
    /* Data received callback. */
    PC_FakeMailboxDataReceivedCallback_t    rxCallback;
} PC_FakeMailboxDescriptor_t;

/************************* PROTOTYPES **************************************************/
/************************************************************************************//**
    \brief Initialize FIFO driver.
    \param[in] descr - UART FIFO descriptor.
****************************************************************************************/
void PC_FakeMailboxInit(PC_FakeMailboxDescriptor_t *const descr);

/************************************************************************************//**
    \brief Close given FIFO descriptor.
    \param[in] descr - UART FIFO descriptor.
****************************************************************************************/
void PC_FakeMailboxClose(PC_FakeMailboxDescriptor_t *const descr);

/************************************************************************************//**
    \brief Puts a piece of data to the TX FIFO.
    \note Generates assert if conveyed chunk of data doesn't fit to the free part
          of TX FIFO.
          Generates assert if TX FIFO is not held for transmission (unexpected call).
    \param[in] descr - UART FIFO descriptor.
    \parem[in] data - pointer to the chunk of data.
    \parem[in] dataSize - data chunk length.
****************************************************************************************/
void PC_FakeMailboxTx(PC_FakeMailboxDescriptor_t *const descr, const uint8_t *data, uint8_t dataSize);

/************************************************************************************//**
    \brief Returns a size of the remaining free place in TX FIFO.
    \param[in] descr - UART FIFO descriptor.
    \return Size of the remaining free place in TX FIFO.
****************************************************************************************/
uint8_t PC_FakeMailboxTxFifoAvailableSize(PC_FakeMailboxDescriptor_t *const descr);

/************************************************************************************//**
    \brief Finalizes the transmission. Sends the "full" interrupt to the destination
           subsystem.
    \param[in] descr - UART FIFO descriptor.
****************************************************************************************/
void PC_FakeMailboxTxEnd(PC_FakeMailboxDescriptor_t *const descr, HAL_HostId_t destSubsystem);

/************************************************************************************//**
    \brief Reads a piece of data from RX FIFO.
    \note Generates assert if RX FIFO is empty (unexpected call).
          Generates assert if size of data in RX FIFO is less than requested.
    \param[in] descr - UART FIFO descriptor.
    \parem[in, out] buffer - a pointer to the given buffer.
    \parem[in] length - number of bytes to be read.
****************************************************************************************/
void PC_FakeMailboxRx(PC_FakeMailboxDescriptor_t *const descr, uint8_t *buffer, uint8_t length);

/************************************************************************************//**
    \brief Finalizes the reception procedure. Indicates that the message has been
           successfully read by higher layer.
    \note Generates assert if message was not read completely.
    \param[in] descr - UART FIFO descriptor.
****************************************************************************************/
void PC_FakeMailboxRxEnd(PC_FakeMailboxDescriptor_t *const descr);

#endif /* _PC_FAKE_MAILBOX_H */

/* eof bbPcFakeMailbox.h */