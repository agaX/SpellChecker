#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "dictionary.h"
#include "vector.c"
#include "word_list.c"
#include "trie.c"

int main(void) {
    const struct CMUnitTest tests[] = {

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
