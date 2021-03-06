﻿// CanRadioRemoteMessageHandler.h
#pragma once

#ifndef _CanRadioRemoteMessageHandler_h
    #define _CanRadioRemoteMessageHandler_h

#include "CanMessageHandlerBase.h"
#include "../AbstractCanMessageSender.h"
#include "../Structs/CanRadioRemoteStructs.h"

class CanRadioRemoteMessageHandler : public CanMessageHandlerBase
{
    static const uint8_t CAN_RADIO_REMOTE_MESSAGE_INTERVAL = 50;

    CanRadioRemoteButtonPacketSender* RadioRemoteSender;
    uint8_t ButtonByte;
    uint8_t ScrollByte;

    virtual void InternalProcess()
    {
        RadioRemoteSender->SendAsByte(
            ButtonByte,
            ScrollByte
        );
    }

    public:
    CanRadioRemoteMessageHandler(AbstractCanMessageSender* object) : CanMessageHandlerBase(object, CAN_RADIO_REMOTE_MESSAGE_INTERVAL)
    {
        RadioRemoteSender = new CanRadioRemoteButtonPacketSender(object);
    }

    void SetData(
        uint8_t buttonByte,
        uint8_t scrollByte
    )
    {
        ButtonByte = buttonByte;
        ScrollByte = scrollByte;

        RadioRemoteSender->SendAsByte(
            ButtonByte,
            ScrollByte);
    }
};

#endif
