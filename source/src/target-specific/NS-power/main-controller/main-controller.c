#include "main-controller.h"
#include "data.h"

static powerStateDataType_t current_state;
// static powerStateDataType_t next_state;
// static powerStateDataType_t last_state;

void MainController_init(void) {
    current_state = IDLE_STATE;
}

powerStateDataType_t MainController_run(void) {
    return current_state;
}

powerStateDataType_t MainController_current_state_get(void) {
    return current_state;
}
