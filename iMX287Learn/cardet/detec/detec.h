#ifndef _DETEC_H_
#define _DETEC_H_

#define CARDET_DEV_PATH     "/dev/dri/card"

void fns_detec_init(void);
void fns_detec_load(void);
void fns_detec_loop(void);
void app_detec_loop(void);

#endif

