//
// Created by matt on 01/10/17.
//

#include "mysql.h"
#include <stdio.h>
#include <string.h>


BOOL database_connect(MYSQL *mysql, char *username, char *password, char *database, char *host, char *port){
    mysql_init(mysql);

    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "test_client");

    if (!mysql_real_connect(mysql, host, username, password, database, 0, NULL, 0)) {
        fprintf(stderr, "Failed to connect to database, error: %s\n", mysql_error(mysql));
        return FALSE;
    }

    return TRUE;
}

struct linked_list* database_query(MYSQL *mysql, char *statement) {
    struct database_row *db_row;
    MYSQL_RES* result;
    struct linked_list *list;
    my_ulonglong number_of_rows;
    unsigned int number_of_fields;
    int i;

    if (mysql_real_query(mysql, statement, strlen(statement))){
        fprintf(stderr, "Something went wrong\n");
    }

    result = mysql_store_result(mysql);

    if (!&result){
        fprintf(stderr, "Something else went wrong\n");
    }

    number_of_rows = mysql_num_rows(result);
    number_of_fields = mysql_num_fields(result);


    for (i = 0; i < (int)number_of_rows; i++) {
        db_row = malloc(sizeof(struct database_row));
        db_row->length = number_of_fields;
        db_row->data = mysql_fetch_row(result);

        if (i == 0) {
            list = linked_list_init(db_row);
        } else {
            linked_list_append(list, db_row);
        }
    }

    return list;
}

char* database_escape(MYSQL* mysql, char* string){
    char *output = malloc((strlen(string) * 2) + 1);
    mysql_real_escape_string(mysql, output, string, strlen(string));
    return output;
}