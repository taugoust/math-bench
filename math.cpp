#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <chrono>
#include <list>
#include <string>

#define ITERATIONS 100000000
//#define ITERATIONS 1000000

class Timer
{
public:
    Timer() : start_(clock_t::now()) {}

    ~Timer()
    {
        double r = std::chrono::duration_cast<millisecond_t>(clock_t::now() - start_).count();
        printf("%.7f\n", ITERATIONS / r);
    }

private:
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1>>;
    using millisecond_t = std::chrono::duration<double, std::ratio<1, 1000>>;

    std::chrono::time_point<clock_t> start_;
};

typedef double (*tfn)(void);

class op_test;
static std::list<op_test *> tests;

class op_test
{
public:
    op_test(const std::string &name, tfn fn) : fn_(fn)
    {
        tests.push_back(this);
    }

    void run()
    {
        fn_();
    }

private:
    tfn fn_;
};

#define randf() (double)rand()
// ((double)(unsigned int)rand()) / ((double)(unsigned int)(RAND_MAX))


double *array1;
double *array2;

#define DEF_OP_TEST(name, expr)                          \
    __attribute__((noinline)) double test_op_fn_##name() \
    {                                                    \
        double total = 0.0;                              \
        srand(42);                                       \
	for (int i = 0; i < ITERATIONS; i++)             \
        {                                                \
            array1[i] = randf();                         \
            array2[i] = randf();                         \
        }                                                \
        Timer t;                                         \
        for (int i = 0; i < ITERATIONS; i++)             \
        {                                                \
            double r1 = array1[i];                       \
            double r2 = array2[i];                       \
            double v = expr;                             \
            total += v;                                  \
        }                                                \
        printf(#name ",");                               \
        return total;                                    \
    }                                                    \
    op_test test_op_##name(#name, test_op_fn_##name);

#pragma GCC diagnostic ignored "-Wunused-variable"
// DEF_OP_TEST(baseline, 1.0)
// DEF_OP_TEST(plus, r1 + r2)
// DEF_OP_TEST(minus, r1 - r2)
// DEF_OP_TEST(mult, r1 *r2)
// DEF_OP_TEST(div, r1 / r2)
DEF_OP_TEST(sqrt, sqrt(r1))
DEF_OP_TEST(sin, sin(r1))
DEF_OP_TEST(cos, cos(r1))
DEF_OP_TEST(tan, tan(r1))
DEF_OP_TEST(asin, atan(r1))
DEF_OP_TEST(acos, atan(r1))
DEF_OP_TEST(atan, atan(r1))
DEF_OP_TEST(exp, exp(r1))
DEF_OP_TEST(log, log(r1))
#pragma GCC diagnostic pop

int main(int argc, char **argv)
{
    array1 = new double[ITERATIONS];
    array2 = new double[ITERATIONS];
    for (op_test *t : tests)
    {
        t->run();
    }
}
