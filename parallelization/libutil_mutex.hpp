#ifndef LIBUTIL_MUTEX_HPP
#define LIBUTIL_MUTEX_HPP

#include <pthread.h>
#include <libutil_debug.h>

namespace libutil {

class Mutex
{
    public:

    Mutex()
        : m_mutex( PTHREAD_MUTEX_INITIALIZER )
    {
        DBG1;
        pthread_mutex_init( &m_mutex, NULL );
        DBG1;
    }

    ~Mutex()
    {
        DBG1;
        pthread_mutex_destroy( &m_mutex );
        DBG1;
    }

    bool lock()
    {
        DBG1;
        return !pthread_mutex_lock( &m_mutex );
    }

    bool trylock()
    {
        DBG1;
        return !pthread_mutex_trylock( &m_mutex );
    }

    bool unlock()
    {
        DBG1;
        return !pthread_mutex_unlock( &m_mutex );
    }

    pthread_mutex_t& rep()
    {
        DBG1;
        return m_mutex;
    }

    private:

    pthread_mutex_t         m_mutex;
};

}

#endif
