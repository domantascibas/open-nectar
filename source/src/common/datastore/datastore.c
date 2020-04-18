#include "datastore.h"

datastore_t datastore = {
    // ---------------------------------------------------------------
    // Main board data
    // ---------------------------------------------------------------
    {
        "00.00.00.00",          // char ucVersion[12];  // 12B

        /* Temperature */
        {
            0,                  // uint8_t ucProcessor; // 1B
        },

        /* Voltage */
        {
            0,                  // uint16_t usVref;   // 2B
            0,                  // uint16_t usVss3V3; // 2B
            0,                  // uint16_t usVss5V;  // 2B
            0,                  // uint16_t usVss12V; // 2B
        },
    },

    // ---------------------------------------------------------------
    // Energy meter data
    // ---------------------------------------------------------------
    {
        0,                      // uint32_t grid; // 4B
        0,                      // uint32_t sun;  // 4B
    },

    // ---------------------------------------------------------------
    // Error data
    // ---------------------------------------------------------------
    {
        0,                      // uint16_t usErrors; // 2B
    },

    // ---------------------------------------------------------------
    // Device Settings data
    // ---------------------------------------------------------------
    {
        // current device mode
        // boost enabled
        // boiler power

        // max temp
        // target temp
        // night temp

        // wifi configured
        // power board calibrated
        // ref voltage
        // ref current
    },
};
