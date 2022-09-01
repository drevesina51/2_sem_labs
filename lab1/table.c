#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
    /* массив диагностических сообщений об ошибках */
const char *errmsgs[] = {"Ok", "Duplicate key", "Table overflow", "Input Error"};

int init_table(Table *table){
        table->msize1 = 15;
        table->msize2 = 15;
        table->csize1 = 0;
        table->csize2 = 0;
        for (int q = 0; q < table->msize1; q++){
                table->ks1[q] = (KeySpace1 *)calloc(1, sizeof(KeySpace1));
    	        if(table->ks1[q] == NULL) {
                	    printf("Error try alloc\n");
                	    return 0;
         	}
        }
        for (int w = 0; w < table->msize2; w++){
                table->ks2[w] = (KeySpace2 *)calloc(1, sizeof(KeySpace2));
                if(table->ks2[w] == NULL) {
                            printf("Error try alloc\n");
                            return 0;
                }
                table->ks2[w]->busy = 0;
		table->ks2[w]->busy2 = 0;
        }
        return 0;
}

int find(Table* table, int k1, char *k2){
        /* проверка первого пространства */
        for(int q = 0; q < table->csize1; q++)
                if (table->ks1[q]->key == k1)
                        return 1;
        /* проверка второго пространства */
	int k = ((int)k2[0]+4)%table->msize2;
        while(table->ks2[k]->busy2){
                if (table->ks2[k]->busy == 1 && strcmp(table->ks2[k]->key, k2) == 0)
                        return 1;
		k++;
		if (k == table->msize2) k = 0;
	}
        return 0; // не нашлось такого ключа
}

int delTable(Table *table){
	for(int i = 0; i < table->msize1; i++){
//		if (i < table->csize1){
//			free(table->ks1[i]->info->info->string);
//			free(table->ks1[i]->info->info);
//			free(table->ks1[i]->info);
//		}
//		free(table->ks1[i]);
	}
        for(int j = 0; j < table->msize2; j++){
//		if (table->ks2[j]->busy2 == 1)
//                	free(table->ks2[j]->key);
//		free(table->ks2[j]);
        }
	free(table);
	return 1;
}

int insert(Table *table, int k1, char *k2, int par, int info1, int info2, char *info3){
        int i, flag = 0;
        i = find(table, k1, k2);
        if (i == 1)
                return 1; // "Duplicate key"
        else if (table->csize1 == table->msize1 || table->csize2 == table->msize2)
                return 2; // "Table overflow"
             else {
                        /* InfoType */
                        InfoType *ptr = (InfoType *)calloc(1, sizeof(InfoType));
               		if(ptr == NULL) {
                        	 printf("Error try alloc\n");
                           	 return 3;
               		}
                        ptr->num1 = info1;
                        ptr->num2 = info2;
                        ptr->string = info3;
                        /* изменение первого пространства */
                        int r = table->csize1;
                        /* Item */
                        Item *tmp = (Item *)calloc(1, sizeof(Item));
                        if(tmp == NULL) {
                                 printf("Error try alloc\n");
                                 return 3;
                        }
                        tmp->key1 = k1;
                        tmp->key2 = k2;
                        tmp->info = ptr;
                        /* keySpace1 */
                        table->ks1[r]->key = k1;
			for(int q = 0; q < table->csize1; q++)
                		if (table->ks1[q]->key == par){
                        		flag = 1;//есть такой ключ
					break;
				}
			if (flag == 1) table->ks1[r]->par = par;
			else table->ks1[r]->par = 0;
                        table->ks1[r]->info = tmp;
                        /* изменение второго пространства */
                        /* перемешивание сложением */
                        int j;
                        j = ((int)(k2[0]) + 4) % table->msize2;
			while (table->ks2[j]->busy == 1) // поиск места
				if (j < table->msize2) j++;
				else j = 0;// переход в начало
                        table->ks2[j]->busy = 1;
			table->ks2[j]->busy2 = 1;
                        table->ks2[j]->key = k2;
                        table->ks2[j]->info = ptr;
                        /* изменение параметров количества */
                        table->csize1 ++;
                        table->csize2 ++;
             }
        return 0; // "Ok"
}

int D_Add(Table *table){
        int k1, rc, n, n1, n2, par, info1, info2;
        char *k2;
        char *info3;
        k2 = (char *)calloc(15, sizeof(char));
        if(k2 == NULL) {
              printf("Error try alloc\n");
              return 0;
        }
        info3 = (char *)calloc(15, sizeof(char));
        if(info3 == NULL) {
              printf("Error try alloc\n");
              return 0;
        }
        printf("Enter key 1: -->\n");
        n = scanf("%d", &k1);
	if (n == EOF) return 0;
	printf("Enter the key of the parent element -->\n");
	n2 = scanf("%d", &par);
	if (n2 == EOF) return 0;
        printf("Enter key 2: -->\n");
        n1 = scanf("%s", k2);
	if (n1 == EOF) return 0;
        if(n == 0 || n1 == 0 || n2 == 0) {
                rc = 3;
                printf("%s\n", errmsgs[rc]);
                return 0; //ошибка ввода
        }
        printf("Entering info:\n");
        printf("Enter 1st number:\n");
        n = scanf("%d", &info1);
	if (n == EOF) return 0;
        printf("Enter 2nd number:\n");
        n1 = scanf("%d", &info2);
	if (n1 == EOF) return 0;
        printf("Enter string:\n");
        n2 = scanf("%s", info3);
	if (n2 == EOF) return 0;
        if (n == 0 || n1 == 0 || n2 == 0) {
                rc = 3;
                printf("%s\n", errmsgs[rc]);
                return 0;
        }
        rc = insert(table, k1, k2, par, info1, info2, info3);
        printf("%s: %d %s\n", errmsgs[rc], k1, k2);
        return 1;
}

int D_Find(Table *table){
	int n, k1, i = -1;
	char *k2;
	k2 = (char *)calloc(15, sizeof(char));
        if(k2 == NULL) {
              printf("Error try alloc\n");
              return 0;
        }
	printf("Enter key 1: -->\n");
        n = scanf("%d", &k1);
	if (n == EOF) return 0;
        printf("Enter key 2: -->\n");
        n = scanf("%s", k2);
	if (n == EOF) return 0;
        /* проверка первого пространства */
        for(int q = 0; q < table->csize1; q++)
                if (table->ks1[q]->key == k1 && strcmp(table->ks1[q]->info->key2, k2) == 0){
			i = q;
			break;
		}
	free(k2);
	if(i != -1){
		printf("|    Key 1    |  Parent key  |    Key 2    |  1st Number  |  2nd Number  |    String   |\n");
		printf("|%13d|%14d|%13s|%14d|%14d|%13s|\n", table->ks1[i]->info->key1, table->ks1[i]->par, table->ks1[i]->info->key2,
                        table->ks1[i]->info->info->num1, table->ks1[i]->info->info->num2, table->ks1[i]->info->info->string);
		return 1;
	}
        else printf("No such key was found\n");
	return 2; // не нашлось такого ключа
}

int D_Delete(Table *table){
        int k1, n, i = 0;
        char *k2;
        k2 = (char *)calloc(15, sizeof(char));
        if(k2 == NULL) {
              printf("Error try alloc\n");
              return 0;
        }
        printf("Enter key 1: -->\n");
        n = scanf("%d", &k1);
	if (n == EOF) return 0;
        printf("Enter key 2: -->\n");
        n = scanf("%s", k2);
	if (n == EOF) return 0;
        /* проверка первого пространства */
        for(int q = 0; q < table->csize1; q++)
                if (table->ks1[q]->key == k1){
                        i++;
			n = q;
                        break;
                }
	//проверка второго пространства
	int k = ((int)k2[0]+4)%table->msize2;
        while(table->ks2[k]->busy2){
        	if (strcmp(table->ks2[k]->key, k2) == 0){
			i++;
               	     	break;
		}
               	k++;
               	if (k == table->msize2) k = 0;
        }
	//если i = 2, то такой ключ есть
        if(i != 2){
		printf("No such key was found\n");
		free(k2);
        	return 2; // не нашлось такого ключа
	} else {
			table->ks2[k]->busy = 0;
			while(n < table->csize1 - 1){
				table->ks1[n] = table->ks1[n+1];
				n++;
			}
//                        free(table->ks1[n]->info->info->string);
//                        free(table->ks1[n]->info->info);
//                        free(table->ks1[n]->info);
			table->csize1--;
                	table->csize2--;
                	free(k2);
                	return 1;
		}
}

int D_Show(Table *table){
        printf("|    Key 1    |  Parent key  |    Key 2    |  1st Number  |  2nd Number  |    String   |\n");
        for (int i = 0; i < table->csize1; i++){
                printf("|%13d|%14d|%13s|%14d|%14d|%13s|\n", table->ks1[i]->info->key1, table->ks1[i]->par,  table->ks1[i]->info->key2,
                        table->ks1[i]->info->info->num1, table->ks1[i]->info->info->num2, table->ks1[i]->info->info->string);
        }
	return 1;
}

int D_Delete_All(Table *table){
        int k1, z, n, i = -1;
        char *k2;
        k2 = (char *)calloc(15, sizeof(char));
        if(k2 == NULL) {
              printf("Error try alloc\n");
              return 0;
        }
	printf("Select a search key: 1/2\n");
	n = scanf("%d", &z);
	if (n == EOF) return 0;
	if (z == 1){//по первому ключу
       		printf("Enter key 1: -->\n");
       	 	n = scanf("%d", &k1);
		if (n == EOF) return 0;
       		for(int q = 0; q < table->csize1; q++)
               	 	if (table->ks1[q]->key == k1){
                	       i = q;
                 	       break;
             		   }
   	        if(i != -1){
			k2 = table->ks1[i]->info->key2;
                	int k = ((int)k2[0]+4)%table->msize2;
               		while(table->ks2[k]->busy2){
                       		if (strcmp(table->ks2[k]->key, k2) == 0){
                               	 	i = k;
                               		break;
                        	}
                        	k++;
                        	if (k == table->msize2) k = 0;
                	}
			table->ks2[k]->busy = 0;
       			while(i < table->csize1 - 1){
       		       		table->ks1[i] = table->ks1[i+1];
               			i++;
                	}
//                        free(table->ks1[i]->info->info->string);
//                        free(table->ks1[i]->info->info);
//                        free(table->ks1[i]->info);
                	table->csize1--;
                	table->csize2--;
			free(k2);
              		return 1;
        	} else printf("No such key was found\n");
	} else{//по второму ключу
		printf("Enter key 2: -->\n");
        	n = scanf("%s", k2);
		if (n == EOF) return 0;
        	int k = ((int)k2[0]+4)%table->msize2;
                while(table->ks2[k]->busy2){
                        if (strcmp(table->ks2[k]->key, k2) == 0){
				i = k;
                                break;
			}
                        k++;
                        if (k == table->msize2) k = 0;
                }
                if(i != -1){
                        table->ks2[i]->busy = 0;
                	for(int q = 0; q < table->csize1; q++)
                       		if (table->ks1[q]->info->info == table->ks2[i]->info){
                               		i = q;
                               		break;
                           	}
                        while(i < table->csize1 - 1){
                                 table->ks1[i] = table->ks1[i+1];
                                 i++;
                        }
//                        free(table->ks1[i]->info->info->string);
//                        free(table->ks1[i]->info->info);
//                        free(table->ks1[i]->info);
                        table->csize1--;
                        table->csize2--;
			free(k2);
                        return 1;
                } else printf("No such key was found\n");
	}
	free(k2);
        return 2; // не нашлось такого ключа
}

int D_Find_By_Any_Key(Table *table){
        int k1, n, z, i = -1;
        char *k2;
        k2 = (char *)calloc(15, sizeof(char));
        if(k2 == NULL) {
              printf("Error try alloc\n");
              return 0;
        }
        printf("Select a search key: 1/2\n");
        n = scanf("%d", &z);
	if (n == EOF) return 0;
        if (z == 1){//по первому ключу
        	printf("Enter key 1: -->\n");
        	n = scanf("%d", &k1);
		if (n == EOF) return 0;
		for(int q = 0; q < table->csize1; q++)
                	if (table->ks1[q]->key == k1){
                		i = q;
                		printf("|    Key 1    |  Parent key  |    Key 2    |  1st Number  |  2nd Number  |    String   |\n");
                		printf("|%13d|%14d|%13s|%14d|%14d|%13s|\n", table->ks1[i]->info->key1, table->ks1[i]->par, table->ks1[i]->info->key2,
                        	table->ks1[i]->info->info->num1, table->ks1[i]->info->info->num2, table->ks1[i]->info->info->string);
				free(k2);
				return 1;
                	}
	} else {//по второму ключу
        	printf("Enter key 2: -->\n");
     		n = scanf("%s", k2);
		if (n == EOF) return 0;
                int k = ((int)k2[0]+4)%table->msize2;
                while(table->ks2[k]->busy2){
                        if (strcmp(table->ks2[k]->key, k2) == 0){
                                i = k;
                                printf("|    Key 2    |  1st Number  |  2nd Number  |    String   |\n");
               			printf("|%13s|%14d|%14d|%13s|\n", table->ks2[k]->key,
                        	table->ks2[k]->info->num1, table->ks2[k]->info->num2, table->ks2[k]->info->string);
                		free(k2);
				return 1;
                        }
                        k++;
                        if (k == table->msize2) k = 0;
                  }
		}
        printf("No such key was found\n");
	free(k2);
        return 2; // не нашлось такого ключа
}

int D_Delete_Keyspace1(Table *table){
        int k1, n, i = -1;
        printf("Enter key 1: -->\n");
        n = scanf("%d", &k1);
	if (n == EOF) return 0;
        /* проверка первого пространства */
        for(int q = 0; q < table->csize1; q++)
                if (table->ks1[q]->key == k1){
                        i = q;
                        break;
                }
        if(i != -1){
		char *k2;
		k2 = table->ks1[i]->info->key2;
                int k = ((int)k2[0]+4)%table->msize2;
                while(table->ks2[k]->busy2){
                        if (strcmp(table->ks2[k]->key, k2) == 0){
                                i = k;
                                break;
                        }
                        k++;
                        if (k == table->msize2) k = 0;
                }
                table->ks2[k]->busy = 0;
                while(i < table->csize1 - 1){
                        table->ks1[i] = table->ks1[i+1];
                        i++;
                }
//                free(table->ks1[i]->info->info->string);
//                free(table->ks1[i]->info->info);
//                free(table->ks1[i]->info);
                table->csize1--;
                table->csize2--;
                return 1;
        }
        else printf("No such key was found\n");
        return 2; // не нашлось такого ключа
}

int D_Find_Keyspace1(Table *table){
        int a, b, n, i = -1;
        printf("Enter the range of the parent key: a <= parent Key <= b\na = ...\n");
        n = scanf("%d", &a);
	if (n == EOF) return 0;
	printf("b = ...\n");
	n = scanf("%d", &b);
	if (n == EOF) return 0;
        /* проверка первого пространства */
        for(int q = 0; q < table->csize1; q++)
                if (table->ks1[q]->par >= a && table->ks1[q]->par <= b){
       	        	 if(i == -1) printf("|    Key 1    |  Parent key  |    Key 2    |  1st Number  |  2nd Number  |    String   |\n");
       		         printf("|%13d|%14d|%13s|%14d|%14d|%13s|\n", table->ks1[q]->info->key1, table->ks1[q]->par, table->ks1[q]->info->key2,
                 	       	table->ks1[q]->info->info->num1, table->ks1[q]->info->info->num2, table->ks1[q]->info->info->string);
		i = 1;
                }
        if(i == -1)  printf("No such key was found\n");
        return 2; // не нашлось такого ключа
}
