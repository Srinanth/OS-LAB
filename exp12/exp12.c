#include <stdio.h>

#define MAXP 10
#define MAXR 10

int main()
{
    int n, r;
    int alloc[MAXP][MAXR], request[MAXP][MAXR];
    int avail[MAXR];
    int work[MAXR];
    int finish[MAXP];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter number of resource types: ");
    scanf("%d", &r);

    // Allocation matrix
    printf("\nEnter Allocation Matrix:\n");
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<r;j++)
        {
            scanf("%d",&alloc[i][j]);
        }
    }

    // Request matrix
    printf("\nEnter Request Matrix:\n");
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<r;j++)
        {
            scanf("%d",&request[i][j]);
        }
    }

    // Available resources
    printf("\nEnter Available Resources:\n");
    for(int j=0;j<r;j++)
    {
        scanf("%d",&avail[j]);
        work[j] = avail[j];   // copy to work
    }

    // Initialize finish
    for(int i=0;i<n;i++)
        finish[i] = 0;

    // Deadlock detection algorithm
    int found = 1;

    while(found)
    {
        found = 0;

        for(int i=0;i<n;i++)
        {
            if(finish[i] == 0)
            {
                int canExecute = 1;

                // check request <= work
                for(int j=0;j<r;j++)
                {
                    if(request[i][j] > work[j])
                    {
                        canExecute = 0;
                        break;
                    }
                }

                if(canExecute)
                {
                    // process completes and releases resources
                    for(int j=0;j<r;j++)
                        work[j] += alloc[i][j];

                    finish[i] = 1;
                    found = 1;

                    printf("P%d completed, Available now: ", i);
                    for(int k=0;k<r;k++)
                        printf("%d ", work[k]);
                    printf("\n");
                }
            }
        }
    }

    // Check for deadlock
    int deadlock = 0;
    printf("\n");

    for(int i=0;i<n;i++)
    {
        if(finish[i] == 0)
        {
            deadlock = 1;
        }
    }

    if(deadlock)
    {
        printf("System is in DEADLOCK\nDeadlocked processes: ");
        for(int i=0;i<n;i++)
            if(finish[i] == 0)
                printf("P%d ", i);
    }
    else
    {
        printf("System is NOT in deadlock (All processes completed)");
    }

    return 0;
}