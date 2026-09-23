#include "Codexion.h"

int main(int argc, char **argv)
{
    t_data  data;
    int     i;

    if (parse_args(argc, argv, &data) != 0)
        return (1);

    if (init_simulation(&data) != 0)
        return (1);

    printf("\n--- Simulation Initialized Successfully ---\n");
    printf("Start Time: %ld ms\n\n", data.start_time);
    
    i = 0;
    while (i < data.nb_coders)
    {
        printf("Coder [%d] holds -> Left Dongle ID: %d | Right Dongle ID: %d\n",
            data.coders[i].id,
            data.coders[i].left_dongle->id,
            data.coders[i].right_dongle->id);
        i++;
    }
    printf("-------------------------------------------\n");

    free(data.dongle);
    free(data.coders);

    return (0);
}