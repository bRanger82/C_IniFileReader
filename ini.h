/**
 * file: ini.h
 *
 * Purpose: prototypes for reading an INI file
 */

#include <stdbool.h>
#include "List.h"

#define MAX_INI_LINE_LEN 2048  // Maximum number of chars for each line of the INI file
#define delim "="              // Delimiter char for Key and Value pair

// Structure for the KeyValuePair
typedef struct ini_tuple
{
    char * key;
    char * value;
} INI_TUPLE;

// Structure for a INI section
typedef struct ini_section
{
    char * section;         // Section [IN]
    SLIST_HEADER * ptuple;  // List of all KeyValuePairs
} INI_SECTION;


// Erstellt eine INI_SECTION Struktur mit den Parameter-Werten
INI_SECTION * createINISection(char *, SLIST_HEADER *);

// Erstellt eine INI_TUPLE Struktur anhand des String, das uebergeben wird
INI_TUPLE * createKeyValuePair(char *);

//Loescht Leerzeichen, welche sich vor und nach einem String
char *Trim(char *);

// Liest eine INI Datei ein und erstellt eine Struktur aus dieser
SLIST_HEADER * ini_parse_file(const char *);

// Sucht in einer Liste von INI_TUPLE einen Wert zu einem Key
INI_TUPLE * findINITupleKey(SLIST * item, char *);

// Sucht in einer Liste von INI_SECTION nach einer Sektion
INI_SECTION * findINISection(SLIST *, char *);

// Durchsucht eine eingelesene INI Datei nach einem Key und gibt den zugehoerigen Wert zurueck
char * getValueBasedOnKey(SLIST_HEADER *, char *, char *);

bool read_ini_long(SLIST_HEADER *, char *, char *, long *);
bool read_ini_float(SLIST_HEADER *, char *, char *, float *);
void read_ini_long_default(SLIST_HEADER *, char *, char *, long *, long *);
void read_ini_float_default(SLIST_HEADER *, char *, char *, float *, float *);
bool read_ini_bool(SLIST_HEADER *, char *, char *, bool *);
void read_ini_bool_default(SLIST_HEADER *, char *, char *, bool *, bool *);
