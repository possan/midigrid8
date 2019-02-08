#ifndef _ADS1015_H_
#define _ADS1015_H_

void ads1015_init();
void ads1015_prepare(int index, int channel);
int ads1015_read(int index);

#endif
