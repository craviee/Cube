#pragma once

template<typename T> class Observer
{
    public:
        Observer(){}
        ~Observer(){}
        virtual void onUpdate(T& source, int number) = 0;
};
