#include "boot_metadata.h"

static void boot_main_halt(void);
static void boot_jump_to_app(void);
static bool install_candidate(const boot_metadata_t *metadata);
static bool rollback_confirmed(const boot_metadata_t *metadata);
static uint32_t mcu_reset_cause_read(void);
static void log_reset_cause(uint32_t cause);

void boot_main_run(void)
{
    boot_metadata_t metadata;
    uint32_t reset_cause = mcu_reset_cause_read();

    log_reset_cause(reset_cause);
    if (!metadata_load_latest(&metadata)) {
        boot_main_halt();
    }

    switch (metadata.state) {
    case BOOT_STATE_PENDING:
        if (!install_candidate(&metadata)) {
            boot_main_halt();
        }
        if (!metadata_commit_trial(metadata.pending_slot)) {
            boot_main_halt();
        }
        break;

    case BOOT_STATE_TRIAL:
        if (!rollback_confirmed(&metadata)) {
            boot_main_halt();
        }
        break;

    case BOOT_STATE_ROLLBACK:
        /* A reset here restarts restoration from the confirmed external slot. */
        if (!rollback_confirmed(&metadata)) {
            boot_main_halt();
        }
        break;

    case BOOT_STATE_NONE:
    default:
        break;
    }

    boot_jump_to_app();
}

static bool install_candidate(const boot_metadata_t *metadata)
{
    boot_image_t candidate;

    return candidate_prevalidate(metadata, &candidate) &&
           installer_install_pending(metadata, &candidate);
}

static bool rollback_confirmed(const boot_metadata_t *metadata)
{
    boot_image_t confirmed;

    if (!confirmed_prevalidate(metadata, &confirmed)) {
        return false;
    }
    if (metadata->state == BOOT_STATE_TRIAL &&
        !metadata_commit_rollback_begin()) {
        return false;
    }
    if (!installer_restore_confirmed(metadata, &confirmed)) {
        return false;
    }
    return metadata_commit_rollback_complete();
}

static void boot_main_halt(void) { for (;;) { } }
static void boot_jump_to_app(void) { }
static uint32_t mcu_reset_cause_read(void) { return 0U; }
static void log_reset_cause(uint32_t cause) { (void)cause; }

/* Platform and installer functions used by this fixture. */
bool candidate_prevalidate(const boot_metadata_t *metadata, boot_image_t *candidate);
bool confirmed_prevalidate(const boot_metadata_t *metadata, boot_image_t *confirmed);
bool installer_install_pending(const boot_metadata_t *metadata, boot_image_t *candidate);
bool installer_restore_confirmed(const boot_metadata_t *metadata, boot_image_t *confirmed);
