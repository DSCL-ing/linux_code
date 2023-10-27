#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
int lockf(int fd, int cmd, off_t len);

int main()
{
    pid_t p1, p2;
    while ((p1 = fork()) == -1)(void)p1;
    if (p1 == 0)
    {
        lockf(1, 1, 0);
        for (int i = 50; i >= 0; i--)
        {
            printf("b ");
            // fflush(stdout);
        }
        printf("\n");
        lockf(1, 0, 0);
    }
    else
    {
        while ((p2 = fork()) == -1)(void)p2;
        if (p2 == 0)
        {
            lockf(1, 1, 0);
            for (int i = 50; i >= 0; i--)
            {
                printf("c ");
                // fflush(stdout);
            }
            printf("\n");
            lockf(1, 0, 0);
        }

        else
        {
            lockf(1, 1, 0);
            for (int i = 50; i >= 0; i--)
            {
                printf("a ");
                // fflush(stdout);
            }
            printf("\n");
            lockf(1, 0, 0);
        }
    }
    return 0;
}