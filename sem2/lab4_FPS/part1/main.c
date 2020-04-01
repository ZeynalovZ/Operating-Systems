#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#define BUF_SIZE 0x200

char *outputNames[] = {
    "pid",
    "filename",
    "state",
    "ppid",
    "gid",
    "session",
    "tty_nr",
    "tp_gid",
    "flags",
    "minflt",
    "cminflt",
    "majflt",
    "cmajflt",
    "utime",
    "stime",
    "cutime",
    "cstime",
    "priority",
    "nice",
    "num_threads",
    "itrealvalue",
    "start_tiime",
    "vsize",
    "rss",
    "rsslim",
    "startcode",
    "endcode",
    "startstack",
    "kstkesp",
    "kstkeip",
    "signal",
    "blocked",
    "sigignore",
    "sigcatch",
    "wchan",
    "nswap",
    "cnswap",
    "exit_signal",
    "processor",
    "rt_priority",
    "policy",
    "delayacct_blkio_tics",
    "cquest_time",
    "start_data",
    "end_data",
    "start_brk",
    "arg_start",
    "arg_end",
    "env_start",
    "env_end",
    "exit_code"

};

static int i = 0;


void statOutput(char *buf)
{
    int len = strlen(buf);
    int currentName = 0;
    char *pch = strtok(buf, " ");
    
    while (pch != NULL && i < 51)
    {
        printf("\n%15s:\t %s", outputNames[i], pch);
        pch = strtok(NULL, " ");
        i++;
    }
}

void environOutput(char *buf)
{
    printf("%s\n", buf);
}

void cmdLineOutput(char *buf)
{
    printf("%s\n", buf);
}

void (*func) (char*);

void readFile(char* filePath, void (*func)(char*))
{
    char buf[BUF_SIZE];
    int i, len;
    FILE *f = fopen(filePath, "r");
    while ((len = fread(buf, 1, BUF_SIZE, f)) > 0)
    {
        for (i = 0; i < len; i++)
            if( buf[i] == 0)
                buf[i] = 10;
        buf[len - 1] = 0;
        func(buf);
    }
    fclose(f);
}


int main(int argc, char *argv[])
{
    readFile("/proc/self/stat", statOutput);
    printf("\n=============================\n");
    readFile("/proc/self/environ", environOutput);
    printf("\n=============================\n");
    readFile("/proc/self/cmdline", environOutput);
    printf("\n=============================\n");
    execl("/bin/ls", "ls", "/proc/self/fd", NULL);
    return 0;
}