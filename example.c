#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MNEMONIC_CHAR_LEN  (4)

const char *frq_rmt(const char *s1);
const char *agc_rmt(const char *s1);
const char *atn_rmt(const char *s1);

static uint32_t freq;
const static uint32_t MAX_FREQ = 3000;

static char *pipe;
const static uint8_t MAX_BUFF_SIZE = 32;

/*
** Convert a string to decimal
*/
const uint32_t convert(const char *s1)
{
    uint32_t tmp = 0;

    while ((';' != *s1) && ('\n' != *s1) && ('\0' != *s1))
    {
        tmp *= 10;
        tmp += (*s1 - '0');
        s1++;
    }

    return tmp;
}

const char *frq_rmt(const char *s1)
{
    uint32_t tmp;

    if ('?' == *s1)
    {
        snprintf(pipe, MAX_BUFF_SIZE, "FRQ %d", freq);
        return (pipe);
    }
    else
    {
        tmp = convert(s1);
        if (tmp <= MAX_FREQ)
        {
            freq = tmp;
        }
        return("");
    }
}

const char *agc_rmt(const char *s1)
{
    if ('?' == *s1)
    {
        return ("\nAuto gain control Query");
    }
    else
    {
        return("\nAuto gain control");
    }
}

const char *atn_rmt(const char *s1)
{
    if ('?' == *s1)
    {
        return ("\nAttenuation Query");
    }
    else
    {
        return("\nAttenuation");
    }
}

/*
 * This function converts characters to upper case. It is 
 * best to work on known letter case.
 */
static void upper_case(const char *buf, char *convert)
{
    char tmp;
    uint8_t i;
    uint8_t length = strlen(buf);

    for (i=0; i<length; i++)
    {
        tmp = buf[i];

        if ('\n' == tmp)
        {
            *convert = 0;
            convert++;
        }
        else if (' ' == tmp)
        {
            /* Do nothing */
        }
        else
        {
            if ((tmp > 0x60) && (tmp < 0x7B))
            {
                *convert = ~0x20 & tmp;
            }
            else
            {
                *convert = tmp;
            }
            convert++;
        }
    }
}

/*
** The supplied strstr() has one undesired effect when <enter> only is
** pressed. This eleminates that problem by returning NULL.
**
** This was taken from the Linux kernel source and modified.
*/
static char *stringstring(const char *s1, const char *s2)
{
    size_t l1, l2;

    l2 = strlen(s2);
    if (l2)
    {
        l1 = strlen(s1);
        while (l1 >= l2) {
            l1--;
            if (!memcmp(s1, s2, l2))
            {
                return (char *)s1;
            }
            s1++;
        }
    }
    return NULL;
}

static char *getLetters(const uint8_t length, const char *s1)
{
    uint8_t i;
    char *tmp;

    tmp = (char *) malloc(sizeof(char) * length);
    for(i=0; i<length; i++)
    {
        tmp[i] = s1[i];
    }

    return tmp;
}

static void process_read(const char *buf)
{
    char convert[128];
    char *cmdptr;
    char *needle;
    uint8_t offset;
    uint8_t count;
    uint8_t loop;
    const char *replyptr;

    /* Allowed commands, always 3 characters plus one space (total of 4 characters) */
    static const char read_str[] =
            "FRQ AGC ATN ";

    /*
     * Functions associated with the allowed commands
     * This is a constant function array pointer that accepts
     * an array of characters. It returns an array of 
     * characters, for now.
     * NOTE: All functions needs to be the same format.
     */
    static const char *
    (*const readfns[sizeof(read_str)/MNEMONIC_CHAR_LEN])(const char *s1) =
    {
        frq_rmt,
        agc_rmt,
        atn_rmt,
    };

    upper_case(buf, convert);

    loop = 1;
    count = 0;

    do
    {
        /* This does a malloc, remember to free the memory when done with it */
        needle = getLetters(3, &convert[count]);
        count += 3;

        /* Compare strings to see if a match is present and return result */
        cmdptr = stringstring(read_str, needle);
        free(needle);

        if (NULL != cmdptr)
        {
            /*
             * cmdptr points to the valid command, so compute offset,
             * in order to get entry into function jump table
             */
            offset = (cmdptr - read_str) / MNEMONIC_CHAR_LEN;

            /* Call function and get pointer to reply*/
            replyptr = (*readfns[offset])(&convert[count]);

            /* rest of the code goes here */
            if (strlen(replyptr))
            {
                printf("%s\n", replyptr);
            }

            do
            {
                count += 1;

                if ('\n' == convert[count])
                {
                    loop = 0;
                }
            } while ((';' != convert[count]) && ('\n' != convert[count]));

            /* Once out of the loop, must increment one more time to account for character */
            count += 1;
        }
        else
        {
            loop = 0;
        }
    } while (loop);
}

int main(int argc, char const *argv[])
{
    char str[128];
    char *ptr;

    freq = 200;
    pipe = (char *) malloc(sizeof(char) * MAX_BUFF_SIZE);

    /* Represents a task */
    for (;;)
    {
        ptr = fgets(str, sizeof(str), stdin);
        if ('\0' != *ptr)
        {
            process_read(ptr);
        }

        /* Usually not part of a task. Just present as another method of exit */
        if (NULL != stringstring("exit\n", ptr))
        {
            break;
        }
    }

    free(pipe);
}
