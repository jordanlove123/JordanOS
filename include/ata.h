#ifndef ATA_H
#define ATA_H
#include <stdint.h>

typedef struct {
    char model[41];
    uint8_t present;
    uint64_t total_sectors;
} ata_drive_t;

void ata_init(void);
int ata_identify(uint16_t *buf);
const ata_drive_t *ata_get_drive(void);
void ata_read(uint32_t lba, uint16_t *buf, uint8_t sectors);

#endif
