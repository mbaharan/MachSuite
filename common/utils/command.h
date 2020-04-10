#ifndef __COMMAND_HEADER__
#define __COMMAND_HEADER__

#define PRINT_READ_TIME(compute_Total_avg, delay) \
    printf("-> Number of CPU cycles halted for bulk read %llu \t~\t %f(uS).\n", compute_Total_avg, delay);

#define PRINT_KERNEL_TIME(compute_Total_avg, delay) \
    printf("-> Number of CPU cycles halted for kernel read %llu \t~\t %f(uS).\n", compute_Total_avg, delay);

#define PRINT_WRITE_TIME(compute_Total_avg, delay) \
    printf("-> Number of CPU cycles halted for bulk write %llu \t~\t %f(uS).\n", compute_Total_avg, delay);


typedef enum
{
    __READ__ = 0,
    __COMPUTE__,
    __WRITE__,
} command_t;

#endif