#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "fsm.h"

void queue_init(queue_t *q)
{
  q->head = 0;
  q->tail = 0;

  for(uint8_t i=0; i<QUEUE_SIZE; i++)
  {
    q->event[i] = -1;
  }
}

uint8_t queue_is_empty(queue_t *q)
{
  return q->head == q->tail;
}

uint8_t queue_is_full(queue_t *q)
{
  uint8_t next;
  uint8_t state;

  // TODO: Start critical section
  next = (q->tail + 1) % QUEUE_SIZE;

  if (next == q->head)
  {
    state = 1;
  }
  else
  {
    state = 0;
  }
  // TODO: End critical section

  return state;
}

uint8_t queue_pop(queue_t *q, fsm_event_t *event)
{
  if (NULL == q)
  {
    return 0;
  }

  if (NULL == event)
  {
    return 0;
  }

  if (queue_is_empty(q))
  {
    return 0;
  }

  *event = q->event[q->head];
  q->head = (q->head + 1) % QUEUE_SIZE;

  return 1;
}

uint8_t queue_push(queue_t *q, fsm_event_t event)
{
  const uint8_t next = (q->tail + 1) % QUEUE_SIZE;

  if (NULL == q)
  {
    return 0;
  }

  if (queue_is_full(q))
  {
    return 0;
  }

  q->event[q->head] = event;
  q->tail = next;

  return 1;
}
