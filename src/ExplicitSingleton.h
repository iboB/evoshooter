//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//
// ExplicitSingleton is a class whose method instance doesn't create the object
// instead you need to explicitly create it with createInstance and destroy it 
// with destroyInstance
// usage:
// class SomeClass : public ExplicitSingleton<SomeClass>
// {
//      DECLARE_EXPLICIT_SINGLETON(SomeClass);
// public:
//      ...other stuff...
#pragma once

template <typename T>
class ExplicitSingleton
{
public:
    static void createInstance()
    {
        assert(!m_instance);
        m_instance = new T();
    }

    static T& instance()
    {
        assert(m_instance);
        return *m_instance;
    }

    static void destroyInstance()
    {
        delete m_instance;
        m_instance = nullptr;
    }

private:
    static T* m_instance;
};

template <typename T>
T* ExplicitSingleton<T>::m_instance = nullptr;

#define DECLARE_EXPLICIT_SINGLETON(type) \
    private: \
        type(); \
        ~type(); \
        friend class ::ExplicitSingleton<type>
