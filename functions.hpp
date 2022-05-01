#define FUNCTIONS_HPP
const double inf = 1e10;
const double zero = 1e-10;
#define sigma(x) (x<zero||x>-zero?inf:0)
#define u(x,x0) (x<x0?0:1)

