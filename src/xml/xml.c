/**
 * \file xml.c
 * \brief XML file related functions.
 *
 * Definitions of functions to read and parse a XML file.
 *
 * \author François-Xavier Balu \<fx.balu@gmail.com\>
 * \date 13 mars 2014
 */


#include <stdio.h>   /* printf(), fopen(), fclose(), fgets() */
#include <stdlib.h>  /* malloc(), free() */
#include <string.h>  /* strlen(), strcpy() */

#include "../log.h"  /* logError() */
#include "node.h"    /* XML_Node */
#include "xml.h"


/**
 * \brief Create an initialized XML_File.
 * Allocate memory for a XML_File and initialize it.
 *
 * \return  Created XML_File.
 */
XML_File* createXMLFile(void)
{
   XML_File* xml;

   if((xml = malloc(sizeof(XML_File))) == NULL) {
      logError("Can't allocate memory for XML_File", __FILE__, __LINE__);
   }
   else {
      logMem(LOG_ALLOC, xml, "XML_File", "xml file", __FILE__, __LINE__);
      xml->path = NULL;
      xml->file = NULL;
      xml->root = NULL;
   }

   return xml;
}


/**
 * \brief Destroy a XML_File.
 *
 * \param xml  Destroyed XML_File.
 */
void destroyXMLFile(XML_File* xml)
{
   if(xml == NULL) {
      logError("Trying to destroy a NULL XML_File", __FILE__, __LINE__);
   }
   else {
      /* free path */
      if(xml->path != NULL) {
         logMem(LOG_FREE, xml->path, "string", "xml path", __FILE__, __LINE__);
         free(xml->path);
      }
      /* close file */
      if(xml->file != NULL) {
         logMem(LOG_FREE, xml->file, "file", "xml file", __FILE__, __LINE__);
         fclose(xml->file);
      }
      /* destroy tree */
      if(xml->root != NULL) {
         destroyXMLNode(xml->root);
      }
      /* free XML_File */
      logMem(LOG_FREE, xml, "XML_File", "xml file", __FILE__, __LINE__);
      free(xml);
   }
}


/**
 * \brief Set a file path for a XML_File.
 * Allocate memory for a XML_File's path, and copy path's content in it.
 * If \p xml already has a name, memory is reallocated instead.
 *
 * \param[in] path  Given file path.
 * \param     xml   Modified XML_File.
 */
void setXMLFilePath(const char* path, XML_File* xml)
{
   /* NULL XML_File */
   if(xml == NULL) {
      logError("Giving a file path to a NULL XML_File", __FILE__, __LINE__);
   }
   /* NULL file path */
   else if(path == NULL) {
      logError("Giving a NULL file path to a XML_File", __FILE__, __LINE__);
   }
   /* XML_File already has a file path */
   else if(xml->path != NULL) {
      if((xml->path = realloc(xml->path, (strlen(path) + 1) * sizeof(char))) == NULL) {
         logError("Can't reallocate memory for file path", __FILE__, __LINE__);
      }
      else {
         strcpy(xml->path, path);
      }
   }
   /* XML_File doesn't have a file path */
   else {
      if((xml->path = malloc((strlen(path) + 1) * sizeof(char))) == NULL) {
         logError("can't allocate memory for file path", __FILE__, __LINE__);
      }
      else {
         logMem(LOG_ALLOC, xml->path, "string", "xml path", __FILE__, __LINE__);
         strcpy(xml->path, path);
      }
   }
}


void openXMLFile(XML_File* xml)
{
   if(xml == NULL) {
      logError("Can't open XML file of a NULL XML_File", __FILE__, __LINE__);
   }
   else if(xml->path == NULL) {
      logError("No path found in XML_File", __FILE__, __LINE__);
   }
   else if(xml->file != NULL) {
      logError("File already opened in XML_File", __FILE__, __LINE__);
   }
   else if((xml->file = fopen(xml->path, "r")) == NULL) {
      logError("Can't open file with XML_File's path", __FILE__, __LINE__);
   }
   else {
      logMem(LOG_ALLOC, xml->file, "file", "xml file", __FILE__, __LINE__);
   }
}


void closeXMLFile(XML_File* xml)
{
   if(xml == NULL) {
      logError("Can't close XML file of a NULL XML_File", __FILE__, __LINE__);
   }
   else if(xml->file == NULL) {
      logError("Can't close a NULL file in XML_File", __FILE__, __LINE__);
   }
   else {
      logMem(LOG_FREE, xml->file,"file", "xml file", __FILE__, __LINE__);
      fclose(xml->file);
      xml->file = NULL;
   }
}


int checkFirstLineXMLFile(XML_File* xml)
{
   char firstLine[XML_BUFFER_LENGTH];

   if(xml == NULL) {
      logError("Can't check first line of a NULL XML_File", __FILE__, __LINE__);
   }
   else if(xml->file == NULL) {
      logError("Can't read a NULL file in XML_File", __FILE__, __LINE__);
   }
   else if(fgets(firstLine, XML_BUFFER_LENGTH, xml->file) == NULL) {
      logError("Can't read first line of XML_File", __FILE__, __LINE__);
   }
   else {
      return (strcmp(firstLine, XML_FIRST_LINE) == 0);
   }

   return 0;
}


XML_Node* parseXMLFile(FILE* file)
{
   XML_Node *current, *child, *root;
   XML_Tag* tag;
   int endOfParsing;

   current = child = root = NULL;
   tag = NULL;
   endOfParsing = 0;

   /* read first tag */
   if((tag = readXMLTag(file)) == NULL) {
      logError("Nothing to parse", __FILE__, __LINE__);
      destroyXMLTag(tag);
      return NULL;
   }
   else if(tag->type == CLOSING) {
      logError("First tag is a closing tag", __FILE__, __LINE__);
      destroyXMLTag(tag);
      return NULL;
   }
   else if(tag->type == UNIQUE) {
      root = createXMLNode();
      initXMLNodeFromXMLTag(root, tag);
      destroyXMLTag(tag);
      return root;
   }
   else /* tag->type == OPENING */ {
      current = root = createXMLNode();
      initXMLNodeFromXMLTag(current, tag);
      destroyXMLTag(tag);
   }

   /* read following node's value or tags, if any */
   while(endOfParsing == 0) {
      //reachNextXMLTag(file);  // prevent parsing to read a node's value.
      readXMLNodeValue(current, file);
      tag = readXMLTag(file);

      if(tag == NULL) {
         logError("No tag remaining, and tree isn't finished",
                  __FILE__, __LINE__);
         destroyXMLNode(root);
         return NULL;
      }
      /* Tag opens a child node for current node */
      else if(tag->type == OPENING) {
         child = createXMLNode();
         initXMLNodeFromXMLTag(child, tag);
         addXMLNodeToParent(current, child);
         current = child;
      }
      else if(tag->type == UNIQUE) {
         child = createXMLNode();
         initXMLNodeFromXMLTag(child, tag);
         addXMLNodeToParent(current, child);
      }
      /* Tag close current node */
      else if(tag->type == CLOSING) {
         if(current->parent != NULL) {
            current = current->parent;
         }
         else {
            endOfParsing = 1;
         }
      }

      destroyXMLTag(tag);
   }

   if(root != current) {
      logError("Last closed node isn't root node", __FILE__, __LINE__);
      destroyXMLNode(root);
      return NULL;
   }

   return root;
}


XML_File* loadXMLFile(const char* path){
   XML_File* xml;

   if((xml = createXMLFile()) != NULL){
      setXMLFilePath(path, xml);
      openXMLFile(xml);
      checkFirstLineXMLFile(xml);
      xml->root = parseXMLFile(xml->file);
   }

   return xml;
}


/**
 * \brief Reads a value in a XML file.
 *
 * \param[in] path  Values path in the XML file.
 *                  To find a node's value, use "root/foo/bar$"
 *                  To find an attribute, use "root/foo/bar:attribute"
 * \param[in] xml   Searched XML file.
 */
char* getXMLValue(char* path, XML_File* xml){
   char strBuffer[XML_BUFFER_LENGTH];
   char charBuffer;
   char* value;
   XML_Node* n;
   XML_Attribute* attr;
   int iPath, iBuf;

   value = NULL;
   n = xml->root;
   attr = NULL;
   iPath = 0;

   /* reads path */
   while(value == NULL){
      iBuf = 0;
      charBuffer = path[iPath];
      while((charBuffer != '/') &&
            (charBuffer != ':') &&
            (charBuffer != '$') &&
            (charBuffer != '\0')){
         strBuffer[iBuf] = charBuffer;
         iBuf++;
         iPath++;
         charBuffer = path[iPath];
      }
      strBuffer[iBuf] = '\0';
      iPath++;

      /* found end of path */
      if(charBuffer == '\0'){
         logError("Reached end of path without ':' or '$'.", __FILE__, __LINE__);
         return NULL;
      }

      /* find child with this name */
      while((n != NULL) && (strcmp(strBuffer, n->name) != 0)){
         n = n->next;
      }
      if(n == NULL){
         logError("Didn't find a child with this name", __FILE__, __LINE__);
         return NULL;
      }

      /* found node character '/', checks child */
      if(charBuffer == '/'){
         n = n->first;
      }
      /* found value character '$', reads value */
      else if(charBuffer == '$'){
         value = n->value;
      }
      /* found attribute character ':', reads attribute */
      else if(charBuffer == ':'){

         /* read attribute's name */
         iBuf = 0;
         while(path[iPath] != '\0'){
            strBuffer[iBuf] = path[iPath];
            iBuf++;
            iPath++;
         }
         strBuffer[iBuf] = '\0';

         /* searches attribute */
         attr = n->attr;
         while((attr != NULL) && (strcmp(strBuffer, attr->name) != 0)){
            attr = attr->next;
         }
         if(attr == NULL){
            logError("Didn't find an attribute with this name", __FILE__, __LINE__);
            return NULL;
         }
         else{
            value = attr->value;
         }
      }
   }

   return value;
}
