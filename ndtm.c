/*
 * ndtm.c:    Non-deterministic Turing machine simulator
 *
 * Author:    Giorgio Pristia
 *
 * This program takes in input rules, accepting states and max moves
 * for a turing machine, then a list of strings to test the tm for.
 * It prints to stdout 1 for every string accepted by the tm,
 *                     0 for not accepted strings,
 *                     U if the tm does not terminate in an accepting state.
 * When a branch goes over max moves, it's considered not to terminate.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "core.h"

#define BUFSZ 0x800
#define BLANK '_'
#define LEFT  'L'
#define STAY  'S'
#define RIGHT 'R'
#define OUT   "01U"

/* Functions to parse each section of the input */
typedef void parse_funct(char*, struct tm*);
parse_funct f_tr, f_acc, f_max, f_run,
    * const parse[] = {f_tr, f_acc, f_max, f_run};

/*
 * The program takes input divided in 4 sections: tr, acc, max, run.
 * Each section starts with a specific line and is parsed differently
 * depending on the currente parser state,
 * which is updated when a new section is encountered.
 */
int main(){
    char    buf[BUFSZ],      /* Temporary  buffer */
           *lbuf    =  NULL, /* Increasing buffer */
           *st_n[]  = {"tr\n", "acc\n", "max\n", "run\n", ""};
    size_t  buf_sz,
            lbuf_sz =  0;
    int     st      = -1;    /* Current section state */
    struct tm tm;
    int t = tm_init(&tm);
    assert(!t);
    while(!feof(stdin)){
        /*
         * Read characters from input to buf and increase lbuf
         * False if the last iteration filled buf with the last
         * input character, so now stdin is EOF and no new
         * character has to be read
         */
        if(fgets(buf, sizeof(buf), stdin)){
            /* Match the next section and continue */
            if(!strcmp(st_n[st + 1], buf)){
                ++st;
                continue;
            }
            if(st < 0) continue;
            buf_sz = strlen(buf);
            lbuf = realloc(lbuf, lbuf_sz + buf_sz + 1);
            assert(lbuf);
            strcpy(lbuf + lbuf_sz, buf);
            lbuf_sz += buf_sz;
        }
        /*
         * Buffer is empty if buf was filled with the last characters
         * and input ends in \n, so it has already been parsed
         * Otherwise it is parsed if either the end of line or
         * the end of file is reached
         */
        if(lbuf && (lbuf[lbuf_sz - 1] == '\n' || feof(stdin))){
            parse[st](lbuf, &tm);
            free(lbuf);
            lbuf = NULL;
            lbuf_sz = 0;
        }
    }
    tm_destroy(&tm);
    return EXIT_SUCCESS;
}

/******************** Parser functions ********************/

void f_tr(char *s, struct tm *tm){
    struct rule rule;
    symbol ch_f, ch_d;
    char mv;
    sscanf(s, "%u %c %c %c %u", &rule.st_from, &ch_f, &ch_d, &mv,
                                &rule.st_dest);
    rule.ch_from = ch_f == BLANK ? '\0' : ch_f; /* Blanks are replaced */
    rule.ch_dest = ch_d == BLANK ? '\0' : ch_d; /* with zeroes,        */
    switch(mv){                /* Moves with their corresponding value */
        case LEFT:
            rule.mv_dest = -1;
            break;
        case RIGHT:
            rule.mv_dest =  1;
            break;
        case STAY:
            rule.mv_dest =  0;
    }
    int t = rule_dict_insert(tm->rules, &rule);
    assert(!t);
    set_max(tm->accept, rule.st_from > rule.st_dest ?
                        rule.st_from : rule.st_dest );
}

void f_acc(char *s, struct tm *tm){
    int t = set_put(tm->accept, atoi(s));
    assert(t);
}

void f_max(char *s, struct tm *tm){
    tm->max = atoi(s);
}

void f_run(char *s, struct tm *tm){
    tape *t = tape_init(s, BLANK, "\n");
    assert(t);
    struct tmconf *conf = calloc(1, sizeof(*conf));
    assert(conf);
    conf->ttl = tm->max;
    conf->t   = t;
    int v = tm_run(tm, conf);
    assert(v >= 0);
    printf("%c\n", v[OUT]);
}
