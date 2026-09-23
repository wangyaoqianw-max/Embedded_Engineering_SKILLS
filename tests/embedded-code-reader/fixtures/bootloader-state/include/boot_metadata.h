#ifndef BOOT_METADATA_H
#define BOOT_METADATA_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    BOOT_STATE_NONE = 0,
    BOOT_STATE_PENDING,
    BOOT_STATE_TRIAL,
    BOOT_STATE_ROLLBACK
} boot_state_t;

typedef enum {
    SLOT_NONE = 0,
    SLOT_A,
    SLOT_B
} firmware_slot_t;

typedef struct {
    uint32_t sequence;
    boot_state_t state;
    firmware_slot_t confirmed_slot;
    firmware_slot_t pending_slot;
} boot_metadata_t;

typedef struct {
    firmware_slot_t source_slot;
    uint32_t image_size;
    uint32_t payload_crc;
} boot_image_t;

bool metadata_load_latest(boot_metadata_t *metadata);
bool metadata_commit_trial(firmware_slot_t installed_slot);
bool metadata_commit_rollback_begin(void);
bool metadata_commit_rollback_complete(void);
bool metadata_confirm_application(void);

#endif
