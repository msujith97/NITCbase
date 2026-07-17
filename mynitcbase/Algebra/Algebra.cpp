#include "Algebra.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>


// will return if a string can be parsed as a floating point number
bool isNumber(char *str) {
  int len;
  float ignore;
  /*
    sscanf returns the number of elements read, so if there is no float matching
    the first %f, ret will be 0, else it'll be 1

    %n gets the number of characters read. this scanf sequence will read the
    first float ignoring all the whitespace before and after. and the number of
    characters read that far will be stored in len. if len == strlen(str), then
    the string only contains a float with/without whitespace. else, there's other
    characters.
  */
  int ret = sscanf(str, "%f %n", &ignore, &len);
  return (ret == 1 && len == strlen(str));
}

/* used to select all the records that satisfy a condition.
the arguments of the function are
- srcRel - the source relation we want to select from
- targetRel - the relation we want to select into. (ignore for now)
- attr - the attribute that the condition is checking
- op - the operator of the condition
- strVal - the value that we want to compare against (represented as a string)
*/ 
// int Algebra::select(char srcRel[ATTR_SIZE], char targetRel[ATTR_SIZE], char attr[ATTR_SIZE], int op, char strVal[ATTR_SIZE])
// {
//     // -------stage 8-----
 
// // int srcRelId = OpenRelTable::getRelId(srcRel);
// // if (srcRelId == E_RELNOTOPEN) {
// //     return E_RELNOTOPEN;
// // }

// // AttrCatEntry attrCatEntry;
// // // get the attribute catalog entry for attr
// // int ret = AttrCacheTable::getAttrCatEntry(srcRelId, attr, &attrCatEntry);
// // if (ret != SUCCESS) {
// //     return E_ATTRNOTEXIST;
// // }

// // // Convert strVal to NUMBER or STRING
// // int type = attrCatEntry.attrType;
// // Attribute attrVal;

// // if (type == NUMBER) {
// //     if (isNumber(strVal)) {
// //         attrVal.nVal = atof(strVal);
// //     } else {
// //         return E_ATTRTYPEMISMATCH;
// //     }
// // }
// // else if (type == STRING) {
// //     strcpy(attrVal.sVal, strVal);
// // }
// // else {
// //     return E_ATTRTYPEMISMATCH;
// // }

// // // Reset search index
// // RelCacheTable::resetSearchIndex(srcRelId);

// // // Get relation metadata
// // RelCatEntry relCatEntry;
// // if (RelCacheTable::getRelCatEntry(srcRelId, &relCatEntry) != SUCCESS) {
// //     return E_RELNOTOPEN;
// // }

// // // Print header
// // printf("|");
// // for (int i = 0; i < relCatEntry.numAttrs; ++i) {
// //     AttrCatEntry attrCatEntry;
// //     AttrCacheTable::getAttrCatEntry(srcRelId, i, &attrCatEntry);
// //     printf(" %s |", attrCatEntry.attrName);
// // }
// // printf("\n");

// // // Iterate through matching records
// // while (true) {

// //     RecId searchRes = BlockAccess::linearSearch(srcRelId, attr, attrVal, op);

// //     if (searchRes.block != -1 && searchRes.slot != -1) {

// //         RecBuffer blockBuffer(searchRes.block);

// //         Attribute record[relCatEntry.numAttrs];
// //         blockBuffer.getRecord(record, searchRes.slot);

// //         printf("|");
// //         for (int i = 0; i < relCatEntry.numAttrs; ++i) {

// //             AttrCatEntry currentAttr;
// //             AttrCacheTable::getAttrCatEntry(srcRelId, i, &currentAttr);

// //             if (currentAttr.attrType == NUMBER) {
// //                 printf(" %g |", record[i].nVal);
// //             } else {
// //                 printf(" %s |", record[i].sVal);
// //             }
// //         }
// //         printf("\n");

// //     } else {
// //         break;
// //     }
// // }

// // return SUCCESS;


// //------stage 8 end


// // // ----stage 9 start

//     // get the srcRel's rel-id (let it be srcRelid), using OpenRelTable::getRelId()
//     // if srcRel is not open in open relation table, return E_RELNOTOPEN
//     int srcRelId = OpenRelTable::getRelId(srcRel);
//     if (srcRelId == E_RELNOTOPEN)
//     {
//         return E_RELNOTOPEN;
//     }

//     // get the attr-cat entry for attr, using AttrCacheTable::getAttrCatEntry()
//     // if getAttrcatEntry() call fails return E_ATTRNOTEXIST
//     AttrCatEntry attrCatEntry;
//     int ret = AttrCacheTable::getAttrCatEntry(srcRelId,attr,&attrCatEntry);
//     if (ret != SUCCESS)
//     {
//         return ret;
//     }


//     /*** Convert strVal to an attribute of data type NUMBER or STRING ***/

//     Attribute attrVal;
//     int type = attrCatEntry.attrType;

//     if (type == NUMBER)
//     {
//         // if the input argument strVal can be converted to a number
//         // (check this using isNumber() function)
//         if (isNumber(strVal))
//         {
//             // convert strVal to double and store it at attrVal.nVal using atof()
//             attrVal.nVal = atof(strVal);
//         }
//         // else
//         else
//         {
//             return E_ATTRTYPEMISMATCH;
//         }
//     }
//     else if (type == STRING)
//     {
//         // copy strVal to attrVal.sVal
//         strcpy(attrVal.sVal,strVal);
//     }

//     /*** Creating and opening the target relation ***/
//     // Prepare arguments for createRel() in the following way:
//     // get RelcatEntry of srcRel using RelCacheTable::getRelCatEntry()
//     RelCatEntry relCatEntry;
//     ret = RelCacheTable::getRelCatEntry(srcRelId,&relCatEntry);
//     if (ret != SUCCESS)
//     {
//         return ret;
//     }
//     int src_nAttrs = relCatEntry.numAttrs;


//     /* let attr_names[src_nAttrs][ATTR_SIZE] be a 2D array of type char
//         (will store the attribute names of rel). */
//     // let attr_types[src_nAttrs] be an array of type int
//     char attr_names[src_nAttrs][ATTR_SIZE];
//     int attr_types[src_nAttrs];

//     /*iterate through 0 to src_nAttrs-1 :
//         get the i'th attribute's AttrCatEntry using AttrCacheTable::getAttrCatEntry()
//         fill the attr_names, attr_types arrays that we declared with the entries
//         of corresponding attributes
//     */
//     for (int i = 0; i < src_nAttrs; i++)
//     {
//         AttrCatEntry attrCatEntry;
//         AttrCacheTable::getAttrCatEntry(srcRelId,i,&attrCatEntry);
//         strcpy(attr_names[i],attrCatEntry.attrName);
//         attr_types[i] = attrCatEntry.attrType;
//     }


//     /* Create the relation for target relation by calling Schema::createRel()
//        by providing appropriate arguments */
//     // if the createRel returns an error code, then return that value.
//     ret=Schema::createRel(targetRel,src_nAttrs,attr_names,attr_types);
//     if (ret != SUCCESS)
//     {
//         return ret;
//     }

//     /* Open the newly created target relation by calling OpenRelTable::openRel()
//        method and store the target relid */
//     int targetRelId = OpenRelTable::openRel(targetRel);
//     if(targetRelId==E_CACHEFULL)
//     {
//         Schema::deleteRel(targetRel);
//         return targetRelId;
//     }
//     /* If opening fails, delete the target relation by calling Schema::deleteRel()
//        and return the error value returned from openRel() */

//     /*** Selecting and inserting records into the target relation ***/
//     /* Before calling the search function, reset the search to start from the
//        first using RelCacheTable::resetSearchIndex() */

//     Attribute record[src_nAttrs];

//     /*
//         The BlockAccess::search() function can either do a linearSearch or
//         a B+ tree search. Hence, reset the search index of the relation in the
//         relation cache using RelCacheTable::resetSearchIndex().
//         Also, reset the search index in the attribute cache for the select
//         condition attribute with name given by the argument `attr`. Use
//         AttrCacheTable::resetSearchIndex().
//         Both these calls are necessary to ensure that search begins from the
//         first record.
//     */
//     RelCacheTable::resetSearchIndex(srcRelId);
//     // read every record that satisfies the condition by repeatedly calling
//     // BlockAccess::search() until there are no more records to be read

//     while (BlockAccess::search(srcRelId,record,attr,attrVal,op)==SUCCESS) {

//         // ret = BlockAccess::insert(targetRelId, record);
//         int ret = BlockAccess::insert(targetRelId,record);

//         // if (insert fails) {
//         //     close the targetrel(by calling Schema::closeRel(targetrel))
//         //     delete targetrel (by calling Schema::deleteRel(targetrel))
//         //     return ret;
//         // }
//         if (ret != SUCCESS)
//         {
//             Schema::closeRel(targetRel);
//             Schema::deleteRel(targetRel);
//             return ret;
//         }
//     }

//     // Close the targetRel by calling closeRel() method of schema layer
//     int m=Schema::closeRel(targetRel);

//     // return SUCCESS.
//     return m;
// }

int Algebra::select(char srcRel[ATTR_SIZE], char targetRel[ATTR_SIZE], char attr[ATTR_SIZE], int op, char strVal[ATTR_SIZE]) {
    // 1. Get the srcRel's rel-id
    int srcRelId = OpenRelTable::getRelId(srcRel);
    if (srcRelId < 0 || srcRelId >= MAX_OPEN) {
        return E_RELNOTOPEN;
    }

    // 2. Get the attr-cat entry for the attribute used in the condition
    AttrCatEntry attrCatEntry;
    int res = AttrCacheTable::getAttrCatEntry(srcRelId, attr, &attrCatEntry);
    if (res != SUCCESS) {
        return E_ATTRNOTEXIST;
    }

    /*** Convert strVal to an attribute of data type NUMBER or STRING ***/
    Attribute attrVal;
    int type = attrCatEntry.attrType;

    if (type == NUMBER) {
        if (isNumber(strVal)) {
            attrVal.nVal = atof(strVal);
        } else {
            return E_ATTRTYPEMISMATCH;
        }
    } else if (type == STRING) {
        strcpy(attrVal.sVal, strVal);
    }

    /*** Creating and opening the target relation ***/
    RelCatEntry srcRelCatEntry;
    RelCacheTable::getRelCatEntry(srcRelId, &srcRelCatEntry);
    int src_nAttrs = srcRelCatEntry.numAttrs;

    char attr_names[src_nAttrs][ATTR_SIZE];
    int attr_types[src_nAttrs];

    // Iterate through attributes to prepare for target relation creation
    for (int i = 0; i < src_nAttrs; i++) {
        AttrCatEntry tempAttrEntry;
        AttrCacheTable::getAttrCatEntry(srcRelId, i, &tempAttrEntry);
        strcpy(attr_names[i], tempAttrEntry.attrName);
        attr_types[i] = tempAttrEntry.attrType;
    }

    // Create the target relation
    res = Schema::createRel(targetRel, src_nAttrs, attr_names, attr_types);
    if (res != SUCCESS) {
        return res;
    }

    // Open the newly created target relation
    int targetRelId = OpenRelTable::openRel(targetRel);
    if (targetRelId < 0) {
        Schema::deleteRel(targetRel);
        return targetRelId; // This will be the error code
    }

    /*** Selecting and inserting records into the target relation ***/
    
    // Reset search indices to ensure we start from the beginning of srcRel
    RelCacheTable::resetSearchIndex(srcRelId);
    AttrCacheTable::resetSearchIndex(srcRelId, attr);

    Attribute record[src_nAttrs];

    // Repeatedly call BlockAccess::search() to find matching records
    while (BlockAccess::search(srcRelId, record, attr, attrVal, op) == SUCCESS) {
        
        res = BlockAccess::insert(targetRelId, record);

        if (res != SUCCESS) {
            Schema::closeRel(targetRel);
            Schema::deleteRel(targetRel);
            return res;
        }
    }

    // Close the targetRel after all matching records are inserted
    Schema::closeRel(targetRel);
    return SUCCESS;
}

int Algebra::insert(char relName[ATTR_SIZE], int nAttrs, char record[][ATTR_SIZE]){
    // if relName is equal to "RELATIONCAT" or "ATTRIBUTECAT"
    // return E_NOTPERMITTED;
    if(strcmp(relName,"RELATIONCAT")==0 || strcmp(relName,"ATTRIBUTECAT")==0)
    {
      return E_NOTPERMITTED;
    }

    // get the relation's rel-id using OpenRelTable::getRelId() method
    int relId = OpenRelTable::getRelId(relName);

    // if relation is not open in open relation table, return E_RELNOTOPEN
    // (check if the value returned from getRelId function call = E_RELNOTOPEN)
    if(relId==E_RELNOTOPEN) return E_RELNOTOPEN;
    // get the relation catalog entry from relation cache
    // (use RelCacheTable::getRelCatEntry() of Cache Layer)
    RelCatEntry relCatEntry;
    RelCacheTable::getRelCatEntry(relId,&relCatEntry);

    /* if relCatEntry.numAttrs != numberOfAttributes in relation,
       return E_NATTRMISMATCH */
    if(relCatEntry.numAttrs!=nAttrs) return E_NATTRMISMATCH;
    // let recordValues[numberOfAttributes] be an array of type union Attribute
    union Attribute recordValues[nAttrs];
    

    /*
        Converting 2D char array of record values to Attribute array recordValues
     */
    // iterate through 0 to nAttrs-1: (let i be the iterator)
    for(int i=0;i<nAttrs;i++)
    {
        // get the attr-cat entry for the i'th attribute from the attr-cache
        // (use AttrCacheTable::getAttrCatEntry())
        AttrCatEntry attrCatEntry;
        AttrCacheTable::getAttrCatEntry(relId,i,&attrCatEntry);

        // let type = attrCatEntry.attrType;
        int type = attrCatEntry.attrType;

        if (type == NUMBER)
        {
            // if the char array record[i] can be converted to a number
            // (check this using isNumber() function)
            if(isNumber(record[i]))
            {
                /* convert the char array to numeral and store it
                   at recordValues[i].nVal using atof() */
                recordValues[i].nVal = atof(record[i]);
            }
            // else
            else
            {
                return E_ATTRTYPEMISMATCH;
            }
        }
        else if (type == STRING)
        {
            // copy record[i] to recordValues[i].sVal
            strcpy(recordValues[i].sVal,record[i]);
        }
    }

    // insert the record by calling BlockAccess::insert() function
    // let retVal denote the return value of insert call
    int retVal = BlockAccess::insert(relId,recordValues);

    return retVal;
}

int Algebra::project(char srcRel[ATTR_SIZE], char targetRel[ATTR_SIZE]) {

    int srcRelId = OpenRelTable::getRelId(srcRel);/*srcRel's rel-id (use OpenRelTable::getRelId() function)*/

    // if srcRel is not open in open relation table, return E_RELNOTOPEN
      if(srcRelId==E_RELNOTOPEN)
      {
        return E_RELNOTOPEN;
      } 
       RelCatEntry relcatentry;
      RelCacheTable::getRelCatEntry(srcRelId,&relcatentry);
    // get RelCatEntry of srcRel using RelCacheTable::getRelCatEntry()
        int numAttrs=relcatentry.numAttrs;
    // get the no. of attributes present in relation from the fetched RelCatEntry.

    // attrNames and attrTypes will be used to store the attribute names
    // and types of the source relation respectively
    char attrNames[numAttrs][ATTR_SIZE];
    int attrTypes[numAttrs];

    for(int i=0;i<numAttrs;i++)
    {
       AttrCatEntry temp;
        AttrCacheTable::getAttrCatEntry(srcRelId, i, &temp);

        strcpy(attrNames[i], temp.attrName);
        attrTypes[i] = temp.attrType;
    }
    /*iterate through every attribute of the source relation :
        - get the AttributeCat entry of the attribute with offset.
          (using AttrCacheTable::getAttrCatEntry())
        - fill the arrays `attrNames` and `attrTypes` that we declared earlier
          with the data about each attribute
    */


    /*** Creating and opening the target relation ***/
    int  ret = Schema::createRel(targetRel, numAttrs, attrNames, attrTypes);
    if (ret != SUCCESS) 
    {
        return ret;
    }
    // Create a relation for target relation by calling Schema::createRel()

    // if the createRel returns an error code, then return that value.
    int targetRelId = OpenRelTable::openRel(targetRel);
    if (targetRelId < 0) 
    {
        Schema::deleteRel(targetRel);
        return targetRelId;
    }
    // Open the newly created target relation by calling OpenRelTable::openRel()
    // and get the target relid
    
    // If opening fails, delete the target relation by calling Schema::deleteRel() of
    // return the error value returned from openRel().


    /*** Inserting projected records into the target relation ***/

    RelCacheTable::resetSearchIndex(srcRelId);
    // Take care to reset the searchIndex before calling the project function
    // using RelCacheTable::resetSearchIndex()

    Attribute record[numAttrs];


    while (BlockAccess::project(srcRelId, record)==SUCCESS /* BlockAccess::project(srcRelId, record) returns SUCCESS */)
    {
        // record will contain the next record
        ret = BlockAccess::insert(targetRelId, record);

        // ret = BlockAccess::insert(targetRelId, proj_record);

        if (ret!=SUCCESS/* insert fails */) {
          Schema::closeRel(targetRel);
            // close the targetrel by calling Schema::closeRel()
          Schema::deleteRel(targetRel);  
            // delete targetrel by calling Schema::deleteRel()
            return ret;
            // return ret;
        }
    }
   Schema::closeRel(targetRel);
    // Close the targetRel by calling Schema::closeRel()
    return SUCCESS;
    // return SUCCESS.
}

int Algebra::project(char srcRel[ATTR_SIZE], char targetRel[ATTR_SIZE], int tar_nAttrs, char tar_Attrs[][ATTR_SIZE]) 
{

    int srcRelId ;/*srcRel's rel-id (use OpenRelTable::getRelId() function)*/
    srcRelId=OpenRelTable::getRelId(srcRel);

    // if srcRel is not open in open relation table, return E_RELNOTOPEN
    if(srcRelId==E_RELNOTOPEN)
    {
        return E_RELNOTOPEN;
    }

    // get RelCatEntry of srcRel using RelCacheTable::getRelCatEntry()
    RelCatEntry relCatEntry;
    RelCacheTable::getRelCatEntry(srcRelId,&relCatEntry);


    // get the no. of attributes present in relation from the fetched RelCatEntry.
    int src_nAttrs = relCatEntry.numAttrs;

    // declare attr_offset[tar_nAttrs] an array of type int.
    int attr_offset[tar_nAttrs];

    // where i-th entry will store the offset in a record of srcRel for the
    // i-th attribute in the target relation.


    // let attr_types[tar_nAttrs] be an array of type int.
    // where i-th entry will store the type of the i-th attribute in the
    // target relation.
    int attr_types[tar_nAttrs];


    /*** Checking if attributes of target are present in the source relation
         and storing its offsets and types ***/

    /*iterate through 0 to tar_nAttrs-1 :
        - get the attribute catalog entry of the attribute with name tar_attrs[i].
        - if the attribute is not found return E_ATTRNOTEXIST
        - fill the attr_offset, attr_types arrays of target relation from the
          corresponding attribute catalog entries of source relation
    */
    for(int i=0;i<tar_nAttrs;i++)
    {
        AttrCatEntry attrCatEntry;
        int ret = AttrCacheTable::getAttrCatEntry(srcRelId,tar_Attrs[i],&attrCatEntry);
        if(ret!=SUCCESS)
        {
            return E_ATTRNOTEXIST;
        }
        attr_offset[i]=attrCatEntry.offset;
        attr_types[i]=attrCatEntry.attrType;
    }


    /*** Creating and opening the target relation ***/


    // Create a relation for target relation by calling Schema::createRel()
    int x=Schema::createRel(targetRel,tar_nAttrs,tar_Attrs,attr_types);
    if(x!=SUCCESS)
    {
        return x;
    }

    // if the createRel returns an error code, then return that value.
    

    // Open the newly created target relation by calling OpenRelTable::openRel()
    // and get the target relid
    int targetRelId = OpenRelTable::openRel(targetRel);


    // If opening fails, delete the target relation by calling Schema::deleteRel()
    // and return the error value from openRel()
    if(targetRelId<0 || targetRelId>=MAX_OPEN)
    {
        Schema::deleteRel(targetRel);
        return targetRelId;
    }


    /*** Inserting projected records into the target relation ***/

    // Take care to reset the searchIndex before calling the project function
    // using RelCacheTable::resetSearchIndex()
    RelCacheTable::resetSearchIndex(srcRelId);

    Attribute record[src_nAttrs];

    while(BlockAccess::project(srcRelId,record)==SUCCESS) 
    {
        // the variable `record` will contain the next record


        Attribute proj_record[tar_nAttrs];

        //iterate through 0 to tar_attrs-1:
        //    proj_record[attr_iter] = record[attr_offset[attr_iter]]
        for(int i=0;i<tar_nAttrs;i++)
        {
            proj_record[i]=record[attr_offset[i]];
        }

        // ret = BlockAccess::insert(targetRelId, proj_record);
        int ret = BlockAccess::insert(targetRelId,proj_record);

        if (ret!=SUCCESS) 
        {
            // close the targetrel by calling Schema::closeRel()
            // delete targetrel by calling Schema::deleteRel()
            // return ret;
            Schema::closeRel(targetRel);
            Schema::deleteRel(targetRel);
            return ret;
        }
    }

    // Close the targetRel by calling Schema::closeRel()
    Schema::closeRel(targetRel);

    // return SUCCESS.
    return SUCCESS;
}

int Algebra::join(char srcRelation1[ATTR_SIZE], char srcRelation2[ATTR_SIZE], char targetRelation[ATTR_SIZE], char attribute1[ATTR_SIZE], char attribute2[ATTR_SIZE]) {

    // 1. Get Relation IDs
    int relid1 = OpenRelTable::getRelId(srcRelation1);
    int relid2 = OpenRelTable::getRelId(srcRelation2);

    if (relid1 == E_RELNOTOPEN || relid2 == E_RELNOTOPEN) {
        return E_RELNOTOPEN;
    }

    // 2. Get Attribute Catalog Entries
    AttrCatEntry attrCatEntry1, attrCatEntry2;
    int ret1 = AttrCacheTable::getAttrCatEntry(relid1, attribute1, &attrCatEntry1);
    int ret2 = AttrCacheTable::getAttrCatEntry(relid2, attribute2, &attrCatEntry2);

    if (ret1 == E_ATTRNOTEXIST || ret2 == E_ATTRNOTEXIST) {
        return E_ATTRNOTEXIST;
    }

    // 3. Type Check
    if (attrCatEntry1.attrType != attrCatEntry2.attrType) {
        return E_ATTRTYPEMISMATCH;
    }

    // 4. Check for Duplicate Attribute Names (excluding join attributes)
    RelCatEntry relcat1, relcat2;
    RelCacheTable::getRelCatEntry(relid1, &relcat1);
    RelCacheTable::getRelCatEntry(relid2, &relcat2);

    for (int i = 0; i < relcat1.numAttrs; i++) {
        AttrCatEntry attr1;
        AttrCacheTable::getAttrCatEntry(relid1, i, &attr1);
        for (int j = 0; j < relcat2.numAttrs; j++) {
            AttrCatEntry attr2;
            AttrCacheTable::getAttrCatEntry(relid2, j, &attr2);
            
            // Skip the join attributes themselves
            if (strcmp(attr1.attrName, attribute1) == 0 && strcmp(attr2.attrName, attribute2) == 0) {
                continue;
            }
            if (strcmp(attr1.attrName, attr2.attrName) == 0) {
                return E_DUPLICATEATTR;
            }
        }
    }

    // 5. Ensure Index on Attribute 2 (Inner Relation)
    if (attrCatEntry2.rootBlock == -1) {
        int res = BPlusTree::bPlusCreate(relid2, attribute2);
        if (res != SUCCESS) return res;
    }

    // 6. Prepare Target Relation Schema
    int nAttrs1 = relcat1.numAttrs;
    int nAttrs2 = relcat2.numAttrs;
    int nTargetAttrs = nAttrs1 + nAttrs2 - 1;

    char targetRelAttrNames[nTargetAttrs][ATTR_SIZE];
    int targetRelAttrTypes[nTargetAttrs];

    int targetIdx = 0;
    for (int i = 0; i < nAttrs1; i++) {
        AttrCatEntry buf;
        AttrCacheTable::getAttrCatEntry(relid1, i, &buf);
        strcpy(targetRelAttrNames[targetIdx], buf.attrName);
        targetRelAttrTypes[targetIdx] = buf.attrType;
        targetIdx++;
    }
    for (int i = 0; i < nAttrs2; i++) {
        AttrCatEntry buf;
        AttrCacheTable::getAttrCatEntry(relid2, i, &buf);
        if (strcmp(buf.attrName, attribute2) == 0) continue; // Skip join attr in rel2
        strcpy(targetRelAttrNames[targetIdx], buf.attrName);
        targetRelAttrTypes[targetIdx] = buf.attrType;
        targetIdx++;
    }

    // 7. Create and Open Target Relation
    int res = Schema::createRel(targetRelation, nTargetAttrs, targetRelAttrNames, targetRelAttrTypes);
    if (res != SUCCESS) return res;

    int targetRelId = OpenRelTable::openRel(targetRelation);
    if (targetRelId < 0) {
        Schema::deleteRel(targetRelation);
        return targetRelId; // Returns E_CACHEFULL
    }

    // 8. Join Execution (Nested Loop with Index)
    Attribute record1[nAttrs1], record2[nAttrs2];
    Attribute targetRecord[nTargetAttrs];

    RelCacheTable::resetSearchIndex(relid1);
    
    while (BlockAccess::project(relid1, record1) == SUCCESS) {
        // Reset Search Index for rel2 to perform a fresh search for every record in rel1
        RelCacheTable::resetSearchIndex(relid2);
        AttrCacheTable::resetSearchIndex(relid2, attribute2);

        while (BlockAccess::search(relid2, record2, attribute2, record1[attrCatEntry1.offset], EQ) == SUCCESS) {
            // Combine records
            int k = 0;
            for (int i = 0; i < nAttrs1; i++) targetRecord[k++] = record1[i];
            for (int i = 0; i < nAttrs2; i++) {
                if (i == attrCatEntry2.offset) continue;
                targetRecord[k++] = record2[i];
            }

            if (BlockAccess::insert(targetRelId, targetRecord) != SUCCESS) {
                OpenRelTable::closeRel(targetRelId);
                Schema::deleteRel(targetRelation);
                return E_DISKFULL;
            }
        }
    }

    OpenRelTable::closeRel(targetRelId);
    return SUCCESS;
}