#include "lifegame/util.h"
#include "lifegame/stage.h"
#include "lifegame/config.h"

int main()
{
    Stage *stage = new_stage(STAGE_HEIGHT, STAGE_WIDTH);

    int count = 0;

    while (1)
    {
        if (count > SIMULATION_STEP)
        {
            break;
        }

        echo_stage(stage);
        sleep(1);
        step_stage(stage);

        count++;
    }

    return 0;
}