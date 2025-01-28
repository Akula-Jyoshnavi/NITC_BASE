#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
#include "FrontendInterface/FrontendInterface.h"
#include <bits/stdc++.h>
using namespace std;



//------------------------------------------------  STAGE-1   -------------------------------------------------------------------------

/*int main(int argc, char *argv[]) {
  // Initialize the Run Copy of Disk 
  Disk disk_run;
  //StaticBuffer buffer;
  //OpenRelTable cache;
  
  unsigned char buffer[BLOCK_SIZE];
  char message[]="hello";
  Disk::readBlock(buffer, 1000);
  memcpy( buffer,message , 6);
  Disk::writeBlock(buffer, 1000);


  unsigned char buffer2[BLOCK_SIZE];
  char message2[6];
  Disk::readBlock(buffer2, 1000);
  memcpy(message2, buffer2 , 6);
  std::cout << message2;

  return 0;
  //return FrontendInterface::handleFrontend(argc, argv);
}*/

// ------------------------------------------------exercise stage-1 -------------------------------------------------------------------

/*void read_block(int no)
{
  
  unsigned char buffer[BLOCK_SIZE];
  Disk::readBlock(buffer, no);
   
   for(int i=0;i<BLOCK_SIZE;i++)
   {
     cout<<(int)buffer[i]<<" ";
   }
  
  cout<<endl;
}

int main(int argc, char *argv[]) {
  Disk disk_run;
  for(int i=0;i<4;i++)
  {
    read_block(i);
  } 
  return 0;
}*/

// ------------------------------------------------------------STAGE-2 --------------------------------------------------------------


/*int main(int argc, char *argv[]) {
  Disk disk_run;
  StaticBuffer buffer;
  OpenRelTable cache;
  
  // create objects for the relation catalog and attribute catalog
  RecBuffer relCatBuffer(RELCAT_BLOCK);
  RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

  HeadInfo relCatHeader;
  HeadInfo attrCatHeader;

  // load the headers of both the blocks into relCatHeader and attrCatHeader.
  // (we will implement these functions later)
  relCatBuffer.getHeader(&relCatHeader);
  attrCatBuffer.getHeader(&attrCatHeader);

  for ( int i = 0;i<relCatHeader.numEntries;i++) {

    Attribute relCatRecord[RELCAT_NO_ATTRS]; // will store the record from the relation catalog

    relCatBuffer.getRecord(relCatRecord, i);

    printf("Relation: %s\n", relCatRecord[RELCAT_REL_NAME_INDEX].sVal);
    char*  relCatstr=relCatRecord[RELCAT_REL_NAME_INDEX].sVal;

    for (int j=0;j<attrCatHeader.numEntries;j++) {

      // declare attrCatRecord and load the attribute catalog entry into it
       Attribute attrCatRecord[ATTRCAT_NO_ATTRS]; 

       attrCatBuffer.getRecord(attrCatRecord, j);
       char * attrCatstr=attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal;
       //printf("\n hi \n %s \t %s\n",relCatstr,attrCatstr);
      if (!strcmp(relCatstr,attrCatstr)) {
        const char *attrType = attrCatRecord[ATTR
        CAT_ATTR_TYPE_INDEX].nVal == NUMBER ? "NUM" : "STR";
        printf("  %s: %s\n", attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, attrType);
      }
    }
    printf("\n");
  }
  return 0;
}*/



// -------------------------------------------------------EXERCISE STAGE-2 Q1 ----------------------------------------------------


/*int main(int argc, char *argv[]) {
  Disk disk_run;
  StaticBuffer buffer;
  OpenRelTable cache;
  
  RecBuffer relCatBuffer(RELCAT_BLOCK);  
  HeadInfo relCatHeader;  
  relCatBuffer.getHeader(&relCatHeader);
  
  for ( int i = 0;i<relCatHeader.numEntries;i++) {
    int block_num=ATTRCAT_BLOCK;
    
    Attribute relCatRecord[RELCAT_NO_ATTRS]; 

    relCatBuffer.getRecord(relCatRecord, i);

    printf("Relation: %s\n", relCatRecord[RELCAT_REL_NAME_INDEX].sVal);
    char*  relCatstr=relCatRecord[RELCAT_REL_NAME_INDEX].sVal;
    
    while(1)
    {
    
      RecBuffer attrCatBuffer(block_num);
      HeadInfo attrCatHeader;
      attrCatBuffer.getHeader(&attrCatHeader);
      
     for (int j=0;j<attrCatHeader.numEntries;j++) {

       Attribute attrCatRecord[ATTRCAT_NO_ATTRS]; 

       attrCatBuffer.getRecord(attrCatRecord, j);
       char * attrCatstr=attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal;
       
      if (!strcmp(relCatstr,attrCatstr)) {
        const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER ? "NUM" : "STR";
        printf("  %s: %s\n", attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, attrType);
         }
      }
      
      block_num=attrCatHeader.rblock;
      if(block_num==-1) break;
    }
    printf("\n");
  }
  return 0;
}*/

//-------------------------------------------------------EXERCISE STAGE-2 Q2 ----------------------------------------------------


/*void print_relation(){
   RecBuffer relCatBuffer(RELCAT_BLOCK);
  RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

  HeadInfo relCatHeader;
  HeadInfo attrCatHeader;

  
  relCatBuffer.getHeader(&relCatHeader);
  attrCatBuffer.getHeader(&attrCatHeader);
  
   
  for ( int i = 0;i<relCatHeader.numEntries;i++) {

    Attribute relCatRecord[RELCAT_NO_ATTRS]; 
    relCatBuffer.getRecord(relCatRecord, i);

    printf("Relation: %s\n", relCatRecord[RELCAT_REL_NAME_INDEX].sVal);
    char*  relCatstr=relCatRecord[RELCAT_REL_NAME_INDEX].sVal;

    for (int j=0;j<attrCatHeader.numEntries;j++) {

      
       Attribute attrCatRecord[ATTRCAT_NO_ATTRS]; 

       attrCatBuffer.getRecord(attrCatRecord, j);
       char * attrCatstr=attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal;
       
      if (!strcmp(relCatstr,attrCatstr)) {
        const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER ? "NUM" : "STR";
        printf("  %s: %s\n", attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, attrType);
        if(!strcmp(attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal,"Class")&&!strcmp(attrCatstr,"Students"))
        {   
             strcpy(attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal,"Batch");
             attrCatBuffer.setRecord(attrCatRecord,j);
        }
      }
    }
    printf("\n");
  }
  
}
int main(int argc, char *argv[]) {
   Disk disk_run;
   StaticBuffer buffer;
   OpenRelTable cache;
   
   printf("Before updating schema \n");
   print_relation();
   printf("\nAfter updating schema \n");
   print_relation();
  
  return 0;
}*/




//---------------------------------------------------------Stage-3--------------------------------------------------------------------
/*int main(int argc, char *argv[]) {
  Disk disk_run;
  StaticBuffer buffer;
  OpenRelTable cache;

  for (int i = 0 ;i <= 3;i++){ 
      RelCatEntry* relcat=(RelCatEntry*)malloc(sizeof(RelCatEntry));
      RelCacheTable::getRelCatEntry(i,relcat);
      printf("Relation: %s\n", relcat->relName);
      for (int j = 0;j < relcat->numAttrs;j++) {
          AttrCatEntry* attrcat=(AttrCatEntry*)malloc(sizeof(AttrCatEntry));
          AttrCacheTable::getAttrCatEntry(i,j,attrcat);
          const char *attrType = attrcat->attrType== NUMBER ? "NUM" : "STR";
          printf("  %s: %s\n",attrcat->attrName,attrType);
         }

  }
  return 0;
}*/



/*int read_block(int no)
{
  
  unsigned char buffer[BLOCK_SIZE];
  Disk::readBlock(buffer, no);
   
   for(int i=0;i<BLOCK_SIZE;i++)
   {   
     int num=(int)buffer[i];
     if(num==3) return i;
   }
  return 0;
  cout<<endl;
}


int getfreeblock(){
   for(int i=0;i<4;i++)
  {
    int num=read_block(i);
    return i*BLOCK_SIZE+num;
  } 
  return 0;
}
int main(int argc, char *argv[]) {
  Disk disk_run;
  StaticBuffer buffer;
  OpenRelTable cache;
 
  int num=getfreeblock();
  
  RecBuffer Buffer(num);  
  HeadInfo CatHeader;  
  Buffer.getHeader(&CatHeader);
  
  Attribute CatRecord[CatHeader.numAttrs]; 
  Buffer.getRecord(CatRecord,0);
  
  for(int i=0;i<CatHeader.numAttrs;i++)
  {
    
    CatRecord[i]=12;
  }
  Buffer.setRecord(CatRecord,0);
  
  return 0;
}*/



// -----------------------------------------------------STAGE-3 Q1---------------------------------------------------------------------
/*int main(int argc, char *argv[]) {
  Disk disk_run;
  StaticBuffer buffer;
  OpenRelTable cache;
 
  RecBuffer relCatBuffer(RELCAT_BLOCK);
  HeadInfo relCatHeader;
  relCatBuffer.getHeader(&relCatHeader);
    for ( int i = 0;i<relCatHeader.numEntries;i++) 
    {
      Attribute relCatRecord[RELCAT_NO_ATTRS]; 
      relCatBuffer.getRecord(relCatRecord, i);

      //printf("Relation: %s\n", relCatRecord[RELCAT_REL_NAME_INDEX].sVal);
      char*  relCatstr=relCatRecord[RELCAT_REL_NAME_INDEX].sVal;
      if(!strcmp(relCatstr,"marks"))
      {
        printf("Relation: %s\n", relCatRecord[RELCAT_REL_NAME_INDEX].sVal);
         int firstblock=relCatRecord[RELCAT_FIRST_BLOCK_INDEX].nVal;
          RecBuffer Buffer(firstblock);
           HeadInfo Header;
          Buffer.getHeader(&Header);
         Attribute CatRecord[Header.numEntries]; 
         Buffer.getRecord(CatRecord,0);
         for(int i=0;i<=Header.numEntries;i++)
         {
           printf(" %d",(int)CatRecord[i].nVal);
         }
      }
    }
  
}*/

int main(int argc, char *argv[]) {
  Disk disk_run;
  StaticBuffer buffer;
  OpenRelTable cache;

  return FrontendInterface::handleFrontend(argc, argv);
}


