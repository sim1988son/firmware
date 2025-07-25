#include "configuration.h"
#if defined(ARCH_ESP32) && !MESHTASTIC_EXCLUDE_PAXCOUNTER
#include "Default.h"
#include "MeshService.h"
#include "PaxcounterModule.h"
#include "graphics/ScreenFonts.h"
#include "graphics/SharedUIDisplay.h"
#include "graphics/images.h"
#include <assert.h>

PaxcounterModule *paxcounterModule;

/**
 * Callback function for libpax.
 * We only clear our sent flag here, since this function is called from another thread, so we
 * cannot send to the mesh directly.
 */
void PaxcounterModule::handlePaxCounterReportRequest()
{
    // The libpax library already updated our data structure, just before invoking this callback.
    LOG_INFO("PaxcounterModule: libpax reported new data: wifi=%d; ble=%d; uptime=%lu",
             paxcounterModule->count_from_libpax.wifi_count, paxcounterModule->count_from_libpax.ble_count, millis() / 1000);
    paxcounterModule->reportedDataSent = false;
    paxcounterModule->setIntervalFromNow(0);
}

PaxcounterModule::PaxcounterModule()
    : concurrency::OSThread("Paxcounter"),
      ProtobufModule("paxcounter", meshtastic_PortNum_PAXCOUNTER_APP, &meshtastic_Paxcount_msg)
{
}

/**
 * Send the Pax information to the mesh if we got new data from libpax.
 * This is called periodically from our runOnce() method and will actually send the data to the mesh
 * if libpax updated it since the last transmission through the callback.
 * @param dest - destination node (usually NODENUM_BROADCAST)
 * @return false if sending is unnecessary, true if information was sent
 */
bool PaxcounterModule::sendInfo(NodeNum dest)
{
    if (paxcounterModule->reportedDataSent)
        return false;

    LOG_INFO("PaxcounterModule: send pax info wifi=%d; ble=%d; uptime=%lu", count_from_libpax.wifi_count,
             count_from_libpax.ble_count, millis() / 1000);

    meshtastic_Paxcount pl = meshtastic_Paxcount_init_default;
    pl.wifi = count_from_libpax.wifi_count;
    pl.ble = count_from_libpax.ble_count;
    pl.uptime = millis() / 1000;

    meshtastic_MeshPacket *p = allocDataProtobuf(pl);
    p->to = dest;
    p->decoded.want_response = false;
    p->priority = meshtastic_MeshPacket_Priority_BACKGROUND;

    service->sendToMesh(p, RX_SRC_LOCAL, true);

    paxcounterModule->reportedDataSent = true;

    return true;
}

bool PaxcounterModule::handleReceivedProtobuf(const meshtastic_MeshPacket &mp, meshtastic_Paxcount *p)
{
    return false; // Let others look at this message also if they want. We don't do anything with received packets.
}

meshtastic_MeshPacket *PaxcounterModule::allocReply()
{
    meshtastic_Paxcount pl = meshtastic_Paxcount_init_default;
    pl.wifi = count_from_libpax.wifi_count;
    pl.ble = count_from_libpax.ble_count;
    pl.uptime = millis() / 1000;
    return allocDataProtobuf(pl);
}

int32_t PaxcounterModule::runOnce()
{
    if (isActive()) {
        if (firstTime) {
            firstTime = false;
            LOG_DEBUG("Paxcounter starting up with interval of %d seconds",
                      Default::getConfiguredOrDefault(moduleConfig.paxcounter.paxcounter_update_interval,
                                                      default_telemetry_broadcast_interval_secs));
            struct libpax_config_t configuration;
            libpax_default_config(&configuration);

            configuration.blecounter = 1;
            configuration.blescantime = 0; // infinite
            configuration.wificounter = 1;
            configuration.wifi_channel_map = WIFI_CHANNEL_ALL;
            configuration.wifi_channel_switch_interval = 50;
            configuration.wifi_rssi_threshold = Default::getConfiguredOrDefault(moduleConfig.paxcounter.wifi_threshold, -80);
            configuration.ble_rssi_threshold = Default::getConfiguredOrDefault(moduleConfig.paxcounter.ble_threshold, -80);
            libpax_update_config(&configuration);

            // internal processing initialization
            libpax_counter_init(handlePaxCounterReportRequest, &count_from_libpax,
                                Default::getConfiguredOrDefault(moduleConfig.paxcounter.paxcounter_update_interval,
                                                                default_telemetry_broadcast_interval_secs),
                                0);
            libpax_counter_start();
        } else {
            sendInfo(NODENUM_BROADCAST);
        }
        return Default::getConfiguredOrDefaultMsScaled(moduleConfig.paxcounter.paxcounter_update_interval,
                                                       default_telemetry_broadcast_interval_secs, numOnlineNodes);
    } else {
        return disable();
    }
}

#if HAS_SCREEN

#include "graphics/ScreenFonts.h"
#include "graphics/SharedUIDisplay.h"

void PaxcounterModule::drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
    display->clear();
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(FONT_SMALL);
    int line = 1;

    // === Set Title
    const char *titleStr = "Pax";

    // === Header ===
    graphics::drawCommonHeader(display, x, y, titleStr);

    char buffer[50];
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(FONT_SMALL);

    libpax_counter_count(&count_from_libpax);

    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->setFont(FONT_SMALL);
    display->drawStringf(display->getWidth() / 2 + x, graphics::getTextPositions(display)[line++], buffer,
                         "WiFi: %d\nBLE: %d\nUptime: %ds", count_from_libpax.wifi_count, count_from_libpax.ble_count,
                         millis() / 1000);
}
#endif // HAS_SCREEN

#endif