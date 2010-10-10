#ifndef LIBUTIL_THREADS_HPP
#define LIBUTIL_THREADS_HPP

#include <pthread.h>
#include <libutil_unaryfunction.h>
#include <libutil_mutex.hpp>
#include <libutil_condition.hpp>
#include <libutil_debug.h>
#include <assert.h>
#include <vector>

namespace libutil {

void* execute( void* );

class GenericComputation
{
    public:
    virtual unsigned int size() = 0;
    virtual void eval( unsigned int ) = 0;
};

struct ThreadData
{
    ThreadData( long tid, libutil::Condition* activationCondition,
            libutil::Condition* completionCondition,
            bool* active, int* complete,
            GenericComputation* computation )
    : m_tid( tid )
    , m_activationCondition( activationCondition )
    , m_completionCondition( completionCondition )
    , m_active( active )
    , m_complete( complete )
    , m_computation( computation )
    {
        DBG1;
    }
    long                           m_tid;
    libutil::Condition*            m_activationCondition;
    libutil::Condition*            m_completionCondition;
    bool*                          m_active;
    int*                           m_complete;
    GenericComputation*            m_computation;
};

std::ostream& operator<<(std::ostream& ostr, const ThreadData& data)
{
    ostr << " tid = " << data.m_tid << " activationCond = "
         << data.m_activationCondition
         << " completionCond = " << data.m_completionCondition
         << " active = " << *(data.m_active)
         << " complete = " << *(data.m_complete)
         << " computation = " << data.m_computation;
    return ostr;
}

template <typename Arg, typename Result>
class ParallelizableComputation : public GenericComputation
{
    public:
    ParallelizableComputation(
                const UnaryFunctionConst<Arg,Result>& f,
                const std::vector<Arg>& inputs,
                std::vector<Result>& outputs )
        : m_f( f )
        , m_size( inputs.size() )
        , m_inputs( inputs )
        , m_outputs( outputs )
    {
        DBG1;
        assert( inputs.size() == outputs.size() );
        DBG1;
    }

    void eval( unsigned int i )
    {
        DBG1;
        m_outputs[i] = m_f( m_inputs[i] );
        DBG1;
    }

    unsigned int size()
    {
        DBG1;
        return m_size;
    }

    private:
    const UnaryFunctionConst<Arg,Result>&   m_f;
    unsigned int                            m_size;
    const std::vector<Arg>&                 m_inputs;
    std::vector<Result>&                    m_outputs;
};

template <typename Arg, typename Result>
class Threads
{
    public:

    Threads( ParallelizableComputation<Arg,Result>& computation )
        : m_threads()
        , m_complete(0)
        , m_completionMutex()
        , m_completionCondition( m_completionMutex )
        , m_active(false)
        , m_activationMutex()
        , m_activationCondition( m_activationMutex )
        , m_threadData()
        , m_computation( computation )
     {
        DBG1;
         m_threads.resize( m_computation.size() );
         m_threadData.resize( m_computation.size() );

         for ( size_t i = 0; i < m_computation.size(); i++ )
         {
        DBG1;
             m_threadData[i] = new ThreadData( i,
                                               &m_activationCondition,
                                               &m_completionCondition,
                                               &m_active,
                                               &m_complete,
                                               &m_computation );
        DBG1;
        DBGX2(m_threadData[i]);
        assert(
             pthread_create( 
                &m_threads[i],
                NULL,
                execute,
                m_threadData[i]) == 0 );
        DBG1;
         }
        DBG1;
     }

     ~Threads()
     {
        DBG1;
         for ( size_t i = 0; i < m_computation.size(); i++ )
         {
        DBG1;
             delete m_threadData[i];
        DBG1;
         }
        DBG1;
     }

     void activate()
     {
        DBG1;
         m_activationCondition.mutex().lock();
        DBG1;
         m_active = true;
        DBG1;
         m_activationCondition.broadcast();
        DBG1;
         m_activationCondition.mutex().unlock();
        DBG1;
     }

     void waitForCompletion()
     {
        DBG1;
         // wait for completion of all threads
         m_completionCondition.mutex().lock();
        DBG1;
         if( m_complete != m_computation.size() )
         {
        DBG1;
             m_completionCondition.wait();
        DBG1;
         }
        DBG1;
         m_completionCondition.mutex().unlock();
        DBG1;
     }

     private:

     std::vector<pthread_t>                        m_threads;
     int                                           m_complete;
     libutil::Mutex                                m_completionMutex;
     libutil::Condition                            m_completionCondition;
     bool                                          m_active;
     libutil::Mutex                                m_activationMutex;
     libutil::Condition                            m_activationCondition;
     std::vector<ThreadData*>                      m_threadData;
     ParallelizableComputation<Arg,Result>&        m_computation;
};

void* execute( void* threadData )
{
        DBG1;
    assert( threadData );
        DBG1;
    ThreadData* data = (ThreadData*)threadData;
        DBG1;
    assert( data );
        DBG1;
    libutil::Condition* cond = data->m_activationCondition;
        DBG1;
    assert( cond );
        DBG1;
    long tid = data->m_tid;
        DBGX2(*data);
        DBG1;
    cond->mutex().lock();
        DBG1;
    if ( !(*data->m_active) )
    {
        DBG1;
        // wait will automatically unlock mutex while
        // it waits
        cond->wait();
        DBG1;
    }
        DBG1;
    cond->mutex().unlock();
        DBG1;
    // at this point the threads are active
    // and should run the computations to completion
    // it is assumed the compuations do not *modify*
    // the same memory
        DBG1;
    data->m_computation->eval( tid );
        DBG1;
    cond = data->m_completionCondition;
        DBG1;
    cond->mutex().lock();
        DBG1;
    (*data->m_complete)++;
        DBG1;
    if ( *data->m_complete == data->m_computation->size() )
    {
        DBG1;
        cond->signal();
        DBG1;
    }
        DBG1;
    cond->mutex().unlock();
        DBG1;
    pthread_exit( NULL );
        DBG1;
    return NULL;
}

}

#endif
