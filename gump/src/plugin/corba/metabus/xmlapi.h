#ifndef _PARSEXML_H
#define _PARSEXML_H

#include <libxml/parser.h>
#include <libxml/tree.h>

extern char* utf2gb (const unsigned char *in, const char *encoding);
extern xmlNodePtr xml_get_node(xmlNodePtr aCur, const char* aName);

extern void xml_get_text(xmlDocPtr aDoc, xmlNodePtr aCur, 
						char* aBuf, int aLen, const char* aEncoding=NULL);
extern void xml_get_text(xmlDocPtr aDoc, xmlNodePtr aCur, int *aOut);
extern void xml_get_text(xmlDocPtr aDoc, xmlNodePtr aCur, short *aOut);

extern void xml_get_prop(xmlNodePtr aCur, const char* aName, 
						char* aBuf, int aLen, const char* aEncoding=NULL);
extern void xml_get_prop(xmlNodePtr aCur, const char* aName, int *aOut);
extern void xml_get_prop(xmlNodePtr aCur, const char* aName, short *aOut);

#endif
