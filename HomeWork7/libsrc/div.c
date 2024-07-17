#include "libcalc.h"
float divL(int a, int b) {
    float res = (float)a / (float)b;
    printf("%d / %d = %.3f\n", a, b, res);
    return res;
}
