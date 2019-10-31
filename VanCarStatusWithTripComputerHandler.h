﻿// VanCarStatusWithTripComputerHandler.h
#pragma once

#ifndef _VanCarStatusWithTripComputerHandler_h
    #define _VanCarStatusWithTripComputerHandler_h

#include "AbstractVanMessageHandler.h"

#include "VanDataToBridgeToCan.h"
#include "VanIgnitionDataToBridgeToCan.h"
#include "DoorStatus.h"

#include "CanDisplayPopupHandler.h"
#include "CanTripInfoHandler.h"
#include "VanCarStatusWithTripComputerStructs.h"

class VanCarStatusWithTripComputerHandler : public AbstractVanMessageHandler {
    CanDisplayPopupHandler* canPopupHandler;
    CanTripInfoHandler* canTripInfoHandler;

    ~VanCarStatusWithTripComputerHandler()
    {

    }

public:
    VanCarStatusWithTripComputerHandler(CanDisplayPopupHandler* _canPopupHandler, CanTripInfoHandler* _canTripInfoHandler)
    {
        canPopupHandler = _canPopupHandler;
        canTripInfoHandler = _canTripInfoHandler;
    }

    bool ProcessMessage(
        const uint8_t identByte1,
        const uint8_t identByte2,
        const uint8_t vanMessageWithoutId[],
        const uint8_t messageLength,
        VanDataToBridgeToCan& dataToBridge,
        VanIgnitionDataToBridgeToCan& ignitionDataToBridge,
        DoorStatus& doorStatus) override
    {
        if (!IsVanIdent(identByte1, identByte2, VAN_ID_CARSTATUS))
        {
            return false;
        }

        const VanCarStatusWithTripComputerPacket packet = DeSerialize<VanCarStatusWithTripComputerPacket>(vanMessageWithoutId);

        dataToBridge.Trip1Consumption = SwapHiByteAndLoByte(packet.data.Trip1FuelConsumption.data);
        dataToBridge.Trip1Distance = SwapHiByteAndLoByte(packet.data.Trip1Distance.data);
        dataToBridge.Trip1Speed = packet.data.Trip1Speed;

        dataToBridge.Trip2Consumption = SwapHiByteAndLoByte(packet.data.Trip2FuelConsumption.data);
        dataToBridge.Trip2Distance = SwapHiByteAndLoByte(packet.data.Trip2Distance.data);
        dataToBridge.Trip2Speed = packet.data.Trip2Speed;

        dataToBridge.FuelConsumption = SwapHiByteAndLoByte(packet.data.FuelConsumption.data);
        dataToBridge.FuelLeftToPump = SwapHiByteAndLoByte(packet.data.FuelLeftToPumpInKm.data);

        //canTripInfoHandler->SetTripData(
        //    dataToBridge.Rpm,
        //    dataToBridge.Speed,
        //    dataToBridge.Trip1Distance,
        //    dataToBridge.Trip1Speed,
        //    dataToBridge.Trip1Consumption,
        //    dataToBridge.Trip2Distance,
        //    dataToBridge.Trip2Speed,
        //    dataToBridge.Trip2Consumption,
        //    dataToBridge.FuelConsumption,
        //    //dataToBridge.FuelLeftToPump
        //    dataToBridge.FuelLevel
        //);

        if (packet.data.Field10.TripButton)
        {
            canTripInfoHandler->TripButtonPress();
        }

        doorStatus.status.FrontLeft = packet.data.Doors.FrontLeft;
        doorStatus.status.FrontRight = packet.data.Doors.FrontRight;
        doorStatus.status.RearLeft = packet.data.Doors.RearLeft;
        doorStatus.status.RearRight = packet.data.Doors.RearRight;
        doorStatus.status.BootLid = packet.data.Doors.BootLid;

        if (doorStatus.asByte != 0)
        {
            CanDisplayPopupItem item;
            item.DisplayTimeInMilliSeconds = CAN_POPUP_DOOR_MESSAGE_TIME;
            item.Category = CAN_POPUP_MSG_SHOW_CATEGORY1;
            item.MessageType = CAN_POPUP_MSG_DOORS_BOOT_BONNET_REAR_SCREEN_AND_FUEL_TANK_OPEN;
            item.DoorStatus1 = doorStatus.asByte;
            item.DoorStatus2 = 0;
            item.KmToDisplay = 0;
            item.IsInited = false;
            item.Counter = 0;
            item.Visible = false;
            item.SetVisibleOnDisplayTime = 0;
            item.VANByte = 0x02;
            canPopupHandler->QueueNewMessage(item);
        }

        return true;
    }
};

#endif
