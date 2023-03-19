/**
 * file: main.c
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "ini.h"



bool fileExists(char * filename)
{
    return ( access( filename, F_OK ) != -1 );
}

bool isINIFilename(char * filename)
{
    if (NULL == filename)
    {
        return false;
    }

    int len = strlen(filename);
    if (len < 4)
    {
        return false;
    }

    char * allowExt = ".ini";

    return (strcmp(filename+(len-4), allowExt) == 0);
}

int main(int argc, char * argv[])
{
    char * filename = "datenlogger.ini";
    if (!fileExists(filename) || !isINIFilename(filename))
    {
        printf("Fehler: die Datei %s kann nicht gelesen werden.\n", filename);
        return EXIT_FAILURE;
    }

    SLIST_HEADER * head = ini_parse_file(filename);
    if (NULL == head) // Es gab einen Fehler beim Lesen der Datei bzw. beim Erstellen der Struktur
    {
        printf("Fehler beim Einlesen der Datei %s!\n", filename);
        return EXIT_FAILURE;
    }

    printf("START PART Cases: Without Default values\n");

    printf("(1) ***************************************\n");
    long value_l = 0;
    read_ini_long(head, "misc", "MaxRandomPitch", &value_l);
    printf("read_ini_long = %lu\n", value_l);

    printf("(2) ***************************************\n");
    float value_f = 0.0f;
    read_ini_float(head, "misc", "MaxRandomYaw", &value_f);
    printf("read_ini_float = %f\n", value_f);

    printf("(3) ***************************************\n");
    bool value_b = false;
    read_ini_bool(head, "features", "triggerbot", &value_b);
    printf("read_ini_bool = %s\n", value_b ? "true" : "false");

    printf("END PART Cases: Without Default values\n\n\n");

    printf("START PART Cases: WITH Default values\n");

    printf("(1) ***************************************\n");
    long value_ld = 0;
    long value_l_default = 123;
    read_ini_long_default(head, "misc", "MaxRandomPitch", &value_ld, &value_l_default);
    printf("read_ini_long_default = %lu (key exists case)\n", value_ld);
    read_ini_long_default(head, "misc", "MaxRandomPitch1", &value_ld, &value_l_default);
    printf("read_ini_long_default = %lu (key does NOT exist case)\n", value_ld);

    printf("(2) ***************************************\n");
    float value_fd = 0.0f;
    float value_f_default = 2.5f;
    read_ini_float_default(head, "misc", "MaxRandomYaw", &value_fd, &value_f_default);
    printf("read_ini_float_default = %f (key exists case)\n", value_fd);
    read_ini_float_default(head, "misc", "MaxRandomYaw1", &value_fd, &value_f_default);
    printf("read_ini_float_default = %f (key does NOT exist case)\n", value_fd);

    printf("(3) ***************************************\n");
    bool value_bd = false;
    bool value_b_default = false;
    read_ini_bool_default(head, "features", "triggerbot", &value_bd, &value_b_default);
    printf("read_ini_bool_default = %s (key exists case)\n", value_bd ? "true" : "false");
    read_ini_bool_default(head, "features", "triggerbot1", &value_bd, &value_b_default);
    printf("read_ini_bool_default = %s (key does NOT exist case)\n", value_bd ? "true" : "false");

    printf("END PART Cases: WITH Default values\n");

    // ---- ENDE Testablauf ---
    deleteList(head);

    return EXIT_SUCCESS;
}


