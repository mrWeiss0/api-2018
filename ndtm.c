/*
 * ndtm.c:    Non-deterministic Turing machine simulator
 *
 * Author:    Giorgio Pristia
 *
 * The program takes input file from stdin,
 * parses it and prints it back to stdout.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define BUFSZ 512

/* Functions to parse each section of the input */
void f_tr (char*);
void f_acc(char*);
void f_max(char*);
void f_run(char*);
void  (* const parse[])(char*) = {f_tr, f_acc, f_max, f_run};

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
                fputs(st_n[st], stdout);
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
            parse[st](lbuf);
            free(lbuf);
            lbuf = NULL;
            lbuf_sz = 0;
        }
    }
    return EXIT_SUCCESS;
}

void f_tr(char *s){
    unsigned int in_st, out_st;
    char in_ch, out_ch;
    char mv;
    sscanf(s, "%u %c %c %c %u",  &in_st, &in_ch, &out_ch, &mv, &out_st);
    printf(   "%u %c %c %c %u\n", in_st,  in_ch,  out_ch,  mv,  out_st);
}

void f_acc(char *s){
    unsigned int st = atoi(s);
    printf("%u\n", st);
}

void f_max(char *s){
    unsigned int max = atoi(s);
    printf("%u\n", max);
}

void f_run(char *s){
    fputs(s, stdout);
}
