#ifndef TABLE_H
#define TABLE_H

#define IndexType1 int
#define IndexType2 int
#define KeyType1 int
#define KeyType2 char
#define BusyType2 int

typedef struct InfoType{
    int num1; //первое число
    int num2; //второе число
    char *string; //строка
} InfoType;

typedef struct Item{
        KeyType1 key1;
        KeyType2 *key2;
//ind1, ind2
        InfoType *info; //указатель на информацию
} Item;

typedef struct KeySpace1{
    KeyType1 key;   // ключ элемента, не может быть 0
    KeyType1 par;   // ключ родительского элемента
    Item *info;	// указатель на информацию
} KeySpace1;

typedef struct KeySpace2{
	BusyType2 busy;     // признак занятости элемента
        BusyType2 busy2; // был ли заполнен
	KeyType2 *key;       // ключ элемента
	InfoType *info;	    // указатель на информацию
} KeySpace2;

typedef struct Table{
    KeySpace1 	*ks1[15];	// указатель на первое пространство ключей
    KeySpace2 	*ks2[15];   // указатель на второе пространство ключей
    IndexType1 	msize1; // размер области 1-го пространства ключей
    IndexType2 	msize2; // размер области 2-й пространства ключей
    IndexType1 	csize1; // количество элементов в первой области
    IndexType1 	csize2; // количество элементов во второй области
} Table;

int D_Add(Table *), //вставка элемента в таблицу
    D_Find(Table *), //поиск элемента в таблице по ключу
    D_Delete(Table *), //удаление по ключу
    D_Show(Table *), //вывод таблицы на экран
    D_Delete_All(Table *), // удаление из таблицы всех элементов, заданного ключом в одном из ключевых пространств
    D_Find_By_Any_Key(Table *), //поиск в таблице элемента по любому заданному ключу
         /* особые операции для первого пространства */
    D_Delete_Keyspace1(Table *),// удаление из таблицы элемента, заданного своим ключом
    D_Find_Keyspace1(Table *);//поиск в таблице всех элементов, заданных диапазоном ключа родительского элемента

int init_table(Table *),//создание таблиц
    delTable(Table *),//удаление таблицы
    find(Table* , int, char *), //проверка наличия элемента по ключам
    insert(Table *, int, char *, int, int, int, char *); //вставка в таблицу

#endif // TABLE_H
