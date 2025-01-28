#include "RelCacheTable.h"
#include <cstring>
#include<bits/stdc++.h>

RelCacheEntry* RelCacheTable::relCache[MAX_OPEN];

/*
Get the relation catalog entry for the relation with rel-id `relId` from the cache
NOTE: this function expects the caller to allocate memory for `*relCatBuf`
*/
int RelCacheTable::getRelCatEntry(int relId, RelCatEntry* relCatBuf) {
  if (relId < 0 || relId >= MAX_OPEN) {
    return E_OUTOFBOUND;
  }

  if (relCache[relId] == nullptr) {
    return E_RELNOTOPEN;
  }

  *relCatBuf = relCache[relId]->relCatEntry;

  return SUCCESS;
}

int RelCacheTable::setRelCatEntry(int relId, RelCatEntry *relCatBuf) {

  if(relId < 0 || relId >= MAX_OPEN) {
    return E_OUTOFBOUND;
  }

  if(relCache[relId] == nullptr) {
    return E_RELNOTOPEN;
  }
  relCache[relId]->relCatEntry=*relCatBuf;
  // copy the relCatBuf to the corresponding Relation Catalog entry in
  // the Relation Cache Table.
  relCache[relId]->dirty = true;
  // set the dirty flag of the corresponding Relation Cache entry in
  // the Relation Cache Table.

  return SUCCESS;
}
/* Converts a relation catalog record to RelCatEntry struct
NOTE: this function expects the caller to allocate memory for `*relCatEntry`
*/
void RelCacheTable::recordToRelCatEntry(union Attribute record[RELCAT_NO_ATTRS],
                                        RelCatEntry* relCatEntry) {
                                        
  strcpy(relCatEntry->relName, record[RELCAT_REL_NAME_INDEX].sVal);
  relCatEntry->numAttrs = (int)record[RELCAT_NO_ATTRIBUTES_INDEX].nVal;
  relCatEntry->numRecs = (int)record[RELCAT_NO_RECORDS_INDEX].nVal;
  relCatEntry->firstBlk = (int)record[RELCAT_FIRST_BLOCK_INDEX].nVal;
  relCatEntry->lastBlk = (int)record[RELCAT_LAST_BLOCK_INDEX].nVal;
  relCatEntry->numSlotsPerBlk = (int)record[RELCAT_NO_SLOTS_PER_BLOCK_INDEX].nVal;
}

/* This function can be used to convert the Relation Cache entry to the corresponding record
 that can be written back to Relation Catalog block when closing a relation in the cache memory. */
void RelCacheTable::relCatEntryToRecord(RelCatEntry* relCatEntry, union Attribute record[RELCAT_NO_ATTRS]) {
  // left to you
  strcpy(record[RELCAT_REL_NAME_INDEX].sVal, relCatEntry->relName);

  record[RELCAT_NO_ATTRIBUTES_INDEX].nVal = (double)relCatEntry->numAttrs;
  record[RELCAT_NO_RECORDS_INDEX].nVal = (double)relCatEntry->numRecs;
  record[RELCAT_FIRST_BLOCK_INDEX].nVal = (double)relCatEntry->firstBlk;
  record[RELCAT_LAST_BLOCK_INDEX].nVal = (double)relCatEntry->lastBlk;
  record[RELCAT_NO_SLOTS_PER_BLOCK_INDEX].nVal = (double)relCatEntry->numSlotsPerBlk;

}


/* will return the searchIndex for the relation corresponding to `relId
NOTE: this function expects the caller to allocate memory for `*searchIndex`
*/
int RelCacheTable::getSearchIndex(int relId, RecId* searchIndex) {
  if(relId<0||relId>=MAX_OPEN) return E_OUTOFBOUND;
  if (relCache[relId] == nullptr)  return E_RELNOTOPEN ;

  *searchIndex = relCache[relId]->searchIndex;
  return SUCCESS;
}

// sets the searchIndex for the relation corresponding to relId
int RelCacheTable::setSearchIndex(int relId, RecId* searchIndex) {
  if(relId<0||relId>=MAX_OPEN) return E_OUTOFBOUND;
  if (relCache[relId] == nullptr)  return E_RELNOTOPEN ;

  relCache[relId]->searchIndex=*searchIndex;

  return SUCCESS;
}

int RelCacheTable::resetSearchIndex(int relId) {
  RecId* searchIndex=(RecId*)malloc(sizeof(RecId));
  searchIndex->block=-1;
  searchIndex->slot=-1;
  return setSearchIndex( relId,  searchIndex);
  // use setSearchIndex to set the search index to {-1, -1}
}

