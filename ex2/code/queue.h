#ifndef QUEUE_H__
#define QUEUE_H__

#define QUEUE_SIZE 10

#include "fsm.h"

typedef struct {
    fsm_event_t event[QUEUE_SIZE];
    uint8_t head;
    uint8_t tail;
} queue_t;

void queue_init(queue_t *q);
uint8_t queue_is_empty(queue_t *q);
uint8_t queue_is_full(queue_t *q);
uint8_t queue_pop(queue_t *q, fsm_event_t *event);
uint8_t queue_push(queue_t *q, fsm_event_t event);

#endif /* QUEUE_H__ */
