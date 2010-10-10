#ifndef LIBUTIL_CONDITION_HPP
#define LIBUTIL_CONDITION_HPP

#include <pthread.h>
#include <libutil_mutex.hpp>
#include <libutil_debug.h>

namespace libutil {

class Condition
{
    public:
    Condition( libutil::Mutex& condMutex )
        : m_conditionMutex( condMutex )
        , m_conditionVariable( PTHREAD_COND_INITIALIZER )
    {
        DBG1;
        pthread_cond_init( &m_conditionVariable, NULL );
        DBG1;
    }
    
    ~Condition()
    {
        DBG1;
        pthread_cond_destroy( &m_conditionVariable );
        DBG1;
        m_conditionMutex.unlock();
        DBG1;
    }

    void wait()
    {
        DBG1;
        pthread_cond_wait( &m_conditionVariable,
                           &m_conditionMutex.rep() );
        DBG1;
    }

    void signal()
    {
        DBG1;
        pthread_cond_signal( &m_conditionVariable );
        DBG1;
    }

    void broadcast()
    {
        DBG1;
        pthread_cond_broadcast( &m_conditionVariable );
        DBG1;
    }

    libutil::Mutex& mutex()
    {
        DBG1;
        return m_conditionMutex;
    }

    private:
    libutil::Mutex&         m_conditionMutex;
    pthread_cond_t          m_conditionVariable;
};

}

#endif
