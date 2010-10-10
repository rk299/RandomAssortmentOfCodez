#ifndef LIBUTIL_DEBUG_H
#define LIBUTIL_DEBUG_H
#include <sstream>
#include <iostream>

#ifdef DEBUGGING_ON
#define NAME(X) #X
#define DEBUG_PRINT1(X) __FILE__ << " in " << __func__ << ":" << __LINE__ << NAME(X) << " = " << X
#define DEBUG_PRINT2(X) __FILE__ << ":" << __LINE__ << NAME(X) << " = " << X
#define DBG1 std::cout << __FILE__ << " in " << __func__ << ":" << __LINE__ << std::endl
#define DBG2 std::cout << __FILE__ << ":" << __LINE__ << std::endl
#define DBGX1(X) std::cout << DEBUG_PRINT1(X) << std::endl
#define DBGX2(X) std::cout << DEBUG_PRINT2(X) << std::endl
#else
#define DBG1
#define DBG2
#define DBGX1(X)
#define DBGX2(X)
#endif
#endif
