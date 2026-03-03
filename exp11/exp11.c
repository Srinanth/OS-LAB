#include <stdio.h>
#include <stdbool.h>

int main() {
    int n, m;

    printf("Enter number of threads (n): ");
    scanf("%d", &n);

    printf("Enter number of resource types (m): ");
    scanf("%d", &m);

    int Available[m];
    int Max[n][m];
    int Allocation[n][m];
    int Need[n][m];

    bool Finish[n];
    int SafeSequence[n];
    int Work[m];

    /* Input Available vector */
    printf("\nEnter Available vector:\n");
    for (int j = 0; j < m; j++) {
        scanf("%d", &Available[j]);
    }

    /* Input Max matrix */
    printf("\nEnter Max matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &Max[i][j]);
        }
    }

    /* Input Allocation matrix */
    printf("\nEnter Allocation matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &Allocation[i][j]);
        }
    }

    /* Compute Need matrix: Need = Max - Allocation */
    for (int i = 0; i < n; i++) {
        Finish[i] = false;
        for (int j = 0; j < m; j++) {
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }

    /* Initialize Work = Available */
    for (int j = 0; j < m; j++) {
        Work[j] = Available[j];
    }

    int count = 0;

    /* Safety Algorithm */
    while (count < n) {
        bool found = false;

        for (int i = 0; i < n; i++) {
            if (!Finish[i]) {
                bool canRun = true;

                /* Check Needi ≤ Work */
                for (int j = 0; j < m; j++) {
                    if (Need[i][j] > Work[j]) {
                        canRun = false;
                        break;
                    }
                }

                if (canRun) {
                    /* Pretend Ti finishes, release resources */
                    for (int j = 0; j < m; j++) {
                        Work[j] += Allocation[i][j];
                    }

                    SafeSequence[count++] = i;
                    Finish[i] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            printf("\nSystem is in an UNSAFE state (no safe sequence).\n");
            return 0;
        }
    }

    /* Output safe sequence */
    printf("\nSystem is in a SAFE state.\nSafe sequence: ");
    for (int i = 0; i < n; i++) {
        printf("T%d ", SafeSequence[i]);
    }
    printf("\n");

    return 0;
}
