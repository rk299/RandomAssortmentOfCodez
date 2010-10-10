#ifndef LIBUTIL_UNARYFUNCTION_H
#define LIBUTIL_UNARYFUNCTION_H

#include <functional>

namespace libutil {

/**
 * @brief - interface of unary function that requires
 * the operator () to be defined s.t. input is const
 * @note - result is always returned by value
 */
template<typename Arg, typename Result>
class UnaryFunctionConst : public std::unary_function<Arg,Result>
{
    public:
    virtual Result operator()(const Arg&) const = 0;
};

}

#endif
