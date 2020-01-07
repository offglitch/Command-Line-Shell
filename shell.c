#include <fcntl.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "debug.h"
#include "history.h"
#include "timer.h"
#include "tokenizer.h"

#define BUFFER_SIZE 255
#define BGJOBS_SIZE 10

#define GRN "\x1B[32m"
#define BLU "\x1B[34m"
#define RESET "\x1B[0m"
#define BLD "\e[1m"
#define NRM "\e[1m"

char *tokens[ARG_MAX * 10]; // token array of the command
char *line = NULL;          // command string
int cmd_index = 0;          // number of command
pid_t global_pid = 0;       // pid for child process

struct background_job
{
    pid_t pid; // pid of the background job
    char *cmd; // command of the background job
};

struct background_job bgJobs[BGJOBS_SIZE];

/**
 * Initialize the Background Jobs Array
 *
 * Parameters: void
 * Returns: void
 */
void initBackgroundJob()
{
    // loop the backgroud jobs list
    for (int i = 0; i < BGJOBS_SIZE; i++)
    {
        // intialize the each items
        memset(&bgJobs[i], 0, sizeof(struct background_job));
    }
}

/**
 * Add the new Background Job to Array
 *
 * Parameters:
 * - bgJob: backgroud job structure for new thing
 * Returns: void
 */
void addBackgroundJob(struct background_job bgJob)
{
    // loop the background job list
    for (int i = 0; i < BGJOBS_SIZE; i++)
    {
        // if find the empty item, add the new job to array
        if (bgJobs[i].pid == 0)
        {
            bgJobs[i] = bgJob; // add the new job
            return;
        }
    }
}

/**
 * Remove the exited background job from the Array
 *
 * Parameters:
 * - pid: process id of the exited background job
 * Returns: void
 */
void removeBackgroundJob(pid_t pid)
{
    // loop the background job
    for (int i = 0; i < BGJOBS_SIZE; i++)
    {
        if (bgJobs[i].pid == pid) // if find the same item as pid
        {
            bgJobs[i].pid = 0; // remove the item
        }
    }
}

/**
 * Print the all background jobs
 *
 * Parameters: void
 * Returns: void
 */
void print_backgroundjobs()
{
    // loop the background jobs array
    for (int i = 0; i < BGJOBS_SIZE; i++)
    {
        if (bgJobs[i].pid != 0) // if item is not empty
        {
            printf("%s\n", bgJobs[i].cmd); // print the background job
        }
    }
}

/**
 * Signal handler for SIGCHILD
 * (This handler function will be called when a background job is exited)
 *
 * Parameters: sig-signal number
 * Returns: void
 */
void childHandler(int sig)
{
    pid_t pid = wait(NULL);   // get the pid of the exited background job
    removeBackgroundJob(pid); // remove the exited background job from array
}

/**
 * Signal handler for SIGINT
 * (This handler function will be called when press the ctrl+c)
 *
 * Parameters: sig-signal number
 * Returns: void
 */
void intHandler(int sig)
{
    kill(global_pid, SIGKILL); // kill the running child process
    printf("\n");              // print the enter for next prompt
}

/**
 * Get the HOME directory path
 * 
 * Parameters: void
 * Returns: char point of the HOME directory path string
 */
char *getHomeDir()
{
    struct passwd *pw = getpwuid(getuid()); // get the uid of the current login
    char *m_HomeDir = pw->pw_dir;           // get the HOME directory path
    return m_HomeDir;                       // return the path
}

/**
 * Replace the string
 * 
 * Parameters:
 * - orig: char point of the original source string
 * - rep: char point of the replaced string
 * - with: char point of the new string for replacing 
 * Returns: char point of the replaced string
 */
char *str_replace(char *orig, char *rep, char *with)
{
    char *result;     // the return string
    char *ins;        // the next insert point
    char *tmp = NULL; // varies
    int len_rep;      // length of rep (the string to remove)
    int len_with;     // length of with (the string to replace rep with)
    int len_front;    // distance between rep and end of last rep
    int count;        // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count)
    {
        ins = tmp + len_rep;
    }

    // allocate the temp string
    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--)
    {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

/**
 * Redirect to File (ex: ls -l > out.txt)
 * 
 * Parameters:
 * - cmd: char point of the command string
 * Returns: void
 */
void fileRedirection(char *cmd)
{
    char *tokens[2];       // token array for commands
    char *curr_tok = NULL; // current token string
    int i = 0, j = 0;      // index numbers

    // tokenize the command line(get the command and output file path)
    while ((curr_tok = next_token(&cmd, ">")) != NULL)
    {
        tokens[i] = curr_tok; // add the current token to token array
        i++;                  // increse the number of token
        if (i == 2)
            break;
    }

    char *m_FilePath = str_replace(tokens[1], " ", ""); // output file path
    char *cmdTokens[ARG_MAX];                           // command token array
    char *cmd_token = NULL;                             // initialize the command token

    // get the tokens of the command
    while ((cmd_token = next_token(&tokens[0], " \r\t\n")) != NULL)
    {
        cmdTokens[j] = cmd_token; // add the token to array
        j++;                      // increase the number of token
    }

    cmdTokens[j] = (char *)NULL; // set the EOF the last token

    global_pid = fork(); // create the child process for excute the command
    if (global_pid == 0) // child process part
    {
        remove(m_FilePath);                                 // if the output file is existed, remove it
        int out = open(m_FilePath, O_RDWR | O_CREAT, 0666); // open the output file
        int save_out = dup(fileno(stdout));                 // dup the stdout
        dup2(out, fileno(stdout));                          // redirect the stdout to file

        execvp(cmdTokens[0], cmdTokens); // execute the command

        fflush(stdout);                 // flush the stdout
        fclose(STDIN_FILENO);           // close the stdin for defense the loop input
        close(out);                     // close the file
        dup2(save_out, fileno(stdout)); // redirect stdout to origianl again
        close(save_out);                // close the stdout
    }
    else if (global_pid == -1) // if creating the child process is failed
    {
        perror("fork"); // print error
    }
    else // if parent process
    {
        int status = -1;
        waitpid(global_pid, &status, 0); // wait till child process is exited
    }
}

/**
 * Pipe redirect (ex: ls -l | sort)
 * 
 * Parameters:
 * - cmd: char point of the command string
 * Returns: void
 */
void pipeRedirection(char *cmd)
{
    char *tokens[ARG_MAX]; // token array for command
    char *curr_tok = NULL; // current token string
    int i = 0;             // number of commands

    // split the commands by pipe operator (|)
    while ((curr_tok = next_token(&cmd, "|")) != NULL)
    {
        tokens[i] = curr_tok; // add the command to array
        i++;                  // increase the number of commands
    }

    int m_NumPipes = i - 1;      // get the number of the pipes
    int pipefds[2 * m_NumPipes]; // multiple pipe fds

    // loop the pipe fds
    for (int n = 0; n < m_NumPipes; n++)
    {
        // open the multiple pipes
        if (pipe(pipefds + n * 2) < 0) // if opening pipe is failed
        {
            perror("couldn't pipe"); // output the error
            exit(EXIT_FAILURE);      // exit shell
        }
    }

    char *cmdTokens[ARG_MAX]; // tokens of command
    char *cmd_token = NULL;   // command token

    int pipe_index = 0; // index number of the pipe

    // loop the commands
    for (int index = 0; index < i; index++)
    {
        int j = 0;                             // index of the token
        char *str_cmd = strdup(tokens[index]); // store the command

        // loop the tokens
        while ((cmd_token = next_token(&tokens[index], " \r\t\n")) != NULL)
        {
            cmdTokens[j] = cmd_token; // add the token to array
            j++;                      // increase the token index
        }

        cmdTokens[j] = (char *)NULL; // EOF

        global_pid = fork(); // create the child process for execute the command
        if (global_pid == 0) // if the child process
        {
            if (index < i - 1) // if not last command
            {
                // connect the pipe to stdout
                if (dup2(pipefds[pipe_index + 1], 1) < 0)
                {
                    perror("1-dup2"); // if failure, exit shell
                    exit(EXIT_FAILURE);
                }
            }

            if (pipe_index != 0) // if not first pipe
            {
                // connect the pipe to stdin
                if (dup2(pipefds[pipe_index - 2], 0) < 0)
                {
                    perror("2-dup2"); ///j-2 0 j+1 1
                    exit(EXIT_FAILURE);
                }
            }

            // close the pipes
            for (int n = 0; n < 2 * m_NumPipes; n++)
            {
                close(pipefds[n]);
            }

            // if last command is file redirect
            if (index == (i - 1) && strstr(str_cmd, ">"))
            {
                fileRedirection(str_cmd); // file redirect
            }
            else
            {
                // execute the command
                execvp(cmdTokens[0], cmdTokens);
            }

            fclose(STDIN_FILENO); // close the stdin to defense the loop input
        }
        else if (global_pid == -1) // creating the child process is failed
        {
            perror("fork");
        }

        pipe_index += 2; // increase the pipe index
    }

    // loop the pipes
    for (int n = 0; n < 2 * m_NumPipes; n++)
    {
        close(pipefds[n]); // close the pipes
    }

    int status;
    // loop the child processes
    for (int n = 0; n < m_NumPipes + 1; n++)
        wait(&status); // wait the child processes
}

/**
 * Change current directory command (cd /use/bin)
 * 
 * Parameters:
 * - pth: current directory path for changing
 * Returns: void
 */
int cd(char *pth)
{
    // path buffer
    char path[PATH_MAX];
    memset(path, 0, PATH_MAX);

    if (pth == NULL) // if pth is empty
    {
        strcpy(path, getHomeDir()); // set the HOME directory
    }
    else if (pth[0] == '~') // if pth is ~
    {
        strcpy(path, str_replace(pth, "~", getHomeDir())); // set the HOME directory
    }
    else
    {
        strcpy(path, pth); // copy the path to buffer
    }

    char cwd[PATH_MAX]; // current directory path
    memset(cwd, 0, PATH_MAX);
    if (path[0] != '/') // if not absolutely path
    {                   // true for the dir in cwd
        getcwd(cwd, sizeof(cwd));
        strcat(cwd, "/");
        strcat(cwd, path);
        chdir(cwd); // change the directory
    }
    else // if absolutely path
    {
        chdir(path); // change the directory
    }

    return 0;
}

/**
 * Print the prompt
 * 
 * Parameters: void
 * Returns: void
 */
void print_prompt(void)
{
    char m_UserName[BUFFER_SIZE];   // buffer for user name
    char m_HostName[BUFFER_SIZE];   // buffer for host name
    char m_CurDirPath[BUFFER_SIZE]; // buffer for current dir path

    // initialize the buffer
    memset(m_UserName, 0, BUFFER_SIZE);
    memset(m_HostName, 0, BUFFER_SIZE);
    memset(m_CurDirPath, 0, BUFFER_SIZE);

    getlogin_r(m_UserName, BUFFER_SIZE);  // get the user name
    gethostname(m_HostName, BUFFER_SIZE); // get the host name

    char *m_HomeDir = getHomeDir();
    if (cmd_index == 0)
    {
        cd(m_HomeDir); // change the current directory with HOME directory
    }

    // get the current directory
    getcwd(m_CurDirPath, BUFFER_SIZE);

    // print the shell
    printf(BLD "--[" GRN "%d|%s@%s" RESET BLD ":" BLU "%s" RESET BLD "]--$ " RESET, cmd_index, m_UserName, m_HostName, str_replace(m_CurDirPath, m_HomeDir, "~"));
    fflush(stdout);
}

/**
 * Main function
 * 
 * Parameters: void
 * Returns: return the exit code
 */
int main(void)
{
    initBackgroundJob();           // initialize the background jobs array
    signal(SIGCHLD, childHandler); // register the signal handler for SIGCHILD
    signal(SIGINT, intHandler);    // register the signal handler for SIGINT

    bool m_ScriptMode = !isatty(STDIN_FILENO); // check the script mode
    bool m_BackgroundMode = false;             // flag for background mode

    // process the commands
    while (true)
    {
        m_BackgroundMode = false; // disable the background mode
        if (!m_ScriptMode)        // if not script mode
        {
            print_prompt(); // print the prompt
        }

        size_t line_sz = 0; // size of the commands

        // get the commands from stdin
        int m_res = getline(&line, &line_sz, stdin);
        if (m_ScriptMode && m_res == -1) // if script mode and EOF
        {
            break;
        }

        line[strlen(line) - 1] = '\0'; // set the EOF for commands

        if (!strcmp(line, "")) // if commands is empty
        {
            continue;
        }

        cmd_index++; // increase the number of commands

        char *next_tok = NULL; // char point of the next token
        char *curr_tok;        // char point of the current token

        if (line[0] == '!') // process the history instruct
        {
            next_tok = (char *)malloc(ARG_MAX); // alloc the buffer to next token
            memset(next_tok, 0, ARG_MAX);
            get_cmd(line + 1, next_tok); // get the commands from the history array
        }
        else
        {
            next_tok = line; // process the regular commands
        }

        if (!strcmp(next_tok, "")) // if next token is empty
        {
            free(next_tok);
            continue;
        }

        // add the history
        struct history_entry entry;
        entry.cmd_id = cmd_index - 1; // set the command index
        entry.cmd = strdup(next_tok); // set the command
        char *str_cmd = strdup(next_tok);
        add_history(entry); // add the entry to history array

        // process the instructs (exit, history, jobs)
        if (next_tok == NULL || next_tok[0] == '#') // if #
        {
            continue;
        }
        else if (!strcmp(next_tok, "exit")) // if exit command
        {
            break;
        }
        else if (!strcmp(next_tok, "history")) // if history command
        {
            print_history(); // print the history
            continue;
        }
        else if (!strcmp(next_tok, "jobs")) // if jobs command
        {
            print_backgroundjobs(); // print the background jobs
            continue;
        }

        // process the redirect for pipe & file
        if (strstr(next_tok, "|") || strstr(next_tok, ">"))
        {
            pipeRedirection(next_tok);
            continue;
        }

        int i = 0;

        // tokenize the command
        while ((curr_tok = next_token(&next_tok, " \t\r\n")) != NULL)
        {
            if (curr_tok[0] == '#') // if #
            {
                next_tok = NULL;
                break;
            }

            if (strstr(curr_tok, "$")) // process the parameter expansion
            {
                char *cur_str = strdup(curr_tok); // set the current token
                char *sub_tok = NULL;

                // tokenize the current token
                while ((sub_tok = next_token(&curr_tok, " \t\r\n")) != NULL)
                {
                    if (sub_tok[0] == '$') // if $
                    {
                        // repace the current token ($) with environment value
                        cur_str = str_replace(cur_str, sub_tok, getenv(sub_tok + 1));
                    }
                }

                curr_tok = cur_str; // set the updated token
            }

            tokens[i++] = curr_tok; // add the token to array
        }

        tokens[i] = (char *)NULL; // set the EOF

        if (i > 0 && !strcmp(tokens[0], "cd")) // if "cd" command
        {
            cd(tokens[1]); // change the current path
            continue;
        }
        else if (i == 3 && !strcmp(tokens[0], "setenv")) // if "setenv" command
        {
            setenv(tokens[1], tokens[2], 1); // set environment variable
            continue;
        }

        // process the background job
        if (tokens[i - 1] != NULL && !strcmp(tokens[i - 1], "&"))
        {
            m_BackgroundMode = true; // enable the background job
            tokens[i - 1] = NULL;    // set EOF
        }

        // create the child process
        global_pid = fork();
        if (global_pid == 0)
        {
            // child
            execvp(tokens[0], tokens); // execute the command
            fclose(STDIN_FILENO);      // close the stdin to defense the loop input
        }
        else if (global_pid == -1) // creating child process is failed
        {
            perror("fork");
        }
        else
        {
            // parent
            if (!m_BackgroundMode) // if regular job
            {
                int status = -1;
                waitpid(global_pid, &status, 0);
            }
            else // if background job
            {
                // add the background job to array
                struct background_job m_BgJob; // create the background job struct
                m_BgJob.pid = global_pid;      // set the pid
                m_BgJob.cmd = str_cmd;         // set the command
                addBackgroundJob(m_BgJob);     // add the background job
                m_BackgroundMode = false;      // disable the background job mode
            }
        }

        free(next_tok); // free the next token buffer
    }

    return 0;
}
