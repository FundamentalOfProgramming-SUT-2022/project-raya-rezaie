#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include<stdbool.h>
#include<time.h>
#include<panel.h>
#include<stdio.h>
#include<windows.h>
#include<unistd.h>

/////////////////////////from main top///////////////
int maxchars=64000*80+1,startrow=1,startcol=1,mxrw=64000,maxcols=92,displayrows=27,returnhandler=1;
        char *file_txt_imp;
        bool ins=false,allowcr=true;
        char c,str[50000];
        char first_part_str[50000];
        int cur_lnum=0,line_num=20,ch=25;
//////////////////////////////////////////////////////
         WINDOW *my_wins[3];
         PANEL  *my_panels[3];
///////////////////////////////////////////////////////

void sampan();

#define DEF_PATH "d:/root"
#define DEF_FLDR "root"


//#define _WIN32_WINNT 0x0500
void showclock();
void ins_k();
void chksv();
void cord();
void off_ins_k();
char* left(char *string, const int length);
void malloc_error(void);
int texteditor(char *text,int maxchars,const int startrow, const int startcol,int mxrw,const int maxcols,const int displayrows, const int returnhandler,const char *chr_allow, bool ins, const bool allowcr);
struct SMALL_RECT {
    SHORT Left;
    SHORT Top;
    SHORT Right;
    SHORT Bottom;
};
int x,y;
bool notsaved=false;

#define DEF_PATH "d:/root"
#define DEF_FLDR "root"

int cat(const char *ptr1,int rows)
{
FILE* ptr;
    char ch;
    int num_lines=0,num_chars=0,n=1,row,col;
    //getmaxyx(stdscr,row,col);
    ptr = fopen(ptr1, "r");
    if (NULL == ptr) {
        wprintw(my_wins[0],"file can't be opened \n");
    }
     while (1)
     {
        ch = fgetc(ptr);
        //if (ch=='\n')
        if (ch=='\0'||ch=='\n'){ 
         num_lines++;
         n++;}
         //(ch == '\n' || ch == '\0')
        if(ch==EOF) break;
        num_chars++;
        printw("%c", ch);
        if(rows>1&&n%(rows-1)==0)
        {
            printw("-----MORE-----");
            getchar();
            printf("\n");
            n=1;
        }

    } 
    fclose(ptr);
    wprintw(my_wins[0],"\n============================================EOF====================================================\n");
    if (num_chars>0) //if file not empty add one to line
    num_lines++;
    wprintw(my_wins[0],"Number of character(s): %d\n",num_chars);
    if(num_chars>0)
    wprintw(my_wins[0],"Number of line(s):      %d\n",num_lines);
    return 0;
}

char** str_chop(const char* str, const char* space, int* num)
{
    int str_num = 0;
    char** words = NULL;
    if (!*str)
    str="EMPTY STRING";    

    while(*str)
    {
       char** tmp1 = realloc(words, (str_num + 1) * sizeof *words);
       words = tmp1;

       char* tmp = strstr(str, space);
       if (tmp == NULL) 
       { 
           int len = strlen(str);
           words[str_num] = calloc(len + 1, 1);
           memcpy(words[str_num], str, len);
           str_num++;    
           break;
       }
       
       int len = tmp - str;
       words[str_num] = calloc(len + 1, 1);
       if (words[str_num] == NULL) exit(1);
       memcpy(words[str_num], str, len);
       str_num++;
       str = tmp + strlen(space);
    }
    *num = str_num;
    return words;
}

int filecreate(char *name )
{
    printf("%s",name);
    FILE *fptr = fopen(name,"w");
    fclose(fptr);
}






int texteditor(char *text, int maxchars, const int startrow, const int startcol,int mxrw,const int maxcols,const int displayrows,const int returnhandler,const char *chr_allow, bool ins, const bool allowcr)

        {
        int keyin,fpos=0,row,col,stsc=0;
        char *txt_edt_mem, *original, *savetext,**txt_on_scr; 
        bool exitflag=false;
/*                                   WINDOW *menubar;
                          //nodelay(stdscr, FALSE);
                        menubar=newwin(1,79,23,1);
                        
                        box(menubar, ACS_VLINE, ACS_HLINE);
                        nodelay(menubar, true); */

        if (!maxchars)
                maxchars=mxrw*maxcols+1;

        if (!mxrw || mxrw > maxchars-1)
                mxrw=maxchars-1;
                
        if (ins)
                curs_set(2);
        else
                curs_set(1);

        if ((txt_on_scr = malloc(mxrw * sizeof(char *))) ==NULL)
                malloc_error();
        for(keyin = 0; keyin < mxrw; keyin++)
                if ((txt_on_scr[keyin] = malloc((maxcols+1) * sizeof(char)))==NULL)
                        malloc_error();
                
        if ((original=malloc(maxchars*sizeof(char)))==NULL)
                malloc_error();
        strcpy(original,text);

        if ((savetext=malloc(maxchars*sizeof(char)))==NULL)
                malloc_error();

        refresh();
        while (!exitflag)
                {

                        showclock();
                       
                int cn_t;
                do
                        {

                        //refresh(menubar);
                        //nodelay(stdscr, false);
                        

                        cn_t=0;
                        txt_edt_mem=text;
                        for (row=0; row < mxrw; row++)
                                {
                                txt_on_scr[row][0]=127;
                                txt_on_scr[row][1]='\0';
                                }
                
                        row=0;
                        while ((strlen(txt_edt_mem) > maxcols || strchr(txt_edt_mem,'\n') != NULL) &&  (txt_on_scr[mxrw-1][0]==127 || txt_on_scr[mxrw-1][0]=='\n'))
                                {
                                strncpy(txt_on_scr[row],txt_edt_mem,maxcols);
                                txt_on_scr[row][maxcols]='\0';
                                if (strchr(txt_on_scr[row],'\n') != NULL)
                                        left(txt_on_scr[row],strchr(txt_on_scr[row],'\n')-txt_on_scr[row]);
                                else
                                        left(txt_on_scr[row],strrchr(txt_on_scr[row],' ')-txt_on_scr[row]);
                                if (txt_on_scr[mxrw-1][0]==127 || txt_on_scr[mxrw-1][0]=='\n')
                                        {
                                        txt_edt_mem+=strlen(txt_on_scr[row]);
                                        if (txt_edt_mem[0]=='\n' || txt_edt_mem[0]==' ' || txt_edt_mem[0]=='\0')
                                                txt_edt_mem++;
                                        row++;
                                        }
                                }
                        if (row == mxrw-1 && strlen(txt_edt_mem) > maxcols) 
                                {
                                strcpy(text,savetext);
                                if (keyin==8) //backspace
                                        fpos++;
                                cn_t=1;
                                }
                        }
                while (cn_t);
                        
                strcpy(txt_on_scr[row],txt_edt_mem);
                
                keyin=0;
                if (strchr(txt_on_scr[mxrw-1],'\n') != NULL) 
                        if (strchr(txt_on_scr[mxrw-1],'\n')[1] != '\0')
                               
                                keyin=8;
                col=fpos;
                row=0;
                cn_t=0;
                while (col > strlen(txt_on_scr[row]))
                        {
                        col-=strlen(txt_on_scr[row]);
                        cn_t+=strlen(txt_on_scr[row]);
                        if (text[cn_t] ==' ' || text[cn_t]=='\n' || text[cn_t]=='\0')
                                {
                                col--;
                                cn_t++;
                                }
                        row++;
                        }
                if (col > maxcols-1)
                        {
                        row++;
                        col=0;
                        }
                
                if (!keyin) 
                        {
                        if (row < stsc)
                                stsc--;
                        if (row > stsc+displayrows-1)
                                stsc++;
                        for (cn_t=0;cn_t < displayrows; cn_t++)
                                {
                                mvhline(cn_t+startrow,startcol,' ',maxcols); //fill blanks with SPACE
                                if (txt_on_scr[cn_t+stsc][0] != 127)
                                        mvprintw(cn_t+startrow,startcol,"%s",txt_on_scr[cn_t+stsc]);
                                }
                        move(row+startrow-stsc,col+startcol);
                        
                        keyin=getch(); 
                        }
                        
                switch(keyin)
                        {
                        case KEY_F(1): 
                                 exitflag=true;
                        case 9: // tab
                        case KEY_BTAB: // shift-tab
                                exitflag=true;
                                break;
                        case KEY_HOME:
                                if (col)
                                        {
                                        fpos=0;
                                        for (col=0; col < row; col++)
                                                {
                                                fpos += strlen(txt_on_scr[col]);
                                                if ((strchr(txt_on_scr[row],'\n') != NULL) || (strchr(txt_on_scr[row],' ') != NULL))
                                                        fpos++;
                                                }
                                        }
                                break;
                        case KEY_END:
                                if (col < strlen(txt_on_scr[row]))
                                        {
                                        fpos=-1;
                                        for (col=0; col <=row; col++)
                                                {
                                                fpos+=strlen(txt_on_scr[col]);
                                                if ((strchr(txt_on_scr[row],'\n') != NULL) || (strchr(txt_on_scr[row],' ') != NULL))
                                                        fpos++;
                                                }
                                        }
                                break;
                        case KEY_LEFT:
                        cord();
                                if (fpos){
                                        fpos--;
                                        }
                                        break;
                        case KEY_RIGHT:
                        
                                if (fpos < strlen(text) && (row != mxrw-1 || col < maxcols-1)){
                                        fpos++;
                                        }      
                                break;
                                
                        case KEY_UP:
                                if (row)
                                        {
                                        if (col > strlen(txt_on_scr[row-1]))
                                                fpos=strlen(txt_on_scr[row-1]);
                                        else
                                                fpos=col;
                                        keyin=0;
                                        for (col=0; col < row-1; col++)
                                                {
                                                fpos+=strlen(txt_on_scr[col]);
                                                keyin+=strlen(txt_on_scr[col]);
                                                if ((strlen(txt_on_scr[col]) < maxcols) || (text[keyin]==' ' && strlen(txt_on_scr[col])==maxcols))
                                                        {
                                                        fpos++;
                                                        keyin++;
                                                        cord();
                                                        }
                                                }
                                        }
                                        
                                 break;
                        case KEY_DOWN:
                                if (row < mxrw-1)
                                        if (txt_on_scr[row+1][0] !=127)
                                                {
                                                if (col < strlen(txt_on_scr[row+1]))
                                                        fpos=col;
                                                else
                                                        fpos=strlen(txt_on_scr[row+1]);
                                                keyin=0;
                                                for (col=0; col <= row; col++)
                                                        {
                                                        fpos+=strlen(txt_on_scr[col]);
                                                        keyin+=strlen(txt_on_scr[col]);
                                                        if ((strlen(txt_on_scr[col]) < maxcols) || (text[keyin]==' ' && strlen(txt_on_scr[col])==maxcols))
                                                                {
                                                                fpos++;
                                                                keyin++;
                                                                 cord();
                                                                
                                                                }
                                                        }
                                                
                                                }
                                               
                                break;
                        case KEY_IC: 
                                ins=!ins;
                                if (ins)
                                        {curs_set(2);
                                         ins_k();}
                                else
                                       { curs_set(1);
                                          off_ins_k();}
                                break;
                        case KEY_DC:
                                if (strlen(text))
                                        {
                                        strcpy(savetext,text);
                                        memmove(&text[fpos],&text[fpos+1],maxchars-fpos);
                                        }
                                break;
                        case 8:
                                if (strlen(text) && fpos)
                                        {
                                        strcpy(savetext,text);
                                        fpos--;
                                        memmove(&text[fpos],&text[fpos+1],maxchars-fpos);
                                        cord();
                                        }
                                break;
                        case 25:
                                break;
                                break;
                               
                               
                                break; 
                        case 10: // return
                        case 459:
                                switch (returnhandler)
                                        {
                                        case 1:
                                                chksv();
                                                if (txt_on_scr[mxrw-1][0] == 127 || txt_on_scr[mxrw-1][0] == '\n')
                                                        {
                                                        memmove(&text[fpos+1],&text[fpos],maxchars-fpos);
                                                        text[fpos]='\n';
                                                        fpos++;
                                                        cord();
                                                        }
                                                        
                                                break;
                                        case 2:
                                                keyin=' ';
                                                ungetch(keyin);
                                                break;
                                        case 3:

                                                exitflag=true;
                                        }
                                break;
                        default:
                                if (((chr_allow==NULL && keyin > 31 && keyin < 127) || (chr_allow != NULL && strchr(chr_allow,keyin))) && strlen(text) < maxchars-1 && (row !=mxrw-1 || (strlen(txt_on_scr[mxrw-1]) < maxcols || (ins && (row!=mxrw-1 && col < maxcols )))))
                                        {
                                                chksv();
                                        if (ins || text[fpos+1]=='\n' || text[fpos]== '\n')
                                                memmove(&text[fpos+1],&text[fpos],maxchars-fpos);
                                                chksv();
                                        text[fpos]=keyin;
                                        if (row != mxrw-1 || col < maxcols-1)
                                                fpos++;
                                        }
                        }
                if(!allowcr)
                        if (text[0]=='\n')
                                {
                                memmove(&text[0],&text[1],maxchars-1);
                                if (fpos)
                                        fpos--;
                                }
                                        
                }
sampan();
        free(original);
        free(savetext);
        for(stsc = 0; stsc < mxrw; stsc++)
                free(txt_on_scr[stsc]);
        free(txt_on_scr);
        
        switch(keyin)
                {
                case 9: // tab
                //sampan();
                        //return 6;
                        break;
                case KEY_BTAB:
                        return 4;

                }
        return 0; 
        }
        

char* left(char *string, const int length)
        {
        if (strlen(string) > length)
                string[length]='\0';
        return string;
        }

void malloc_error(void)
        {
        endwin();
        fprintf(stderr, "malloc error:out of memory\n");
        exit(EXIT_FAILURE);
        }

/* void adjustWindowSize()
{
    struct SMALL_RECT test; 

    HANDLE hStdout;
    COORD coord;
    BOOL ok;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    coord.X = 100;
    coord.Y = 50;
    ok = SetConsoleScreenBufferSize(hStdout, coord);

    test.Left = 0;
    test.Top = 0;
    test.Right = coord.X-1;
    test.Bottom = coord.Y-1;

    SetConsoleWindowInfo(hStdout, ok, &test);

} //end adjustWindowSize  */

void showclock()
{ 
getyx(stdscr, y, x);
wmove(stdscr, 0, 30);
time_t t = time(NULL);
struct tm tm = *localtime(&t);
printw("%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
move(y,x);
}

void chksv()
{
notsaved=true;
getyx(stdscr, y, x);
mvaddstr(28,3,"1.txt");
if (notsaved){
//mvaddch(10, 10, ' ' | COLOR_PAIR(1)) 
//attron(COLOR_PAIR(3));
addch(ACS_DIAMOND| COLOR_PAIR(3)|A_BOLD);}
move(y,x);
}

void ins_k()
{
getyx(stdscr, y, x);
mvaddstr(28,15,"INS");
move(y,x);
}

void off_ins_k()
{
getyx(stdscr, y, x);
move(28,15);
hline(ACS_CKBOARD,3);
move(y,x);
}

void cord()
{
getyx(stdscr, y, x);
wmove(stdscr, 0, 10);
printw("X:%3d,Y:%3d",x,y);
move(y,x);
}


int main()
        {
/*   if (argc < 2)
   {
        printf("Missing Filename\n");
        exit(1);
   } */
        initscr();
        noecho();
        cbreak();
        nodelay(stdscr, TRUE);
        keypad(stdscr,TRUE);

                     resize_term(35, 95); //adjust Window Size;
                     clear();
            HWND consoleWindow = GetConsoleWindow();
            SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
            //lock the console window to prevent sceen croption
        start_color();
        file_txt_imp=malloc(maxchars*sizeof(char));
        FILE *fp = fopen("d:/root/tmp.txt" , "r");
        
        //char *filename;
        // filename = argv[1];
         //FILE* fp = fopen(filename, "r");
        while (fgets(str, sizeof(str), fp) != NULL)
        strcat(file_txt_imp, str);
        init_pair(1,COLOR_WHITE, COLOR_CYAN);
        init_pair(2,COLOR_GREEN,COLOR_BLACK);
        init_pair(3,COLOR_RED,COLOR_BLACK);
        //init_color()
        wbkgd(stdscr, COLOR_PAIR(1));
        init_color(COLOR_CYAN, 10, 20, 750);     
        box(stdscr, ACS_VLINE, ACS_HLINE);
        
        move(28,1);
        hline(ACS_CKBOARD,90);
        move(28,2);
        addch(ACS_RARROW|COLOR_PAIR(2)|A_BOLD);
        mvaddstr(28,3,"1.txt");
        if (notsaved)
        addch(ACS_DIAMOND);

        texteditor(file_txt_imp,maxchars,startrow,startcol,mxrw,maxcols,displayrows,returnhandler,NULL,ins,allowcr);

        clear();
        FILE *fp2 = fopen("d:/root/tmp200.txt" , "w");
        endwin();
        fprintf(fp2, "%s",file_txt_imp);
        fclose(fp);
        fclose(fp2);
        free(file_txt_imp);
        return 0;
        }//main


void sampan()
{
         //WINDOW *my_wins[3];
        //PANEL  *my_panels[3];
                echo();
        //cbreak();
        //nodelay(my_wins[0], TRUE);
        nocbreak();
        //keypad(my_wins[0],TRUE);
        //initscr();
        //nodelay();
        int lines = 20, cols = 80, y = 7, x = 8, i;
        char inp_str[200];
    int str_num,columns, rows,v,start_folder=1;
    //CONSOLE_SCREEN_BUFFER_INFO csbi;
    //GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    //columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    //rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
   
    bool root=true;



        /* Create windows for the panels */
        my_wins[0] = newwin(lines, cols, y, x);



                box(my_wins[0], 0, 0);
        my_panels[0] = new_panel(my_wins[0]);   

       
update_panels();

        doupdate();

        //while(1)


while(1)
{
    mvwprintw(my_wins[0], 1, 1, "Command: ");
    wgetstr(my_wins[0],inp_str);
    char** word = str_chop(inp_str, " ", &str_num);
    if (strcmp(word[0], "createfile") == 0&&strcmp(word[1], "-file") == 0)
    //check if pipe command entered and set falg 
    //parse the entered command
    {   
        if(*word[2]=='\"')
        {
            char** word = str_chop(inp_str, "\"", &str_num);
            char** folders = str_chop(word[1], "/", &str_num);
            if((strcmp(folders[1], DEF_FLDR))==0)
            start_folder=2;
            //printf("%d",v);
            //DEF_FLD
            //chdir(DEF_PATH);
            for (int i=start_folder;i<str_num-1;i++)
            {
                 CreateDirectory (folders[i], NULL);
                 chdir(folders[i]);
            }
            //printf("%d",str_num);
            //printf("%s",folders[str_num-1]);
            //getchar();
            filecreate(folders[str_num-1]);
            //create file  last name in string

            //check for existance of file
            //
            //printf("%d folder(s) created.\n",i);
            //printf("Current folder:%s",folder[i])
        } 
        if(*word[2]=='/')
        {
            char** folders = str_chop(word[2], "/", &str_num);
            if((strcmp(folders[1], DEF_FLDR))==0)
            start_folder=2;
            chdir(DEF_PATH);
            for (int i=2;i<str_num;i++)
            {
                 CreateDirectory (folders[i], NULL);
                 chdir(folders[i]);
            } 
        }
        //for (int i=0;i<str_num;i++)
        //printf("%s\n",folders[i]);
       /*   for (int i=0;i<str_num;i++)
        strcat(folders[0],folders[i]);  
  
            printf("%s\n",folders[0]); */
    }
    else if (strcmp(word[0], "quit") == 0) 
    texteditor(file_txt_imp,maxchars,startrow,startcol,mxrw,maxcols,displayrows,returnhandler,NULL,ins,allowcr);

    //exit(0);
    else if (strcmp(word[0], "cat") == 0)
    {
        cat("readme",rows);
    }
    else if (strcmp(word[0], "compare") == 0)
    {

    }


    else
    {
        //touchwin(my_wins[0]);
     wprintw(my_wins[0],"Invalid Command.\n");
     wprintw(my_wins[0],"Try again or enter \"help\" for list of commands.\n\n");
    }

}//while

//wgetch(my_wins[0]);}
    //gets(inp_str);
          //update_panels();

        /* Show it on the screen */
        

        
        //endwin();
}
