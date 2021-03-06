#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#define true 1
#define false 0

//#define DBG_1 1
//#define SHOW 1

#define PATH_LENGHTS 257


char interprate(char* filePath)
{
    FILE* file=fopen(filePath,"r");
    if(file==NULL) return false;
    char cLine[PATH_LENGHTS];
    int lLineLen=PATH_LENGHTS;
    int iBytesRead;
    int iTabSize;
    char fNext=true;
    int iCrrTabId;
    int status=0;

    int pID;
    int i;
    #ifdef DBG_1
    int dbgC=0;
    #endif // DBG_1


    while(!feof(file))
    {
        #ifdef DBG_1
        printf("\n>%d",dbgC);
        dbgC++;
        #endif
        iTabSize=0;
        fgets(cLine,lLineLen,file);

        #ifdef DBG_1
        printf(">%s\n",cLine);
        #endif
        if(cLine[0]=='\0') return true;
        iBytesRead=0;
        for(iBytesRead =0; iBytesRead < lLineLen;iBytesRead++)
        {
            if(*(cLine+ iBytesRead)=='\n' || *(cLine+ iBytesRead)=='#' || *(cLine+ iBytesRead)=='\0' )
            {
                *(cLine+ iBytesRead)='\0';
                break;
            }
        }
        if(iBytesRead==lLineLen)
        {
            return false;
        }
        if(iBytesRead==0 ||iBytesRead==1 ) continue; // if empty line
        printf("%s\n", cLine);
        fNext=false;
        iTabSize=0;
        for(i=0;i<iBytesRead;i++)
        {
            if(*(cLine+ i)!=' ') fNext=true;
            if(*(cLine+ i) ==' ')
            {
                *(cLine+ i)='\0';
                if(fNext)
                    iTabSize++;

                fNext=false;
            }
        }
        if(fNext) iTabSize++;

        char* argT[iTabSize+1]; // ostatni elemnt jest nullem, ale pierwszy token jest wrzucany jako file, albo path;
        iCrrTabId=0;
        fNext=true;
        /*
        for(i=0;i<iBytesRead;i++) // skipp all nulls, formerly spaces
        {
            if(*(cLine+ i)!='\0') break;
        }
        */
        i=0;
        int k=0;
        for(iCrrTabId=0;iCrrTabId<(iTabSize );iCrrTabId++)
        {
            for(;*(cLine +i) =='\0';i++)
                ;
            argT[iCrrTabId]=(cLine +i);
            for(;*(cLine +i) !='\0';i++)
                ;
        }
        argT[iTabSize]=NULL;


        pID=fork();
        if(pID!=0)
        {
            wait(&status);
            if(status!=0)
            {
                return false;
            }
            continue;
        }


        if(*(argT[0])=='.' || *(argT[0])=='/')
        {
            execv(argT[0],argT);
        }
        else
        {
            execvp(argT[0],argT);
        }

        exit(-1);

    }// end while


    return true;
}



int main(int args, char* argv[])
{
    char* cb1="/sysOp/lab3/cw2/cw2/myScript";
    char* path=cb1;
    if(args==2)
    {
        path=argv[1];
    }
    if(false==interprate(path))
        printf("\n-----An fatal error has occured-----\n");

    return 0;
}
