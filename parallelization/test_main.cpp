#include <pthread.h>
#include <libutil_threads.hpp>
#include <unistd.h>
#include <libutil_unaryfunction.h>
#include <libutil_debug.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>

class Sleep : public libutil::UnaryFunctionConst<int,int>
{
    public:
    int operator()(const int& sleep_time) const
    {
        std::cout << "\nBegin sleep " << sleep_time << " seconds.";
        sleep(sleep_time);
        std::cout << "\nEnd sleep " << sleep_time << " seconds.";
        return sleep_time;
    }
};

class Prime : public libutil::UnaryFunctionConst<int,std::vector<int> >
{
    public:
    bool isPrime( const int num ) const
    {
        for ( int i = 2; i < sqrt(num); i++ )
        {
            if ( num % i == 0 )
            {
                return false;
            }
        }
        return true;
    }
    std::vector<int> operator()(const int& limit) const
    {
        std::vector<int> primes;
        assert( limit >= 2 );
        for ( size_t i = 2; i <= limit; i++ )
        {
            if ( isPrime( i ) )
            {
                primes.push_back( i );
            }
        }
        std::ostringstream os;
        os << "\nPrimes of " << limit
           << " are ";
        for ( size_t i = 0; i < primes.size() - 1; i++ )
        {
            os << primes[i] << ", ";
        }
        os << primes[ primes.size() - 1 ];
        std::cout << os.str() << std::endl;
        return primes;
    }
};

int main(int argc, const char** argv)
{
    Prime p;
    assert( argc == 2 );
    unsigned int numComputations = atoi( argv[1] );
    std::vector<int> inputs;
    std::vector<std::vector<int> > outputs;
    outputs.resize( numComputations );
    for( size_t i = 2; i < numComputations+2; i++ )
    {
        inputs.push_back( i );
    }
    libutil::ParallelizableComputation<int,
            std::vector<int> > computation( p,
                                            inputs,
                                            outputs );
    DBG1;
    libutil::Threads<int,std::vector<int> > threads( computation );
    DBG1;
    threads.activate();
    DBG1;
    threads.waitForCompletion();
    DBG1;
}
