#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dir.h>
#include <process.h>
#define INCL_WINSHELLDATA
#include <OS2.H>

char szTemp[100];
static char buf[100],db[100],ag[100],*s;


void error()
{
  puts("DOSBox loader (c) C.W.R. 2005");
  puts("Has to be called from within a cmd file");
  exit(0);
}

void main(int argc, char *argv[])
{
  FILE *i,*o;
  if (argc < 2) error();
  s=argv[1];
  strcpy(buf,s);
  i=fopen(buf,"rt");
  if (i==0)
  {
    strcat(buf,".cmd");
    i=fopen(buf,"rt");
    if (i==0) error();
  }
  s=getenv("TEMP");
  if (s==0) s=getenv("TMP");
  if (s==0) s=getenv("TMPDIR");
  strcpy(buf,s);
  strcat(buf,"\\db.conf");
  strcpy(ag,buf);
  o=fopen(buf,"wt");
  if (o==0) error();;
  for(;;)
  {
    s=fgets(buf,100,i);
    if (s==0) error();
    if (strncmp(strupr(buf),"DOSBOX_CONF",11)==0) break;
  }
  s=getenv("DOSBOXDIR");
  if (s==0)
  {
    if (PrfQueryProfileString(HINI_USERPROFILE,(PSZ)"DOSBox",
			   (PSZ)"Location",
			    NULL,
			   (PVOID)szTemp,
			   (LONG)sizeof(szTemp)
			   ) == 0) error();
    strcpy(db,szTemp);
  }
  else strcpy(db,s);
  setdisk((db[0] & 7)-1);
  chdir(db);
  strcat(db,"\\dosbox.exe");
  for(;;)
  {
    s=fgets(buf,100,i);
    if (s==0) break;
    fputs(buf,o);
  }
  fclose(i);
  fclose(o);
  spawnlp(P_NOWAIT,db,db,"-conf",ag,NULL);
}

