#include "boot_metadata.h"

static bool metadata_read_copies(boot_metadata_t *copy_a,
                                 boot_metadata_t *copy_b);
static bool metadata_write_inactive_copy(const boot_metadata_t *next);
static bool metadata_marker_is_committed(uint8_t copy);
static bool metadata_body_crc_is_valid(uint8_t copy);

bool metadata_load_latest(boot_metadata_t *metadata)
{
    boot_metadata_t copy_a;
    boot_metadata_t copy_b;
    bool valid_a;
    bool valid_b;

    if ((metadata == 0) || !metadata_read_copies(&copy_a, &copy_b)) {
        return false;
    }

    valid_a = metadata_marker_is_committed(0U) &&
              metadata_body_crc_is_valid(0U);
    valid_b = metadata_marker_is_committed(1U) &&
              metadata_body_crc_is_valid(1U);
    if (!valid_a && !valid_b) {
        return false;
    }
    *metadata = (!valid_b || (valid_a && copy_a.sequence >= copy_b.sequence)) ?
                copy_a : copy_b;
    return true;
}

static bool metadata_commit_transition(boot_state_t expected,
                                       boot_state_t next_state,
                                       firmware_slot_t next_confirmed,
                                       firmware_slot_t next_pending)
{
    boot_metadata_t latest;
    boot_metadata_t next;

    if (!metadata_load_latest(&latest) || (latest.state != expected)) {
        return false;
    }

    next = latest;
    next.sequence++;
    next.state = next_state;
    next.confirmed_slot = next_confirmed;
    next.pending_slot = next_pending;
    /* Invalidate the inactive copy, write body and CRC, verify, then write its marker last. */
    return metadata_write_inactive_copy(&next);
}

bool metadata_commit_trial(firmware_slot_t installed_slot)
{
    boot_metadata_t latest;

    if (!metadata_load_latest(&latest) ||
        (latest.state != BOOT_STATE_PENDING) ||
        (latest.pending_slot != installed_slot)) {
        return false;
    }
    return metadata_commit_transition(BOOT_STATE_PENDING, BOOT_STATE_TRIAL,
                                      latest.confirmed_slot,
                                      latest.pending_slot);
}

bool metadata_commit_rollback_begin(void)
{
    boot_metadata_t latest;

    if (!metadata_load_latest(&latest)) {
        return false;
    }
    return metadata_commit_transition(BOOT_STATE_TRIAL, BOOT_STATE_ROLLBACK,
                                      latest.confirmed_slot,
                                      latest.pending_slot);
}

bool metadata_commit_rollback_complete(void)
{
    boot_metadata_t latest;

    if (!metadata_load_latest(&latest)) {
        return false;
    }
    return metadata_commit_transition(BOOT_STATE_ROLLBACK, BOOT_STATE_NONE,
                                      latest.confirmed_slot, SLOT_NONE);
}

bool metadata_confirm_application(void)
{
    boot_metadata_t latest;

    if (!metadata_load_latest(&latest) ||
        (latest.state != BOOT_STATE_TRIAL) ||
        (latest.pending_slot == SLOT_NONE)) {
        return false;
    }
    return metadata_commit_transition(BOOT_STATE_TRIAL, BOOT_STATE_NONE,
                                      latest.pending_slot, SLOT_NONE);
}
