#ifndef BUZZER_TASK_H
#define BUZZER_TASK_H

#include "common.h"

void buzzer_task_init();
void buzzer_beep(int count, int duration_ms, int gap_ms);

#endif // BUZZER_TASK_H
