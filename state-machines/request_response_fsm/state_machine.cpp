// 💻 Same idea in C++

#include <iostream>
#include <string_view>

class StateMachine {
public:
    enum class State {
        Idle,
        Ready,
        WaitingForResponse,
        Retrying,
        Fault
    };

    enum class Event {
        Start,
        RequestSent,
        ResponseReceived,
        Timeout,
        RetrySent,
        RetriesExhausted,
        Reset
    };

    void handleEvent(Event e) {
        switch (state_) {
            case State::Idle:
                if (e == Event::Start)
                    transitionTo(State::Ready);
                break;

            case State::Ready:
                if (e == Event::RequestSent)
                    transitionTo(State::WaitingForResponse);
                break;

            case State::WaitingForResponse:
                if (e == Event::ResponseReceived)
                    transitionTo(State::Ready);
                else if (e == Event::Timeout)
                    transitionTo(State::Retrying);
                break;

            case State::Retrying:
                if (e == Event::RetrySent)
                    transitionTo(State::WaitingForResponse);
                else if (e == Event::RetriesExhausted)
                    transitionTo(State::Fault);
                break;

            case State::Fault:
                if (e == Event::Reset)
                    transitionTo(State::Idle);
                break;
        }
    }

    State currentState() const {
        return state_;
    }

    static std::string_view toString(State s) {
        switch (s) {
            case State::Idle: return "Idle";
            case State::Ready: return "Ready";
            case State::WaitingForResponse: return "WaitingForResponse";
            case State::Retrying: return "Retrying";
            case State::Fault: return "Fault";
        }
        return "Unknown";
    }

private:
    State state_ = State::Idle;

    void transitionTo(State next) {
        if (state_ == next) return;

        std::cout << "State: " << toString(state_)
                  << " -> " << toString(next) << '\n';

        state_ = next;
    }
};

void sendEvent(StateMachine& sm, StateMachine::Event e, const char* name) {
    std::cout << "\nEvent: " << name << '\n';
    sm.handleEvent(e);
    std::cout << "Current State: "
              << StateMachine::toString(sm.currentState()) << '\n';
}

int main() {
    StateMachine sm;

    std::cout << "Initial State: "
              << StateMachine::toString(sm.currentState()) << "\n";

    sendEvent(sm, StateMachine::Event::Start, "Start");
    sendEvent(sm, StateMachine::Event::RequestSent, "RequestSent");

    // Simulate timeout + retry
    sendEvent(sm, StateMachine::Event::Timeout, "Timeout");
    sendEvent(sm, StateMachine::Event::RetrySent, "RetrySent");

    // Simulate success
    sendEvent(sm, StateMachine::Event::ResponseReceived, "ResponseReceived");

    // Simulate failure
    sendEvent(sm, StateMachine::Event::RequestSent, "RequestSent");
    sendEvent(sm, StateMachine::Event::Timeout, "Timeout");
    sendEvent(sm, StateMachine::Event::RetriesExhausted, "RetriesExhausted");

    // Recovery
    sendEvent(sm, StateMachine::Event::Reset, "Reset");

    return 0;
}
