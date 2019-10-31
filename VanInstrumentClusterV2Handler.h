﻿// VanInstrumentClusterV2Handler.h
#pragma once

#ifndef _VanInstrumentClusterV2Handler_h
    #define _VanInstrumentClusterV2Handler_h

#include "AbstractVanMessageHandler.h"

#include "VanDataToBridgeToCan.h"
#include "VanIgnitionDataToBridgeToCan.h"
#include "DoorStatus.h"

#include "VanInstrumentClusterV2Structs.h"

class VanInstrumentClusterV2Handler : public AbstractVanMessageHandler {
    ~VanInstrumentClusterV2Handler()
    {

    }

public:
    bool ProcessMessage(
        const uint8_t identByte1,
        const uint8_t identByte2,
        const uint8_t vanMessageWithoutId[],
        const uint8_t messageLength,
        VanDataToBridgeToCan& dataToBridge,
        VanIgnitionDataToBridgeToCan& ignitionDataToBridge,
        DoorStatus& doorStatus) override
    {
        if (!(IsVanIdent(identByte1, identByte2, VAN_ID_INSTRUMENT_CLUSTER_V2) && messageLength == 14))
        {
            return false;
        }

        const VanInstrumentClusterV2Packet packet = DeSerialize<VanInstrumentClusterV2Packet>(vanMessageWithoutId);
        dataToBridge.LightStatuses.status.LowBeam = packet.data.LightsStatus.dipped_beam;
        dataToBridge.LightStatuses.status.HighBeam = packet.data.LightsStatus.high_beam;
        dataToBridge.LightStatuses.status.FrontFog = packet.data.LightsStatus.front_fog;
        dataToBridge.LightStatuses.status.RearFog = packet.data.LightsStatus.rear_fog;
        dataToBridge.LightStatuses.status.LeftIndicator = packet.data.LightsStatus.left_indicator;
        dataToBridge.LightStatuses.status.RightIndicator = packet.data.LightsStatus.right_indicator;
        dataToBridge.FuelLevel = packet.data.FuelLevel;

        ignitionDataToBridge.NightMode = dataToBridge.LightStatuses.status.LowBeam || dataToBridge.LightStatuses.status.HighBeam;

        return true;
    }
};

#endif
