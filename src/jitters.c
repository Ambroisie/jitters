#include <stdio.h>
#include <stdlib.h>

#include "parse/parse-jitters.h"

int main(void)
{
    int res;
    yydebug = getenv("PARSE") != NULL;
    if (yyparse(&res) == 0) {
        printf("%d\n", res);
    }

    return 0;
}
