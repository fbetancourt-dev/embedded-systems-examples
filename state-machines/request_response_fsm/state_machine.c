// 💻 Simple implementation (C)

#include <stdio.h>

typedef enum
{
    STATE_IDLE,
    STATE_READY,
    STATE_WAITING_FOR_RESPONSE,
    STATE_RETRYING,
    STATE_FAULT
} State;

typedef enum
{
    EVENT_START,
    EVENT_REQUEST_SENT,
    EVENT_RESPONSE_RECEIVED,
    EVENT_TIMEOUT,
    EVENT_RETRY_SENT,
    EVENT_RETRIES_EXHAUSTED,
    EVENT_RESET
} Event;

State state = STATE_IDLE;

const char* state_to_string(State s)
{
    switch (s)
    {
    case STATE_IDLE: return "IDLE";
    case STATE_READY: return "READY";
    case STATE_WAITING_FOR_RESPONSE: return "WAITING_FOR_RESPONSE";
    case STATE_RETRYING: return "RETRYING";
    case STATE_FAULT: return "FAULT";
    default: return "UNKNOWN";
    }
}

void handle_event(Event e)
{
    switch (state)
    {
    case STATE_IDLE:
        if (e == EVENT_START)
            state = STATE_READY;
        break;

    case STATE_READY:
        if (e == EVENT_REQUEST_SENT)
            state = STATE_WAITING_FOR_RESPONSE;
        break;

    case STATE_WAITING_FOR_RESPONSE:
        if (e == EVENT_RESPONSE_RECEIVED)
            state = STATE_READY;
        else if (e == EVENT_TIMEOUT)
            state = STATE_RETRYING;
        break;

    case STATE_RETRYING:
        if (e == EVENT_RETRY_SENT)
            state = STATE_WAITING_FOR_RESPONSE;
        else if (e == EVENT_RETRIES_EXHAUSTED)
            state = STATE_FAULT;
        break;

    case STATE_FAULT:
        if (e == EVENT_RESET)
            state = STATE_IDLE;
        break;
    }
}

void send_event(Event e, const char* event_name)
{
    printf("Event: %s\n", event_name);
    handle_event(e);
    printf("State -> %s\n\n", state_to_string(state));
}

int main(void)
{
    printf("Initial State -> %s\n\n", state_to_string(state));

    send_event(EVENT_START, "START");
    send_event(EVENT_REQUEST_SENT, "REQUEST_SENT");

    // Simulate timeout + retry flow
    send_event(EVENT_TIMEOUT, "TIMEOUT");
    send_event(EVENT_RETRY_SENT, "RETRY_SENT");

    // Simulate successful response
    send_event(EVENT_RESPONSE_RECEIVED, "RESPONSE_RECEIVED");

    // Simulate failure case
    send_event(EVENT_REQUEST_SENT, "REQUEST_SENT");
    send_event(EVENT_TIMEOUT, "TIMEOUT");
    send_event(EVENT_RETRIES_EXHAUSTED, "RETRIES_EXHAUSTED");

    // Recovery
    send_event(EVENT_RESET, "RESET");

    return 0;
}
