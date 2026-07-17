#include "OpenRelTable.h"
#include<stdlib.h>
#include <cstring>

OpenRelTableMetaInfo OpenRelTable::tableMetaInfo[MAX_OPEN];
OpenRelTable::OpenRelTable() {
/* ---------------- Initialize caches ---------------- */
    for (int i = 0; i < MAX_OPEN; i++) {
        RelCacheTable::relCache[i] = nullptr;
        AttrCacheTable::attrCache[i] = nullptr;
    }

    /*************** Initialize tableMetaInfo ***************/
    for (int i = 0; i < MAX_OPEN; ++i) {
        tableMetaInfo[i].free = true;
        memset(tableMetaInfo[i].relName, 0, ATTR_SIZE);
    }


    /* ---------------- Read RELCAT header ---------------- */
    RecBuffer relCatBuf(RELCAT_BLOCK);
    HeadInfo relCatHeader;
    relCatBuf.getHeader(&relCatHeader);

    int nextRelId = 0;

    /* ---------------- Load ALL relations ---------------- */
    for (int i = 0; i < 2; i++) {

        /* ---- Read relation catalog record ---- */
        Attribute relRec[RELCAT_NO_ATTRS];
        relCatBuf.getRecord(relRec, i);

        /* ---- Create RelCacheEntry ---- */
        RelCacheEntry *relEntry =
            (RelCacheEntry *)malloc(sizeof(RelCacheEntry));

        RelCacheTable::recordToRelCatEntry(relRec, &relEntry->relCatEntry);
        relEntry->recId.block = RELCAT_BLOCK;
        relEntry->recId.slot  = i;
        relEntry->searchIndex.block = -1;
        relEntry->searchIndex.slot  = -1;

        RelCacheTable::relCache[nextRelId] = relEntry;

        /* ---- Build attribute cache linked list ---- */
        RecBuffer attrCatBuf(ATTRCAT_BLOCK);
        HeadInfo attrCatHeader;
        attrCatBuf.getHeader(&attrCatHeader);

        AttrCacheEntry *head = nullptr;
        AttrCacheEntry *tail = nullptr;

        for (int j = 0; j < attrCatHeader.numEntries; j++) {

            Attribute attrRec[ATTRCAT_NO_ATTRS];
            attrCatBuf.getRecord(attrRec, j);

            if (strcmp(
                    attrRec[ATTRCAT_REL_NAME_INDEX].sVal,
                    relEntry->relCatEntry.relName) != 0)
                continue;

            AttrCacheEntry *attrEntry =
                (AttrCacheEntry *)malloc(sizeof(AttrCacheEntry));

            AttrCacheTable::recordToAttrCatEntry(
                attrRec, &attrEntry->attrCatEntry);

            attrEntry->recId.block = ATTRCAT_BLOCK;
            attrEntry->recId.slot  = j;
            attrEntry->next = nullptr;

            if (!head)
                head = tail = attrEntry;
            else {
                tail->next = attrEntry;
                tail = attrEntry;
            }
        }

        AttrCacheTable::attrCache[nextRelId] = head;
        nextRelId++;
    }

    /*************** Set metadata for catalog relations ***************/
    // RELCAT_RELID = 0
    tableMetaInfo[RELCAT_RELID].free = false;
    strcpy(tableMetaInfo[RELCAT_RELID].relName, RELCAT_RELNAME);

    // ATTRCAT_RELID = 1
    tableMetaInfo[ATTRCAT_RELID].free = false;
    strcpy(tableMetaInfo[ATTRCAT_RELID].relName, ATTRCAT_RELNAME);
}


int OpenRelTable::closeRel(int relId) {
	/* rel-id corresponds to relation catalog or attribute catalog*/
	if (relId == ATTRCAT_RELID || relId == RELCAT_RELID) {
		return E_NOTPERMITTED;
	}

	/* 0 <= relId < MAX_OPEN */
	if (relId >= MAX_OPEN || relId < 0) {
		return E_OUTOFBOUND;
	}

	/* rel-id corresponds to a free slot*/
	if (tableMetaInfo[relId].free) {
		return E_RELNOTOPEN;
	}

	/****** Releasing the Relation Cache entry of the relation ******/

	/* RelCatEntry of the relId-th Relation Cache entry has been modified */
	if (RelCacheTable::relCache[relId]->dirty)
	{

		/* Get the Relation Catalog entry from RelCacheTable::relCache
		Then convert it to a record using RelCacheTable::relCatEntryToRecord(). */
		Attribute record[RELCAT_NO_ATTRS];
		RelCatEntry relCatEntry;
		RelCacheTable::getRelCatEntry(relId, &relCatEntry);
		RelCacheTable::relCatEntryToRecord(&relCatEntry, record);

		RecId recId = RelCacheTable::relCache[relId]->recId;
		// declaring an object of RecBuffer class to write back to the buffer
		RecBuffer relCatBlock(recId.block);

		// Write back to the buffer using relCatBlock.setRecord() with recId.slot
		relCatBlock.setRecord(record, recId.slot);
	}
    // free the memory dynamically alloted to this Relation Cache entry
    // and assign nullptr to that entry
	free(RelCacheTable::relCache[relId]);

	/****** Releasing the Attribute Cache entry of the relation ******/

	// free the memory allocated in the attribute caches which was
	// allocated in the OpenRelTable::openRel() function
	AttrCacheEntry* entry = AttrCacheTable::attrCache[relId], * temp = nullptr;
	while (entry) {
		// if the entry has been modified:
		if (entry->dirty) {
            /* Get the Attribute Catalog entry from attrCache
             Then convert it to a record using AttrCacheTable::attrCatEntryToRecord().
             Write back that entry by instantiating RecBuffer class. Use recId
             member field and recBuffer.setRecord() */
			Attribute rec[ATTRCAT_NO_ATTRS];
			AttrCacheTable::attrCatEntryToRecord(&entry->attrCatEntry, rec);
			
			RecBuffer attrCatBlk(entry->recId.block);
			attrCatBlk.setRecord(rec, entry->recId.slot);
        }

		temp = entry;
		entry = entry->next;
		free(temp);
	}

	/****** Set the Open Relation Table entry of the relation as free ******/
	RelCacheTable::relCache[relId] = nullptr;
	AttrCacheTable::attrCache[relId] = nullptr;

	// update `metainfo` to set `relId` as a free slot
	tableMetaInfo[relId].free = true;

	return SUCCESS;
}

OpenRelTable::~OpenRelTable() 
{
  // free all the memory that you allocated in the constructor
   for (int i = 2; i < MAX_OPEN; ++i) 
   {
    if (!tableMetaInfo[i].free) {
      OpenRelTable::closeRel(i); // we will implement this function later
    }
   }
   
    /**** Closing the catalog relations in the relation cache ****/

    //releasing the relation cache entry of the attribute catalog
   RelCacheEntry* rel= RelCacheTable::relCache[ATTRCAT_RELID]; 
    if ( rel->dirty==true /* RelCatEntry of the ATTRCAT_RELID-th RelCacheEntry has been modified */) {
        RelCatEntry Relcatentry=rel->relCatEntry;
        Attribute record[6];
        RelCacheTable::relCatEntryToRecord(&Relcatentry,record);
        /* Get the Relation Catalog entry from RelCacheTable::relCache
        Then convert it to a record using RelCacheTable::relCatEntryToRecord(). */
        RecId recId=rel->recId;
        // declaring an object of RecBuffer class to write back to the buffer
        RecBuffer relCatBlock(recId.block);
         relCatBlock.setRecord(record,recId.slot); 
        // Write back to the buffer using relCatBlock.setRecord() with recId.slot
    }
    // free the memory dynamically allocated to this RelCacheEntry

  free(RelCacheTable::relCache[ATTRCAT_RELID]);
  RelCacheTable::relCache[ATTRCAT_RELID]=nullptr;
    //releasing the relation cache entry of the relation catalog
  rel= RelCacheTable::relCache[RELCAT_RELID];
    if(  rel->dirty==true/* RelCatEntry of the RELCAT_RELID-th RelCacheEntry has been modified */) {

        /* Get the Relation Catalog entry from RelCacheTable::relCache
        Then convert it to a record using RelCacheTable::relCatEntryToRecord(). */
        RelCatEntry Relcatentry=rel->relCatEntry;
        Attribute record[6];
        RelCacheTable::relCatEntryToRecord(&Relcatentry,record);
        /* Get the Relation Catalog entry from RelCacheTable::relCache
        Then convert it to a record using RelCacheTable::relCatEntryToRecord(). */
        RecId recId=rel->recId;
        // declaring an object of RecBuffer class to write back to the buffer
        RecBuffer relCatBlock(recId.block);
         relCatBlock.setRecord(record,recId.slot);
        // declaring an object of RecBuffer class to write back to the buffer
        // Write back to the buffer using relCatBlock.setRecord() with recId.slot
    }
  for (int i = 0; i <2; i++) {
    if(AttrCacheTable::attrCache[i] != nullptr)
    {
        AttrCacheEntry *curr=AttrCacheTable::attrCache[i];
        while(curr!=nullptr)
        {
            AttrCacheEntry *temp=curr;
            curr=curr->next;
            free(temp);
        }
        AttrCacheTable::attrCache[i] = nullptr;
    }
  }
  
}

/* This function will open a relation having name `relName`.
Since we are currently only working with the relation and attribute catalog, we
will just hardcode it. In subsequent stages, we will loop through all the relations
and open the appropriate one.
*/
int OpenRelTable::getRelId(char relName[ATTR_SIZE]) {

    for(int i=0;i<MAX_OPEN;i++)
    {
        if(tableMetaInfo[i].free == false && strcmp(relName,tableMetaInfo[i].relName)==0)
        {
            return i;
        }
    }
  return E_RELNOTOPEN;
}

int OpenRelTable::getFreeOpenRelTableEntry() {

  /* traverse through the tableMetaInfo array,
    find a free entry in the Open Relation Table.*/

    for(int i=0;i<MAX_OPEN;i++)
    {
        if(tableMetaInfo[i].free)
        {
            return i;
        }
    }
    return E_CACHEFULL;

  // if found return the relation id, else return E_CACHEFULL.
}

int OpenRelTable::openRel(char relName[ATTR_SIZE]) {
  if(OpenRelTable::getRelId(relName)!=E_RELNOTOPEN){
    // (checked using OpenRelTable::getRelId())
    return OpenRelTable::getRelId(relName);
    // return that relation id;
  }

  int relId = OpenRelTable::getFreeOpenRelTableEntry();
  /* find a free slot in the Open Relation Table
     using OpenRelTable::getFreeOpenRelTableEntry(). */

  if (relId==E_CACHEFULL){
    return E_CACHEFULL;
  }

  // let relId be used to store the free slot.

  /****** Setting up Relation Cache entry for the relation ******/
  // Reset searchIndex of RELCAT
  RelCacheTable::resetSearchIndex(RELCAT_RELID);

  /* search for the entry with relation name, relName, in the Relation Catalog using
      BlockAccess::linearSearch().
      Care should be taken to reset the searchIndex of the relation RELCAT_RELID
      before calling linearSearch().*/

  // relcatRecId stores the rec-id of the relation `relName` in the Relation Catalog.
  RecId relcatRecId;
  Attribute attrVal;
  strcpy(attrVal.sVal,relName);

  relcatRecId = BlockAccess::linearSearch(RELCAT_RELID,RELCAT_ATTR_RELNAME,attrVal,EQ);

  if (relcatRecId.block == -1 && relcatRecId.slot == -1) {
    // (the relation is not found in the Relation Catalog.)
    return E_RELNOTEXIST;
  }

  /* read the record entry corresponding to relcatRecId and create a relCacheEntry
      on it using RecBuffer::getRecord() and RelCacheTable::recordToRelCatEntry().
      update the recId field of this Relation Cache entry to relcatRecId.
      use the Relation Cache entry to set the relId-th entry of the RelCacheTable.
    NOTE: make sure to allocate memory for the RelCacheEntry using malloc()
  */

  // Read RELCAT record
    HeadInfo head;
    RecBuffer relcatBuffer(relcatRecId.block);
    relcatBuffer.getHeader(&head);
    Attribute record[RELCAT_NO_ATTRS];
    relcatBuffer.getRecord(record,relcatRecId.slot);

    // Create RelCacheEntry
    RelCacheEntry *relEntry = (RelCacheEntry*)malloc(sizeof(RelCacheEntry));
    RelCacheTable::recordToRelCatEntry(record,&relEntry->relCatEntry);
    relEntry->recId = relcatRecId;
    relEntry->searchIndex = {-1, -1};

    // Store in RelCacheTable
    RelCacheTable::relCache[relId]=relEntry;


  /****** Setting up Attribute Cache entry for the relation ******/

    RelCacheTable::resetSearchIndex(ATTRCAT_RELID);
    

  // let listHead be used to hold the head of the linked list of attrCache entries.
  AttrCacheEntry* listHead = nullptr;
  AttrCacheEntry *tail = nullptr;

  /*iterate over all the entries in the Attribute Catalog corresponding to each
  attribute of the relation relName by multiple calls of BlockAccess::linearSearch()
  care should be taken to reset the searchIndex of the relation, ATTRCAT_RELID,
  corresponding to Attribute Catalog before the first call to linearSearch().*/
  while(1)
  {
      /* let attrcatRecId store a valid record id an entry of the relation, relName,
      in the Attribute Catalog.*/
      RecId attrcatRecId = BlockAccess::linearSearch(ATTRCAT_RELID,ATTRCAT_ATTR_RELNAME,attrVal,EQ);

      if(attrcatRecId.block == -1 && attrcatRecId.slot == -1)
      {
        break;
      }

      /* read the record entry corresponding to attrcatRecId and create an
      Attribute Cache entry on it using RecBuffer::getRecord() and
      AttrCacheTable::recordToAttrCatEntry().
      update the recId field of this Attribute Cache entry to attrcatRecId.
      add the Attribute Cache entry to the linked list of listHead .*/
      // NOTE: make sure to allocate memory for the AttrCacheEntry using malloc()

    // Read ATTRCAT record

        RecBuffer attrCatBuf(ATTRCAT_BLOCK);
        HeadInfo attrCatHeader;
        attrCatBuf.getHeader(&attrCatHeader);

        Attribute attrRecord[ATTRCAT_NO_ATTRS];
        RecBuffer attrBuffer(attrcatRecId.block);
        attrBuffer.getRecord(attrRecord,attrcatRecId.slot);

        // Create Attribute Cache Entry
        AttrCacheEntry *attrEntry =
            (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));

        AttrCacheTable::recordToAttrCatEntry(attrRecord,&attrEntry->attrCatEntry);
        attrEntry->recId = attrcatRecId;
        attrEntry->searchIndex = {-1,-1};
        attrEntry->next = NULL;

        // Add to linked list
        if (listHead == NULL) {
            listHead = attrEntry;
            tail = attrEntry;
        } else {
            tail->next = attrEntry;
            tail = attrEntry;
        }

  }

  // set the relIdth entry of the AttrCacheTable to listHead.
  AttrCacheTable::attrCache[relId]=listHead;

  /****** Setting up metadata in the Open Relation Table for the relation******/

  // update the relIdth entry of the tableMetaInfo with free as false and
  // relName as the input.
    strcpy(OpenRelTable::tableMetaInfo[relId].relName, relName);
    OpenRelTable::tableMetaInfo[relId].free = false;

  return relId;
}