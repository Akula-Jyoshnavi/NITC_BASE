#include "Schema.h"

#include <cmath>
#include <cstring>
#include<bits/stdc++.h>
using namespace std;

int Schema::openRel(char relName[ATTR_SIZE]) {
  int ret = OpenRelTable::openRel(relName);
  // a valid rel-id will be within the range 0 <= relId < MAX_OPEN and any
  // error codes will be negative
  if(ret >= 0&&ret<12){
    return SUCCESS;
  }

  return ret;
}

int Schema::closeRel(char relName[ATTR_SIZE]) {
  if (!strcmp(RELCAT_RELNAME,relName)||!strcmp(ATTRCAT_RELNAME,relName)) {
    return E_NOTPERMITTED;
  }
  int relId = OpenRelTable::getRelId(relName);
  
  if (relId==E_RELNOTOPEN) {
    return E_RELNOTOPEN;
    
  }

  return OpenRelTable::closeRel(relId);
}

int Schema::renameRel(char oldRelName[ATTR_SIZE], char newRelName[ATTR_SIZE]) {
    if(!strcmp(oldRelName,RELCAT_RELNAME)||!strcmp(oldRelName,ATTRCAT_RELNAME)||!strcmp(newRelName,RELCAT_RELNAME)||!strcmp(newRelName,ATTRCAT_RELNAME))
       return E_NOTPERMITTED;
    
    int relid=OpenRelTable::getRelId(oldRelName);
    if(relid>1&&relid<=12) return E_RELOPEN;

    return  BlockAccess::renameRelation(oldRelName, newRelName);
}

int Schema::renameAttr(char *relName, char *oldAttrName, char *newAttrName) {

    if(relName==RELCAT_RELNAME||relName==ATTRCAT_RELNAME)
       return E_NOTPERMITTED;
    
    int relid=OpenRelTable::getRelId(relName);
    if(relid>1&&relid<=12) return E_RELOPEN;
    
     return BlockAccess::renameAttribute(relName, oldAttrName, newAttrName);
}

int Schema::createRel(char relName[],int nAttrs, char attrs[][ATTR_SIZE],int attrtype[]){
    // declare variable relNameAsAttribute of type Attribute
    // copy the relName into relNameAsAttribute.sVal
    Attribute relname_attr;
    strcpy(relname_attr.sVal,relName);    

    // declare a variable targetRelId of type RecId
    RecId targetRelId;
    // Reset the searchIndex using RelCacheTable::resetSearhIndex()
    RelCacheTable::resetSearchIndex(RELCAT_RELID); 
    // Search the relation catalog (relId given by the constant RELCAT_RELID)
    // for attribute value attribute "RelName" = relNameAsAttribute using
    // BlockAccess::linearSearch() with OP = EQ
    char attrName[ATTR_SIZE];
    strcpy(attrName, RELCAT_ATTR_RELNAME);
    targetRelId=BlockAccess::linearSearch(RELCAT_RELID, attrName,relname_attr , EQ);
    // if a relation with name `relName` already exists  ( linearSearch() does
    //                                                     not return {-1,-1} )
    //     return E_RELEXIST;
    if(targetRelId.block!=-1&&targetRelId.slot!=-1) 
         return E_RELEXIST;
    // compare every pair of attributes of attrNames[] array
    // if any attribute names have same string value,
    //     return E_DUPLICATEATTR (i.e 2 attributes have same value)
    for(int i=0;i<nAttrs;i++)
    {
      for(int j=i+1;j<nAttrs;j++)
      {
         if(!strcmp(attrs[i],attrs[j]))
         {
           return E_DUPLICATEATTR;
         }
      }
    }
    /* declare relCatRecord of type Attribute which will be used to store the
       record corresponding to the new relation which will be inserted
       into relation catalog */
    Attribute relCatRecord[RELCAT_NO_ATTRS];
    // fill relCatRecord fields as given below
    // offset RELCAT_REL_NAME_INDEX: relName
    strcpy(relCatRecord[RELCAT_REL_NAME_INDEX].sVal,relName);
    // offset RELCAT_NO_ATTRIBUTES_INDEX: numOfAttributes
    relCatRecord[RELCAT_NO_ATTRIBUTES_INDEX].nVal=nAttrs;
    // offset RELCAT_NO_RECORDS_INDEX: 0
    relCatRecord[RELCAT_NO_RECORDS_INDEX].nVal=0;
    // offset RELCAT_FIRST_BLOCK_INDEX: -1
    relCatRecord[RELCAT_FIRST_BLOCK_INDEX].nVal=-1;
    // offset RELCAT_LAST_BLOCK_INDEX: -1
    relCatRecord[RELCAT_LAST_BLOCK_INDEX].nVal=-1;
    // offset RELCAT_NO_SLOTS_PER_BLOCK_INDEX: floor((2016 / (16 * nAttrs + 1)))
    // (number of slots is calculated as specified in the physical layer docs)
    relCatRecord[RELCAT_NO_SLOTS_PER_BLOCK_INDEX].nVal=(2016 / (16 * nAttrs + 1));

    int retVal = BlockAccess::insert(RELCAT_RELID, relCatRecord);
    // if BlockAccess::insert fails return retVal
    if(retVal!=SUCCESS) 
      return retVal;
    // (this call could fail if there is no more space in the relation catalog)

    // iterate through 0 to numOfAttributes - 1 :
    for(int i=0;i<nAttrs;i++)
    {
        /* declare Attribute attrCatRecord[6] to store the attribute catalog
           record corresponding to i'th attribute of the argument passed*/
        Attribute attrCatRecord[6];
        // (where i is the iterator of the loop)
        // fill attrCatRecord fields as given below
        // offset ATTRCAT_REL_NAME_INDEX: relName
        strcpy(attrCatRecord[RELCAT_REL_NAME_INDEX].sVal,relName);
        // offset ATTRCAT_ATTR_NAME_INDEX: attrNames[i]
        strcpy(attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal,attrs[i]);
        // offset ATTRCAT_ATTR_TYPE_INDEX: attrTypes[i]
        attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal=attrtype[i];
        // offset ATTRCAT_PRIMARY_FLAG_INDEX: -1
        attrCatRecord[ATTRCAT_PRIMARY_FLAG_INDEX].nVal=-1;
        // offset ATTRCAT_ROOT_BLOCK_INDEX: -1
        attrCatRecord[ATTRCAT_ROOT_BLOCK_INDEX].nVal=-1;
        // offset ATTRCAT_OFFSET_INDEX: i
        attrCatRecord[ATTRCAT_OFFSET_INDEX].nVal=i;

         retVal = BlockAccess::insert(ATTRCAT_RELID, attrCatRecord);
         if(retVal!=SUCCESS){
           deleteRel(relName);
           return retVal;
         }
        /* if attribute catalog insert fails:
             delete the relation by calling deleteRel(targetrel) of schema layer
             return E_DISKFULL
             // (this is necessary because we had already created the
             //  relation catalog entry which needs to be removed)
        */
    }

     return SUCCESS;
}

int Schema::deleteRel(char *relName) {
     
    // if the relation to delete is either Relation Catalog or Attribute Catalog,
    //     return E_NOTPERMITTED
        // (check if the relation names are either "RELATIONCAT" and "ATTRIBUTECAT".
        // you may use the following constants: RELCAT_RELNAME and ATTRCAT_RELNAME)
    if (!strcmp(RELCAT_RELNAME,relName)||!strcmp(ATTRCAT_RELNAME,relName)) {
        return E_NOTPERMITTED;
    }
    // get the rel-id using appropriate method of OpenRelTable class by
    // passing relation name as argument
    int relId = OpenRelTable::getRelId(relName); 
     // if relation is opened in open relation table, return E_RELOPEN
    if (relId != E_RELNOTOPEN) {
      // std::cout<<relId<<E_RELNOTOPEN;
       return E_RELOPEN;
    }
    // Call BlockAccess::deleteRelation() with appropriate argument.
    return BlockAccess::deleteRelation(relName);
    // return the value returned by the above deleteRelation() call

    /* the only that should be returned from deleteRelation() is E_RELNOTEXIST.
       The deleteRelation call may return E_OUTOFBOUND from the call to
       loadBlockAndGetBufferPtr, but if your implementation so far has been
       correct, it should not reach that point. That error could only occur
       if the BlockBuffer was initialized with an invalid block number.
    */
}

/* This method creates a bplus indexing on an attribute attrName in a relation relName as specified in arguments. */
int Schema::createIndex(char relName[ATTR_SIZE],char attrName[ATTR_SIZE]){
    // if the relName is either Relation Catalog or Attribute Catalog,
        // return E_NOTPERMITTED
        // (check if the relation names are either "RELATIONCAT" and "ATTRIBUTECAT".
        // you may use the following constants: RELCAT_NAME and ATTRCAT_NAME)
	if (
		strcmp(relName, RELCAT_RELNAME) == 0 ||
		strcmp(relName, ATTRCAT_RELNAME) == 0
	) {
		return E_NOTPERMITTED;
	}

    // get the relation's rel-id using OpenRelTable::getRelId() method
	int relId = OpenRelTable::getRelId(relName);

    // if relation is not open in open relation table, return E_RELNOTOPEN
    // (check if the value returned from getRelId function call = E_RELNOTOPEN)
	if (relId == E_RELNOTOPEN) {
		return E_RELNOTOPEN;
	}

    // create a bplus tree using BPlusTree::bPlusCreate() and return the value
    return BPlusTree::bPlusCreate(relId, attrName);
}

/* This method drops the bplus indexing on an attribute attrName in a relation relName as specified in arguments. */
int Schema::dropIndex(char *relName, char *attrName) {
    // if the relName is either Relation Catalog or Attribute Catalog,
        // return E_NOTPERMITTED
        // (check if the relation names are either "RELATIONCAT" and "ATTRIBUTECAT".
        // you may use the following constants: RELCAT_NAME and ATTRCAT_NAME)
	if (
		strcmp(relName, RELCAT_RELNAME) == 0 ||
		strcmp(relName, ATTRCAT_RELNAME) == 0
	) {
		return E_NOTPERMITTED;
	}

    // get the relation's rel-id using OpenRelTable::getRelId() method
	int relId = OpenRelTable::getRelId(relName);

    // if relation is not open in open relation table, return E_RELNOTOPEN
    // (check if the value returned from getRelId function call = E_RELNOTOPEN)
	if (relId == E_RELNOTOPEN) {
		return E_RELNOTOPEN;
	}

    // get the attribute catalog entry corresponding to the attribute
    // using AttrCacheTable::getAttrCatEntry()
	AttrCatEntry attrCatEntry;
	int retVal = AttrCacheTable::getAttrCatEntry(relId, attrName, &attrCatEntry);

    // if getAttrCatEntry() fails, return E_ATTRNOTEXIST
	if (retVal == E_ATTRNOTEXIST) {
		return E_ATTRNOTEXIST;
	}

	/* get the root block from attrcat entry */
    int rootBlock = attrCatEntry.rootBlock;

	/* if attribute does not have an index (rootBlock = -1) */
    if (rootBlock == -1) {
        return E_NOINDEX;
    }

    // destroy the bplus tree rooted at rootBlock using BPlusTree::bPlusDestroy()
    BPlusTree::bPlusDestroy(rootBlock);

    // set rootBlock = -1 in the attribute cache entry of the attribute using
    // AttrCacheTable::setAttrCatEntry()
	attrCatEntry.rootBlock = -1;
	AttrCacheTable::setAttrCatEntry(relId, attrName, &attrCatEntry);

    return SUCCESS;
}


