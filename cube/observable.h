#pragma once

#include <vector>

#include "observer.h"

template<typename T> class Observable
{
    public:
        Observable(){}
        ~Observable(){}
        void notify(T& source, int number)
        {
            for(auto observer : observers)
            {
                observer->onUpdate(source, number);
            }
        }
        void subscribe(Observer<T>* observer) { observers.push_back(observer); }
        void unsubscribe(Observer<T>* observer)
        {
            for(auto i = observers.begin(); i != observers.end(); i++)
            {
                if(*i == observer)
                {
                    observers.erase(i);
                    break;
                }
            }
        }
    private:
        std::vector<Observer<T>*> observers;
};
