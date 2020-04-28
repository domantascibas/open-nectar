#include "mbed.h"
#include "boiler_temperature.h"
#include "ds18b20.h"
#include "u1wire.h"
#include "u1wire_enumerate.h"

u1wire_enumerate_t info;
u1wire_obj_t *device;
ds18b20_temp_t temp;

void boilerTempFunc(void) {
    
}

// printf("[1wire] enum\r\n");

// // temperatureSensor_init();

// // if(u1wire_init(device)) {
// //     u1wire_debug(device);
// // } else {
// //     printf("[1w] err\r\n");
// // }
// for (device = u1wire_enumerate(&info); device; device = u1wire_enumerate_next(&info)) {
//     u1wire_debug(device);
//     ds18b20_temp_conversion_start(device);
//     while (!ds18b20_temp_ready_p(device))
//         continue;
//     if (ds18b20_temp_read(device, &temp)) {
//         printf("[ds18b20] temp: %.2f\r\n", (float)(temp)/256);
//     }
// }

// printf("[1wire] enum end\r\n");
