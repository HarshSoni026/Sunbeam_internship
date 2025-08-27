#include<stdio.h>
#include<limits.h>
#include<float.h>
int main() {
    printf("size of int: %zu bytes\n", sizeof(int));
    printf("size of float: %zu bytes\n", sizeof(float));
    printf("size of double: %zu bytes\n", sizeof(double));
    printf("size of char: %zu bytes\n", sizeof(char));
    printf("size of long: %zu bytes\n", sizeof(long));
    printf("size of short: %zu bytes\n", sizeof(short));
    printf("size of long long: %zu bytes\n", sizeof(long long));

    printf("SCHAR_MAX: %d\n", SCHAR_MAX);
    printf("SCHAR_MIN: %d\n", SCHAR_MIN);
    printf("UCHAR_MAX: %d\n", UCHAR_MAX);
    printf("CHAR_BIT: %d\n", CHAR_BIT);

    printf("SHRT_MAX: %d\n", SHRT_MAX);
    printf("SHRT_MIN: %d\n", SHRT_MIN);
    printf("USHRT_MAX: %d\n", USHRT_MAX);


    printf("LONG_MAX: %ld\n", LONG_MAX);
    printf("LONG_MIN: %ld\n", LONG_MIN);
    printf("ULONG_MAX: %lu\n", ULONG_MAX);

    printf("LLONG_MAX: %lld\n", LLONG_MAX);
    printf("LLONG_MIN: %lld\n", LLONG_MIN);
    printf("ULLONG_MAX: %llu\n", ULLONG_MAX);

    printf("INT_MAX: %d\n", INT_MAX);
    printf("INT_MIN: %d\n", INT_MIN);
    printf("UINT_MAX: %u\n", UINT_MAX);

    printf("FLOAT_MAX: %f\n", FLT_MAX);
    printf("FLOAT_MIN: %f\n", FLT_MIN);
    printf("DBL_MAX: %lf\n", DBL_MAX);
    printf("DBL_MIN: %lf\n", DBL_MIN);

    return 0;
}