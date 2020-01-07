#include "history.h"

#include <stdio.h>

int historySize = 0;

/**
 * Add the command to history array
 * 
 * Parameters:
 * - entry: command structure (history_entry)
 * Returns: void
 */
void add_history(struct history_entry entry)
{
    if (historySize < HIST_MAX) // if history size is less than MAX
    {
        histories[historySize] = entry; // add the command
        historySize++;                  // increase the history size
    }
    else // else
    {
        // remove the first history entry and move the other entries
        for (int i = 0; i < HIST_MAX - 1; i++)
        {
            memmove(&histories[i], &histories[i + 1], sizeof(struct history_entry));
        }

        histories[HIST_MAX - 1] = entry; // add the history entry
    }
}

// print the history
void print_history()
{
    /* This function should print history entries */
    for (int i = 0; i < historySize; i++)
    {
        printf("%d %s\n", histories[i].cmd_id, histories[i].cmd);
    }
}

void get_cmd(char *input, char *cmd)
{
    if (!strcmp(input, "!")) // if input is !
    {
        strcpy(cmd, histories[historySize - 1].cmd); // get the last command
    }
    else if (!strcmp(input, "0")) // if input is 0
    {
        strcpy(cmd, histories[0].cmd); // get the 0th command
    }
    else // else
    {
        int cmdId = atoi(input);
        if (cmdId == 0) // if the input is string
        {
            // find the last matching command from history array
            for (int i = historySize - 1; i >= 0; i--)
            {
                if (!strncmp(histories[i].cmd, input, strlen(input))) // if match the string
                {
                    strcpy(cmd, histories[i].cmd);
                    return;
                }
            }
        }
        else // if the input is number
        {
            // find the command by the number
            for (int i = 0; i < historySize; i++)
            {
                if (histories[i].cmd_id == cmdId) // if match the number
                {
                    strcpy(cmd, histories[i].cmd);
                    return;
                }
            }
        }
    }
}