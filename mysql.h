//
// Created by matt on 01/10/17.
//

#ifndef MYSQL2CSV_MYSQL_H
#define MYSQL2CSV_MYSQL_H

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef BOOL
#define BOOL char
#endif

#include <mysql/mysql.h>
#include "linked_list.h"

struct Result{
    unsigned long long number_of_rows;
    unsigned int number_of_fields;
    MYSQL_ROW row;
};

typedef struct database_row {
    // struct database_row *next;
    MYSQL_ROW data;
    unsigned long length;
};

BOOL database_connect(MYSQL *mysql, char *username, char *password, char *database, char *host, char *port);

struct linked_list* database_query(MYSQL *mysql, char *statement);
char* database_escape(MYSQL* mysql, char* string);

#endif //MYSQL2CSV_MYSQL_H
