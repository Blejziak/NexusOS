#ifndef DRIVERS_PIC_H
#define DRIVERS_PIC_H

void pic_init(void);
void pic_send_eoi(uint8_t irq);

#endif
