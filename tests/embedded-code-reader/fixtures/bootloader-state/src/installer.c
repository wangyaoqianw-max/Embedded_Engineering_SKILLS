#include "boot_metadata.h"

bool candidate_prevalidate(const boot_metadata_t *metadata,
                           boot_image_t *candidate)
{
    if ((metadata == 0) || (candidate == 0) ||
        (metadata->state != BOOT_STATE_PENDING) ||
        (metadata->pending_slot == SLOT_NONE) ||
        (metadata->pending_slot == metadata->confirmed_slot)) {
        return false;
    }
    candidate->source_slot = metadata->pending_slot;
    return external_header_crc_ok(candidate) &&
           external_payload_crc_ok(candidate) &&
           image_vector_is_valid(candidate);
}

bool confirmed_prevalidate(const boot_metadata_t *metadata,
                           boot_image_t *confirmed)
{
    if ((metadata == 0) || (confirmed == 0) ||
        ((metadata->state != BOOT_STATE_TRIAL) &&
         (metadata->state != BOOT_STATE_ROLLBACK)) ||
        (metadata->confirmed_slot == SLOT_NONE) ||
        (metadata->confirmed_slot == metadata->pending_slot)) {
        return false;
    }
    confirmed->source_slot = metadata->confirmed_slot;
    return external_header_crc_ok(confirmed) &&
           external_payload_crc_ok(confirmed) &&
           image_vector_is_valid(confirmed);
}

bool installer_install_pending(const boot_metadata_t *metadata,
                               boot_image_t *candidate)
{
    if (!candidate_prevalidate(metadata, candidate)) {
        return false;
    }
    if (!internal_app_erase()) {
        return false;
    }
    return copy_payload_and_check_readback(candidate) &&
           internal_app_crc_ok(candidate) &&
           internal_app_vector_is_valid();
}

bool installer_restore_confirmed(const boot_metadata_t *metadata,
                                 boot_image_t *confirmed)
{
    if (!confirmed_prevalidate(metadata, confirmed)) {
        return false;
    }
    if (!internal_app_erase()) {
        return false;
    }
    return copy_payload_and_check_readback(confirmed) &&
           internal_app_crc_ok(confirmed) &&
           internal_app_vector_is_valid();
}

/* Platform and image helpers are declared here to keep the fixture self-contained. */
bool external_header_crc_ok(const boot_image_t *image);
bool external_payload_crc_ok(const boot_image_t *image);
bool image_vector_is_valid(const boot_image_t *image);
bool internal_app_erase(void);
bool copy_payload_and_check_readback(const boot_image_t *image);
bool internal_app_crc_ok(const boot_image_t *image);
bool internal_app_vector_is_valid(void);
