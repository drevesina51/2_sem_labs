#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

int dialog(const char *msgs[], int N) {
        char *errmsg = "";
        int rc;
        int i, n;
        do{
                puts(errmsg);
                errmsg = "\nYou are wrong. Repeate, please!";
                for(i = 0; i < N; ++i)
                        puts(msgs[i]);
                puts("Make your choice: --> ");
                n = scanf("%d", &rc);
		if (n == EOF) return 0;
                if(n == 0)
                        rc = 0;
        } while(rc < 0 || rc >= N);
        return rc;
}

int main (){
        Table *table = (Table *) calloc(1, sizeof(Table)); // создание пустой таблицы
        if(table == NULL) {
                   printf("Error try alloc\n");
                   return 0;
        }
        init_table(table);
        const char *msgs[] = {"0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show", "5. Delete_All",
                              "6. Find_By_Any_Key", "7. Delete_Keyspace1", "8. Find_Keyspace1"};
        const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
        int dialog(const char *msgs[], int);
        int (*fptr[])(Table *) = {NULL, D_Add, D_Find, D_Delete, D_Show, D_Delete_All,
                                 D_Find_By_Any_Key, D_Delete_Keyspace1, D_Find_Keyspace1};
        int rc;

        while(rc = dialog(msgs, NMsgs))
                if(!fptr[rc](table)) // вызов выбранной диалоговой функции
                  break;
        printf("End.\n");
        delTable(table); //удаление таблицы
        return 0;
}
