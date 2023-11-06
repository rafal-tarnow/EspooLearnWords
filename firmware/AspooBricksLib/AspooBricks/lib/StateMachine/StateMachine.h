#pragma once

#include <functional>
#include <cstdint>


class StateMachine{
    using CallbackState = std::function<void(uint8_t)>;
    using CallbackChangeState = std::function<void(uint8_t, uint8_t)>;

    public:

    void begin(uint8_t state){
        mCurrentState = state;
        if(callbackEnterState){
            callbackEnterState(mCurrentState);
        }
    }

    void switchToState(uint8_t state){
        if(callbackExitState){
            callbackExitState(mCurrentState);
        }
        if(callbackChangeState){
            callbackChangeState(mCurrentState, state);
        }
        mCurrentState = state;
        if(callbackEnterState){
            callbackEnterState(mCurrentState);
        }
    }

    uint8_t getCuttentState(){
        return mCurrentState;
    }

    template <typename T>
    void onEnterState(T *obj, void (T::*method)(uint8_t))
    {
        callbackEnterState = [obj, method](uint8_t state)
        {
            (obj->*method)(state);
        };
    }

    template <typename T>
    void onExitState(T *obj, void (T::*method)(uint8_t))
    {
        callbackExitState = [obj, method](uint8_t state)
        {
            (obj->*method)(state);
        };
    }

    template <typename T>
    void onChangeState(T *obj, void (T::*method)(uint8_t, uint8_t))
    {
        callbackChangeState = [obj, method](uint8_t fromState, uint8_t toState)
        {
            (obj->*method)(fromState, toState);
        };
    }

    private:
    CallbackState callbackEnterState;
    CallbackState callbackExitState;
    CallbackChangeState callbackChangeState;
    uint8_t mCurrentState = 0;
};