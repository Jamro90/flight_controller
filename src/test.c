#include <sys/ioctl.h>                                  //Use of binary forms tog et the terminal size
#include <stdio.h>
#include <string.h>

void writeScr(char *string, int rows, int cols);

int main()
{
    struct winsize w;                                   //struct winsize : will get the screensize width and height.
    ioctl(0, TIOCGWINSZ, &w);                           //TIOCGWINSZ, IOCtl to Get the WINdow SiZe.

    int columns = w.ws_col;                             //w.ws_col : number of columns from IOCTL 
    int rows = w.ws_row;                                //w.ws_row : number of rows from IOCTL

    writeScr("Hello world\n", w.ws_row, w.ws_col);

    return 0;
}

/*Functions*/

void writeScr(char *string, int rows, int cols)
{
    int vertl = rows/2;
    int hortl = 0;
    int stringLength = strlen(string) / 2;

    hortl = (cols - strlen(string))/2;                  //Calculate the center Horizontally

    for (int x = 0; x <= rows; x++)                     //For loop to print blank spaces
    {
        printf("\n");
        if (x == vertl)                                 //If x is in the middle (vertically centered), print the string
        {
            printf("\n%*s\n", cols / 2 + stringLength, string );            //Print string to the center horizontally
        }
    }
}

