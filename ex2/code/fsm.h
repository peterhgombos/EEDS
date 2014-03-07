#ifndef FSM_H__
#define FSM_H__

#define ST_INIT     1
#define ST_PAUSED   2
#define ST_PLAYING  3
#define ST_ANY      49

#define EV_NEXT     50
#define EV_PREV     51
#define EV_START    53
#define EV_FINISH   54
#define EV_ANY      99

#define FSM_EVENT_QUEUE_LEN 10

/**
* Struct that describes a transition in the FSM.
*/
typedef struct
{
  uint8_t st;
  uint8_t ev;
  uint8_t (*fn)(void);
} fsm_transition_t;

/**
* Variables of this type are posted to the queue. The FSM retrieves
* events from the queue and reacts to them.
*/
typedef uint8_t fsm_event_t;

/**
* Initialize FSM
*/
void fsm_init (void);

/**
* This will check the event queue for pending events, and react to them
* if there are any.
*/
void fsm_update (void);

/**
* Write an event to the event queue.
*/
uint8_t fsm_event_put (fsm_event_t event);

/**
* Get FSM state
*/
uint8_t fsm_get_state (void);

#endif /* FSM_H__ */
