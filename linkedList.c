#include <stdio.h>
#include <stdlib.h>
#include "rawMode.h"
#include "terminalcontrol.h"
#include "timer.h"
#include "dllist.h"
#include "cli.h"

int main()
{
    enableRawMode();
    run();
    disableRawMode();
    return 0;
}