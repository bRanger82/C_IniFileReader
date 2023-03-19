/**
 * file: ini.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "ini.h"


/*
    Erstellt ein INI_SECTION Struktur Element
    Die Parameter werden fuer das Befuellen der Struktur verwendet
    Rueckgabewert:
        Zeiger auf das neue INI_SECTION Element wenn erfolgreich, sonst NULL
*/
INI_SECTION * createINISection(char * section, SLIST_HEADER * ptuple)
{
    INI_SECTION * iniSection = (INI_SECTION *)malloc(sizeof(INI_SECTION));
    if (NULL == iniSection)
    {
        return NULL;
    }
    char * tmp = ( char *)malloc(sizeof(char) * (strlen(section) - 1));
    if (NULL == tmp)
    {
        return NULL;
    }
    // Die eckigen Klammern des Sektionsnamens entfernen
    memcpy(tmp, section+1, strlen(section)-2);
    tmp[strlen(section)-2] = '\0';

    iniSection->section = tmp;
    iniSection->ptuple = ptuple;

    return iniSection;
}

/*
    Diese Funktion erstellt eine splittet einen String in einen Key und einen Wert auf
    und gibt diesen dann als INI_TUPLE Struktur zurueck
    Paraemter:
        tulpe_line: INI Element, z.B. Key=Wert
    Rueckgabewert:
        Zeiger auf das neue INI_TUPLE Element wenn erfolgreich, sonst NULL
*/
INI_TUPLE * createKeyValuePair(char * tulpe_line)
{
    char *wert = NULL;
    char *schluessel = NULL;

    // initialisieren und ersten Abschnitt erstellen
    schluessel = strtok(tulpe_line, delim); //Schluessel
    if (NULL == schluessel)
    {
        return NULL;
    }

    wert = strtok(NULL, delim); //Wert
    if (NULL == wert)
    {
        return NULL;
    }

    INI_TUPLE * newKVPair = (INI_TUPLE *)malloc(sizeof(INI_TUPLE));
    if (NULL == newKVPair)
    {
        return NULL;
    }
    char * cKey = (char *)malloc(sizeof(char) * (strlen(schluessel) + 1));
    char * cVal = (char *)malloc(sizeof(char) * (strlen(wert) + 1));
    if (NULL == cKey || NULL == cVal)
    {
        return NULL;
    }

    memcpy(cKey, schluessel, strlen(schluessel) + 1);
    memcpy(cVal, wert, strlen(wert) + 1);

    newKVPair->key = Trim(cKey);
    newKVPair->value = Trim(cVal);

    #ifdef DEBUG
    printf("New Key-Value Pair, Key: '%s', Value: '%s'\n", newKVPair->key, newKVPair->value);
    #endif // DEBUG

    return newKVPair;
}

/*
 Entfernt die voran- und hintenangestellte Leerzeichen aus einem String
 Parameter:
  - str: Der String mit Leerzeichen vorne und hinten angestellt
 Rueckgabewert:
    str ohne Leerzeichen
*/
char *Trim(char *str)
{
  char *end;

  // Loesche Leerzeichen vorne
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // Nur Leerzeichen?
    return str;

  // Loesche Leerzeichen hinten
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Null-String setzten
  *(end+1) = 0;

  return str;
}


SLIST_HEADER * ini_parse_file(const char * iniFile)
{
    SLIST_HEADER * sListHead = createSList();

    if (NULL == sListHead)
    {
        printf("ERROR: List object could not be created!\n");
        return NULL;
    }

    FILE *fp;

    fp = fopen(iniFile, "r");

    if (NULL == fp)
    {
        printf("ERROR: File %s could not be read!\n", iniFile);
        return NULL;
    }

    char LineBuffer[MAX_INI_LINE_LEN];

     // List of Key/Value pairs
    SLIST_HEADER * ini_Tuples = NULL;

    // A INI section item
    INI_SECTION * ini_Section = NULL;

    while(fgets(LineBuffer, MAX_INI_LINE_LEN, fp) != NULL)
    {
        char * buffer = Trim(LineBuffer);

        // buffer contains only NULL (just in case handling)
        if (NULL == buffer)
        {
            continue;
        }

        // buffer contains only an empty line or coment
        if (buffer[0] == '\n' || buffer[0] == ';')
        {
            continue;
        }

        int posEnd = strlen(buffer);


        if (buffer[0] == '[' && buffer[posEnd-1] == ']') // Line is a section element
        {
            ini_Tuples = createSList();
            if (NULL == ini_Tuples)
            {
                printf("ERROR: Tuple List cannot be created!\n");
                return NULL;
            }
            ini_Section = createINISection(buffer, ini_Tuples);
            if (NULL == ini_Section)
            {
                return NULL;
            }
            insertLast(sListHead, ini_Section);
            continue;
        }

        if (strstr(buffer, delim) != 0) // Line is a key-value pair (it contains a '=' character)
        {
            INI_TUPLE * ini_KeyValuePair = createKeyValuePair(buffer);
            if (ini_Tuples != NULL)
            {
                insertLast(ini_Tuples, ini_KeyValuePair);
            }
            continue;
        }
    }

    fclose(fp);

    return sListHead;
}

// Suche nach einer Tuple in einer Liste von Elementen
INI_TUPLE * findINITupleKey(SLIST * item, char * key)
{
    if (NULL == item)
    {
        return NULL; // Es gibt kein Element, welches zur Suche passt
    }

    INI_TUPLE * iniTup = (INI_TUPLE *)item->Data;
    if (strcmp(iniTup->key, key) == 0)
    {
        return iniTup; // Element gefunden
    } else
    {
        return findINITupleKey(item->Next, key); // Wert nicht gefunden, suche in naechstem Element
    }

}

// Suche nach einer Sektion in einer Liste von Elementen
INI_SECTION * findINISection(SLIST * item, char * section)
{
    if (NULL == item)
    {
        return NULL; // Es gibt kein Element, welches zur Suche passt
    }

    INI_SECTION * retVal = (INI_SECTION *)item->Data;
    if (NULL == retVal)
    {
        return NULL;
    }

    if (strcmp(retVal->section, section) == 0)
    {
        return retVal; // Element gefunden
    } else
    {
        return findINISection(item->Next, section); // Wert nicht gefunden, suche in naechstem Element
    }

}

/* Durchsucht eine INI-Struktur nach einer Sektion und Schluessl und gibt den zugehoerigen Wert zurueck
   Wenn nichts gefunden wurde wird NULL zurueck gegeben
*/
char * getValueBasedOnKey(SLIST_HEADER * root, char * section, char * key)
{
    // root ist NULL oder leer
    if (NULL == root)
    {
        return NULL;
    }
    if (root->Len == 0)
    {
        return NULL;
    }

    // Suche zuerst in Sektion
    INI_SECTION * iniSec = findINISection(root->First, section);
    if (NULL == iniSec)
    {
        return NULL; // die Sektion gibt es nicht in der Datei
    }

    // Suche der gefundenen Sektion nach dem Key
    INI_TUPLE * iniTup = findINITupleKey(iniSec->ptuple->First, key);

    if (NULL == iniTup)
    {
        return NULL; // Key nicht gefunden
    }

    //Es wurde ein Wert gefunden
    return iniTup->value;
}

bool read_ini_long(SLIST_HEADER * SListHead, char * Section, char * Key, long * Value)
{
    char * tulpeValue = getValueBasedOnKey(SListHead, Section, Key);
    if (NULL == tulpeValue)
    {
        #ifdef DEBUG
        printf("ERROR: read_ini_long.tulpeValue returned NULL\n");
        #endif
        return false;
    }

    *Value = atol(tulpeValue);

    return true;
}

bool read_ini_float(SLIST_HEADER * SListHead, char * Section, char * Key, float * Value)
{
    char * tulpeValue = getValueBasedOnKey(SListHead, Section, Key);
    if (NULL == tulpeValue)
    {
        #ifdef DEBUG
        printf("ERROR: read_ini_float.tulpeValue returned NULL\n");
        #endif
        return false;
    }

    *Value = atof(tulpeValue);

    return true;
}

void read_ini_long_default(SLIST_HEADER * SListHead, char * Section, char * Key, long * Value, long * DefaultValue)
{
    if (!read_ini_long(SListHead, Section, Key, Value))
    {
        *Value = *DefaultValue;
    }
}

void read_ini_float_default(SLIST_HEADER * SListHead, char * Section, char * Key, float * Value, float * DefaultValue)
{
    if (!read_ini_float(SListHead, Section, Key, Value))
    {
        *Value = *DefaultValue;
    }
}

bool read_ini_bool(SLIST_HEADER * SListHead, char * Section, char * Key, bool * Value)
{
    char * tulpeValue = getValueBasedOnKey(SListHead, Section, Key);
    if (NULL == tulpeValue)
    {
        #ifdef DEBUG
        printf("ERROR: read_ini_bool.tulpeValue returned NULL\n");
        #endif
        return false;
    }

    if (tulpeValue[0] == '1')
    {
        *Value = true;
    } else
    {
        *Value = false;
    }

    return true;
}

void read_ini_bool_default(SLIST_HEADER * SListHead, char * Section, char * Key, bool * Value, bool * DefaultValue)
{
    if (!read_ini_bool(SListHead, Section, Key, Value))
    {
        *Value = *DefaultValue;
    }
}
