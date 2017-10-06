#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "main.h"
#include "mysql.h"
#include "linked_list.h"

int main(int argc, char *argv[]) {
    int i;
    char filename[255];
    char table_name[255];
    char sql_statement[1023];
    char mysql_username[127];
    char mysql_password[127];
    char mysql_host[127];
    char mysql_port[9];
    char mysql_database[127];
    BOOL verbose = FALSE;
    MYSQL mysql;
    struct linked_list *row;
    FILE* f;

    memset(sql_statement, '\0', sizeof(sql_statement));
    memset(table_name, '\0', sizeof(table_name));
    memset(mysql_password, '\0', sizeof(mysql_password));
    memset(mysql_username, '\0', sizeof(mysql_username));
    memset(mysql_host, '\0', sizeof(mysql_host));
    memset(mysql_port, '\0', sizeof(mysql_port));
    memset(mysql_database, '\0', sizeof(mysql_database));
    memset(filename, '\0', sizeof(filename));

    for(i = 0; i < argc; i++){
        if (strlen(argv[i]) == 2 && strcmp(&argv[i][0], "-")){
            switch(argv[i][1]){
                case 't':
                    if (i + 1 < argc && strlen(argv[i+1]) <= 256){
                        strcpy(table_name, argv[i+1]);
                        i++;
                    } else {
                        fprintf(stderr, "Unknown table name\n");
                        return 1;
                    }
                    break;
                case 's':
                    if (i + 1 < argc && strlen(argv[i+1]) <= 1024){
                        strcpy(sql_statement, argv[i+1]);
                        i++;
                    } else {
                        fprintf(stderr, "SQL statement required\n");
                        return 1;
                    }
                    break;
                case 'u':
                    if (i + 1 < argc && strlen(argv[i+1]) <= 128){
                        strcpy(mysql_username, argv[i+1]);
                        i++;
                    } else {
                        fprintf(stderr, "MySQL username required\n");
                        return 1;
                    }
                    break;
                case 'p':
                    if (i + 1 < argc && strlen(argv[i+1]) <= 128){
                        strcpy(mysql_password, argv[i+1]);
                        i++;
                    } else {
                        fprintf(stderr, "MySQL password required\n");
                        return 1;
                    }
                    break;
                case 'h':
                    if (i + 1 < argc && strlen(argv[i+1]) <= 128){
                        strcpy(mysql_host, argv[i+1]);
                        i++;
                    } else {
                        fprintf(stderr, "MySQL host required\n");
                        return 1;
                    }
                    break;
                case 'd':
                    if (i + 1 < argc && strlen(argv[i+1]) <= 128){
                        strcpy(mysql_database, argv[i+1]);
                        i++;
                    } else {
                        fprintf(stderr, "MySQL database required\n");
                        return 1;
                    }
                    break;
                case 'P':
                    if (i + 1 < argc && strlen(argv[i+1]) <= 10){
                        strcpy(mysql_port, argv[i+1]);
                        i++;
                    } else {
                        fprintf(stderr, "MySQL port required\n");
                        return 1;
                    }
                    break;
                case 'v':
                    verbose = TRUE;
                    break;
                default:
                    fprintf(stderr, "Unknown option '%s'\n", &argv[i][1]);
                    return 1;
            }
        }else{
            strcpy(filename, argv[i]);
        }
    }

    if (strlen(table_name) > 0 && strlen(sql_statement) > 0){
        fprintf(stderr, "Please supply either a table name or a SQL statement, not both.\n");
        return 1;
    }

    if (strlen(table_name) == 0 && strlen(sql_statement) == 0){
        fprintf(stderr, "Please supply either a table name or a SQL statement.\n");
        return 1;
    }

    if (strlen(table_name) > 0){
        strcpy(sql_statement, "SELECT * FROM `");
        strcat(sql_statement, table_name);
        strcat(sql_statement, "`;");
    }

    if (verbose == TRUE) {
        fprintf(stdout, "Outputting to: %s\nConnecting to %s with user %s\n", filename, mysql_host, mysql_username);
    }

    if (database_connect(&mysql, mysql_username, mysql_password, mysql_database, mysql_host, mysql_port) == FALSE) {
        fprintf(stderr, "Failed to connect to the database, please check the details and try again.\n");
        return 1;
    }

    if (verbose == TRUE) {
        fprintf(stdout, "Executing SQL: %s\n", sql_statement);
    }

    row = database_query(&mysql, sql_statement);

    f = fopen(filename, "w");
    row->meta->current = row;

    while (row != NULL) {
        struct database_row *db_row = (struct database_row*)row->item;

        for(i = 0; i < db_row->length; i++) {
            if (i > 0) {
                fprintf(f, ",");
            }
            char *string = db_row->data[i];
            if (string != NULL) {
                fprintf(f, "\"%s\"", string);
            }

        }
        row = row->next;
        fprintf(f, "\n");
    }

    fprintf(f, "\n");

    fclose(f);
    if (verbose == TRUE) {
        printf("Exported\n");
    }
    return 0;
}