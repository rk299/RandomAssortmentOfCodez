#include <pthread.h>
#include <libutil_threads.hpp>
#include <unistd.h>
#include <libutil_unaryfunction.h>
#include <libutil_debug.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

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

int main(int argc, const char** argv)
{
    Sleep s;
    assert( argc == 2 );
    unsigned int numComputations = atoi( argv[1] );
    std::vector<int> inputs;
    std::vector<int> outputs;
    outputs.resize( numComputations );
    for( size_t i = 0; i < numComputations; i++ )
    {
        inputs.push_back( i );
    }
    libutil::ParallelizableComputation<int,int> computation( s,
                                                             inputs,
                                                             outputs );
    DBG1;
    libutil::Threads<int,int> threads( computation );
    DBG1;
    threads.activate();
    DBG1;
    threads.waitForCompletion();
    DBG1;
}
