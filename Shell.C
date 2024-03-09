#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define Home "/home/ahmeddewedar" 
#define MAX_INPUT_LENGTH 1000
#define LOG_FILE "/home/ahmeddewedar/log_file.txt"



void execute_shell_bultin(char ** str);
int evaluate_expression(char *str);
void shell();
char *read_input();
char **parse_input2(char *str, int k);
char **parse_input(char *str);
int getNumberofWords(char *str);
void execute_command(char ** str,int k);
void setup_environment();
void write_to_log_file(const char *msg);
void reap_child_zombie(); 
void on_child_exit ();

FILE *log_file;



int main()
{
    signal(SIGCHLD, on_child_exit);
    setup_environment();
    shell();
}


void reap_child_zombie() {
    while(1) {
        int status;
        pid_t child = waitpid((pid_t)(-1), &status, WNOHANG);
        if (child == 0)
            break;
        else if (child == -1)
            break;
    }
}

// write to log file when a child process terminated
void write_to_log_file(const char *msg) {
    log_file = fopen(LOG_FILE, "a");
    fprintf(log_file, "%s", msg);
    fclose(log_file);
}

/* when the child process terminates call reap_chhild_process function
*  and write to log file
*/
void on_child_exit () {
    reap_child_zombie();
    write_to_log_file("Child process was terminated\n");
}
char * read_input()
 {
    char* str = (char*)malloc((MAX_INPUT_LENGTH) * sizeof(char));
    gets(str, MAX_INPUT_LENGTH ,stdin);
    return str;
 }


void setup_environment()
{
    chdir(Home);
}

 void shell(){
      char cwd[1024];

      getcwd(cwd, sizeof(cwd));
      
        printf("$<%s>: ",cwd);
        
        char* str=read_input();

        int len =getNumberofWords(str);

        char** strings = parse_input(str);

     while (1)
     {
        
         printf("%s\n",strings[0]);
      
         printf("%s",strings[1]);
         
         if(!strcmp(strings[0],"exit"))exit(1);
        
        switch(evaluate_expression(strings[0])){
            case 1:
                execute_shell_bultin(strings);
                break;
            case 2:
                execute_command(strings,len);
        }
        
         free(strings);
         getcwd(cwd, sizeof(cwd));

         printf("\n$<%s>: ",cwd);char* str=read_input();
         len =getNumberofWords(str);
         strings = parse_input(str);

     }
    
    }

 

int evaluate_expression(char *str) 
 {
       if(!strcmp(str,"echo")||!strcmp(str,"cd")||!strcmp(str,"export"))
      {
           return 1;         
      }
      return 2;
 }

void execute_shell_bultin(char ** str)
{
    if(!strcmp(str[0],"echo"))  
    { 
       if(str[1][0]=="$")
       {
            char x[500];
                int j=0;
                while(str[1][++j]!='\0')
                {
                  x[j-1]=str[1][j];

                }
                x[j-1]='\0';
                
                printf("%s\n",getenv(x));
                
       }else
       {

         printf("%s",str[1]);

       }
    }else if(!strcmp(str[0],"cd"))
    {
        if(!strcmp(str[1],"~"))
         {
              chdir(Home);
         }
         else if(str[1][0]=="$")
       {
            char x[500];
                int j=0;
                while(str[1][++j]!='\0')
                {
                  x[j-1]=str[1][j];

                }
                x[j-1]='\0';
                
                chdir(getenv(x));
                
       }else
       {
         chdir(str[1]);
       }
         perror("cd");
    }else if(!strcmp(str[0],"export"))
    {
           char **strings=parse_input2(str[1],getNumberofWords(str[1]));
               setenv(strings[0], strings[2], 1);
               free(strings);
    }

}






void execute_command(char ** str,int k)
{
    pid_t pid = fork();
    int status;
    if(pid==-1)
     {
        perror("error");

     }
     else if(pid==0)
     {
        if (str[1][0]=='&') {
            printf("process: %d\n", getpid());
            usleep(5000);
            char *args[]={str[0],NULL};
            execvp(args[0],args);
        }
        
       else{ 
        
        
           if(k==1)
           {

              char *x[20];
              x[0]=str[0];
              x[1]=NULL;

              execvp(x[0],x);
                 
           }else{

        
        int n; char ** strings;
        if(str[1][0]=='$')
           {
           
            
                //printf("%s", str[1][0]);
                char x[500];
                int j=1;
                while(str[1][j]!='\0')
                {
                 // printf("scdcds");
                  x[j-1]=str[1][j];
                  j++;
                  //printf("%s\n",x);

                }
                x[j-2]='\0';
                 
                 
                n=getNumberofWords(getenv(x));
               
               strings=parse_input2(getenv(x),n);
                
           }else {
          n=getNumberofWords(str[1]);
        strings=parse_input2(str[1],n);
        }

          if(str[1][0]==34)
          {
            n=getNumberofWords(strings[0]);
              char ** s=parse_input2(strings[0],n);
              free(strings);
              strings=s;
          }
         
        char *  arg[500];
        arg[0]=str[0];
        

         for(int i=0;i<n;i++){      
         arg[1]=strings[i];
         
         arg[2]=NULL;
         int pid2=fork();
         if(pid2==0) execvp(arg[0],arg);
         else{
          if (str[1][0]=='&') {
            waitpid(pid, &status, WNOHANG);
        }
        else {
            waitpid(pid, &status, 0);
        } 
        }  

        //free(arg);
        }
           }
     }}
        
     else
     {
       wait(NULL);        
     }
}
     
int getNumberofWords(char * str)
{  // cd "SS"
    int i=0;
    int len=0;
    int b=1;
    int qoutes=0;
      while(str[i]!='\0')
      {

      if(!qoutes){
         if(b)
         {
           if(str[i]==' '||str[i]==10) i++;
           else {
            b=0;

            len++;

            if(str[i]==34)
            {
              qoutes=1;

            }
             i++;
           }
         }else
         {
             if(str[i]==34)qoutes=1;
            else if(str[i]!=' '&&str[i]!=10)
            {
               i++;
            }else{b=1;i++;}
         }}else
         {

               i++;
               if(str[i]==34){qoutes=0;i++;}
               b=1;
         }

      }


      return len;
}

char** parse_input(char *str)

{
    char** strings = (char**)malloc(2 * sizeof(char*));
    for (int i = 0; i < 2; i++) {
        strings[i] = (char*)malloc((500) * sizeof(char));
    }

    int i=0,j=0;
    int len=0;
    int b=1;

      while(str[i]!='\0'&&len>=0)
      {
         if(b)
            {
               if(str[i]!=' ')b=0;
               else i++;

            }
         else
            {
              if(str[i]==' '){b=1;
              if(len>0)len=-1;
              }
              else
                {
                    strings[0][j]=str[i];
                    
                    len++;
                    i++;
                    j++;
                }
            }
      }
      strings[0][j]='\0';
      j=0;
      len=0;
      while(str[i]!='\0')
        {
            if(b)
            {
               if(str[i]!=' ')b=0;
               else i++;

            }
         else
            {
              if(str[i]==' '){
                 b=1;

                }
              else
                {
                    while(str[i]!='\0'){
                   strings[1][j]=str[i];
                  
                    len++;
                    i++;
                    j++;
                    }
                }
            }
        }
        strings[1][j]='\0';

      return strings;
}

char** parse_input2(char *str,int k)
{

     k++;
    char** strings = (char**)malloc(k * sizeof(char*));
    for (int i = 0; i < k; i++) {
        strings[i] = (char*)malloc((500) * sizeof(char));
    }

    int i=0;
    int j=0;
    int len=0;
    int b=0;
    int qoutes=0;
    
      while(str[i]!='\0')
      {
         if(!qoutes){
         if(b)
         {
           if(str[i]==' '||str[i]==10) i++;
           else
          {
            b=0;
            j=0;
            len++;
            if(str[i]==34)
            {
              qoutes=1;
              i++;
            }
          }
         }
          else
         {
            if(str[i]==34)
            {
              qoutes=1;
              i++;
            }
            else if(str[i]!=' '&&str[i]!=10)
            {

               strings[len][j]=str[i];
               j++;
               i++;
            }
            else
            {
                b=1;
                if(str[0]==' ' && i==0)len=-1;
                else
                    {
                        strings[len][j]='\0';
                    }
                i++;
                j=0;
            }
         }}
         else
            {
              strings[len][j]=str[i];
               j++;
               i++;

               if(str[i]==34){qoutes=0;i++;}
               b=1;
            }
      }

      return strings;
}
