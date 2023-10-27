#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int lockf(int fd, int cmd, off_t len);

int main()
{
    pid_t p1, p2;
    p1 = fork();
    if (p1 == 0)
    {
        lockf(1, 1, 0);
        for (int i = 0; i < 10; i++)
            printf("child%d\n", i);

        lockf(1, 0, 0);
    }
    else
    {
        p2 = fork();
        if (p2 == 0)
        {
            lockf(1, 1, 0);
            for (int i = 0; i < 10; i++)
                printf("son%d\n", i);

            lockf(1, 0, 0);
        }
        else
        {
            lockf(1, 1, 0);
            for (int i = 0; i < 10; i++)
                printf("daughter%d\n", i);
            lockf(1, 0, 0);
        }
    }
    return 0;
}
