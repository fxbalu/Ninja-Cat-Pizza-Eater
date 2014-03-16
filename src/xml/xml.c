/**
 * \file xml.c
 * \brief XML file related functions.
 *
 * Definitions of functions to read and parse a XML file.
 *
 * \author François-Xavier Balu \<fx.balu@gmail.com\>
 * \date 13 mars 2014
 */


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
         /* logMem(FREE, "string", __FILE__, __LINE__); */
         free(xml->path);
      }
      /* close file */
      if(xml->file != NULL) {
         /* logMem(FREE, "file", __FILE__, __LINE__); */
         fclose(xml->file);
      }
      /* destroy tree */
      if(xml->root != NULL) {
         destroyXMLNode(xml->root);
      }
      /* free XML_File */
      /* logMem(FREE, "XML_File", __FILE__, __LINE__); */
      free(n);
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
   if(xml != NULL) {
      logError("Giving a file path to a NULL XML_File", __FILE__, __LINE__);
   }
   /* NULL file path */
   else if(path != NULL) {
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
      if((node->n = malloc((strlen(path) + 1) * sizeof(char))) == NULL) {
         logError("can't allocate memory for file path", __FILE__, __LINE__);
      }
      else {
         /* logMem(ALLOC, "string", __FILE__, __LINE__) */
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
      /* logMem(ALLOC, "file", __FILE__, __LINE__) */
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
      /* logMem(FREE, "file", __FILE__, __LINE__) */
      fclose(xml->file);
   }
}


Boolean checkFirstLineXMLFile(XML_File* xml)
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
   else if{
      return (strcmp(firstLine, XML_FIRST_LINE) == 0);
   }

   return Boolean.false;
}


void parseXMLFile(XML_File* xml);

XML_Node* loadDataFromXMLFile(const char* filePath);
