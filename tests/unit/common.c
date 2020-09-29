#include <criterion/redirect.h>

void redirect_streams(void)
{
    cr_redirect_stdin();
    cr_redirect_stdout();
    cr_redirect_stderr();
}

void write_to_stdin(const char *input)
{
    FILE *f = cr_get_redirected_stdin();
    fputs(input, f);
    fclose(f); // Make sure to send EOF when parsing
}
