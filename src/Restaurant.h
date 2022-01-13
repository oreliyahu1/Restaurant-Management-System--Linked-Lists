#ifndef _Restaurant
#define _Restaurant
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INPUTFILE "Manot.txt" //manot file
#define MAX(a,b) (a>=b)?a:b; //define function max
#define GO_NEXT(curr) curr=curr->next; //move head to next
#define RESERROR(eRROR) if(eRROR!=FALSE) {printError(eRROR); return eRROR;}; //Handles errors

typedef enum STATUS//execution status
{
	TRUE = 1,
	FALSE = 0,
	ITEM_NAME_MAX_LENGTH = 256,
	ITEM_NAME_EXISTS = -1,
	ITEM_NAME_UNEXISTS = -2,
	ITEM_PRICE_NEGATIVE = -3,
	ITEM_QUANTITY_NEGATIVE = -4,
	ITEM_EXCEPTION_QUANTITY = -5,
	DININGROOM_TABLE_UNEXISTS = -6,
	DININGROOM_EMPTY = -7,
	TABLE_AVAILABLE = -8,
	TABLE_INVITEDITEM_UNEXISTS = -9,
}STATUS;

typedef struct Item {//data for ProductList and InvitedItems
	char *name;
	int price;
	int quantity;
}Item;

typedef struct ProductList { //Product List for kitchen //Singly linked lists 
	Item item; //data
	int invitedCount; //Counting of requested products
	struct ProductList *next;
}Product, *P_Product;

typedef struct InvitedItems { //InvitedItems for table //Doubly Linked List
	Item item; //data
	struct InvitedItems *next, *perv;
}InvitedItems, *P_InvitedItems;

typedef struct Kitchen { //manger structure for kitchen 
	int size;
	struct ProductList *products;
}Kitchen, *P_Kitchen;

typedef struct Table { //manger structure for table
	int ID;
	int amountPay;
	P_InvitedItems invitedItems;
}Table, *P_Table;

typedef struct DiningRoom { //manger structure for dininig room
	int capacityTables;
	int takenTables;
	P_Table tables;
}DiningRoom, *P_DiningRoom;

typedef struct Restaurant { //structure for restaurant
	Kitchen kitchen;
	DiningRoom diningRoom;
}Restaurant ,*P_Restaurant;

/////////////////////Auxiliary functions
//general
Restaurant createRestaurant(int tables); //create Restaurant(kit&dr)
Kitchen createKitchen(); //create kicthen
char * createDynamicString(char *str); //get string and return dynamic string(copy of str)
DiningRoom createDiningRoom(int tables); //create dining room
P_InvitedItems createInvitedItems(Item i); //create Invited Item for table
void freeRestaurant(P_Restaurant res); //free restaurant(kit&dr)
void freeKitchen(P_Kitchen kp); //free kitchen
void freeDiningRoom(P_DiningRoom dr);//free dining room
void freeTable(P_Table t); //free table
void printError(STATUS error); //print resaurant error
void unexpectedError(P_Restaurant res, char *func, char *msg); //unexpected Error close free all memory and close program
void error_msg(char *msg); //error message
STATUS Item_checkItem(Item i); //check if item quantity & price is rational
//kitchen functions
P_Product Kitchen_addProduct(P_Kitchen kt, P_Product p); //create produt
P_Product Kitchen_existsProduct(Kitchen kt, char *nameProduct);
void Kitchen_printRequestedInvitedProducts(Kitchen kt);
void Kitchen_printMenu(Kitchen kt, STATUS q);
void Kitchen_printMissingProducts(Kitchen kt);
//DiningRoom functions
P_Table DiningRoom_existsTable(DiningRoom dr, int tableNumber);
void DiningRoom_printUnPaidTables(DiningRoom dr);
//Table functions
P_InvitedItems Table_addInvitedItem(P_Table t, Item i, int quantity);
P_InvitedItems Table_existsInvitedItem(Table ti, char *productName);
void Table_removeInvitedItem(P_Table t, P_InvitedItems ii);
void Table_printBillTable(Table t);
//////////////////////////////////////

//HW FUNCS
P_Product CreateProduct(Item i); //(1) create product for kitchen //change prameters to item
void fgetf_KitchenProducts(P_Restaurant res, FILE *in); //(1) get kitchen products from file
STATUS AddItems(P_Restaurant res, char* productName, int quantity); //(2) - add quantity to kitchen product
STATUS OrderItem(P_Restaurant res, int tableNumber, char *productName, int quantity); //(3) - order item for table
STATUS RemoveItem(P_Restaurant res, int tableNumber, char *productName, int quantity); //(4) - cancel item(by quantity) from table
STATUS RemoveTable(P_Restaurant res, int tableNumber); //(5) - close table
void Report(Restaurant res); //(6) report menu
////////////////////////////////
#endif