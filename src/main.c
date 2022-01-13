#include "Restaurant.h"

char menuChoose(int aFunc); //menu to user
void menu_DiningRoom_Setting(int *table); //get tables size
void menu_Kitchen_AddItems(P_Restaurant res); //FUNCTION 2
void menu_DiningRoom_OrderItem(P_Restaurant res); //FUNCTION 3
void menu_DiningRoom_RemoveItem(P_Restaurant res); //FUNCTION 4
void menu_DiningRoom_RemoveTable(P_Restaurant res); //FUNCTION 5

void main()
{
	Restaurant res;
	int tables;
	char userSelect;
	FILE *manot = fopen(INPUTFILE, "r");
	fprintf(stdout, "	MTM HW2		Present by:	1. Or Eliyahu ID:x	2. Haim Yanni ID: x\n\
*******************************************The Restaurant*******************************************\n");
	if (manot)
	{
		menu_DiningRoom_Setting(&tables);
		res = createRestaurant(tables); //create restaurant
		do
		{
			userSelect = menuChoose((manot) ? 1 : 0); //show menu and get select
			if (manot) //input is reading
			{
				if (userSelect == '1')
				{
					fgetf_KitchenProducts(&res, manot);
					fclose(manot);
					manot = NULL; //no need this file anymore
					if (res.kitchen.size == 0) //no kitchen product
						unexpectedError(&res, "main", "Do you know a restaurant without products??");
				}
				else
					error_msg("Cannot run any function until run '1' ");
			}
			else //after reading input file
			{
				switch (userSelect) {
				case '1':
					error_msg("Cannot run initialization function 1 again.");
					break;
				case '2':
					menu_Kitchen_AddItems(&res);
					break;
				case '3':
					menu_DiningRoom_OrderItem(&res);
					break;
				case '4':
					menu_DiningRoom_RemoveItem(&res);
					break;
				case '5':
					menu_DiningRoom_RemoveTable(&res);
					break;
				case '6':
					Report(res);
					break;
				case 'e':
					fprintf(stdout, "\n\nbye bye\n");
					freeRestaurant(&res);
					userSelect = NULL;
					break;
				default:
					error_msg("Sorry, I dont find this function");
					break;
				}
			}
		} while (userSelect);
	}
	else
		error_msg("problem to read input file");
}

///menuChoose
//get (int)aFunc- if getting products from file
//return (char) - menu choose
char menuChoose(int aFunc)
{
	char userSelect;
	fprintf(stdout, "\n\n\
=======================MENU=======================\
\nOptions	| Description \
\n---------------------------------------------------------\
\n+ (1)	| Getting Products from file %s \
\n+ (2)	| Add quantity to exists product \
\n+ (3)	| Order to table \
\n+ (4)	| Cancel product order from table \
\n+ (5)	| Close table and receiving a bill\
\n+ (6)	| Report menu \
\n+ (e)	| Exit program \
\n---------------------------------------------------------\
%s\
\n\n\nEnter your choice: \
",
INPUTFILE,
(aFunc) ? "\n	!!!Reminder: Before start any function you need to initialization program with function '1'!!!	" : "");

	userSelect = _getche(); //DONT NEED FFLUSH
	fprintf(stdout, "\n");
	return userSelect;
}

///menu_DiningRoom_Setting
//get (int*)tables - size of tables on dining room
//ask user for setting dining room
void menu_DiningRoom_Setting(int *tables)
{
	fprintf(stdout, "Setting dining room:\nEnter the number of tables: ");
	fscanf(stdin, "%d", tables);
	if (*tables <= 0)
	{
		error_msg("cannot create restaurant with less than 0 tables, \ntry again");
		menu_DiningRoom_Setting(tables);
	}
}

///menu_DiningRoom_OrderItem
//get (P_Restaurant)res - restaurant
//Order item to table
void menu_DiningRoom_OrderItem(P_Restaurant res)
{
	char productName[ITEM_NAME_MAX_LENGTH + 1];
	int quantity, tableID;
	fprintf(stdout, "\n\n******************Order to table**********************");
	Kitchen_printMenu(res->kitchen, FALSE); //print menu
	fprintf(stdout, "Enter the table id(from 1 - %d): ", res->diningRoom.capacityTables);
	fscanf(stdin, "%d", &tableID);
	fprintf(stdout, "Enter the name of the product name: ");
	fscanf(stdin, "%s", productName);
	fprintf(stdout, "Enter quantity to order: ");
	fscanf(stdin, "%d", &quantity);
	OrderItem(res, tableID, productName, quantity);
	fprintf(stdout, "******************************************************\n\n");
}

///menu_Kitchen_AddItems
//get (P_Restaurant)res - restaurant
//Add items to kicthen products
void menu_Kitchen_AddItems(P_Restaurant res)
{
	char productName[ITEM_NAME_MAX_LENGTH + 1];
	int quantity;
	fprintf(stdout, "\n\n******************Add Items to kicthen**********************\n");
	Kitchen_printMenu(res->kitchen, TRUE); //print the menu of kicthen with quantity
	fprintf(stdout, "Enter the name of the product name: ");
	fscanf(stdin, "%s", productName);
	fprintf(stdout, "Enter quantity to add: ");
	fscanf(stdin, "%d", &quantity);
	AddItems(res, productName, quantity);
	fprintf(stdout, "************************************************************\n\n");
}

///menu_DiningRoom_RemoveItem
//get (P_Restaurant)res - restaurant
//Remove item from table
void menu_DiningRoom_RemoveItem(P_Restaurant res)
{
	char productName[ITEM_NAME_MAX_LENGTH + 1];
	int quantity, tableID;
	fprintf(stdout, "\n\n******************Cancel Order**********************\n");
	//DiningRoom_printUsedTables(res->diningRoom); //help to debug
	fprintf(stdout, "Enter the table id(from 1 - %d): ", res->diningRoom.capacityTables);
	fscanf(stdin, "%d", &tableID);
	//if (DiningRoom_existsTable(res->diningRoom, tableID)) //help to debug
	//	if (res->diningRoom.tables[tableID - 1].amountPay != TABLE_AVAILABLE)
	//		Table_printBillTable(res->diningRoom.tables[tableID - 1]);
	fprintf(stdout, "Enter the name of the product name: ");
	fscanf(stdin, "%s", productName);
	fprintf(stdout, "Enter quantity to cancel: ");
	fscanf(stdin, "%d", &quantity);
	RemoveItem(res, tableID, productName, quantity);
	fprintf(stdout, "*****************************************************\n\n");
}

///menu_DiningRoom_RemoveTable
//get (P_Restaurant)res - restaurant
//Close table
void menu_DiningRoom_RemoveTable(P_Restaurant res)
{
	int tableID;
	fprintf(stdout, "\n\n******************Close Table**********************\n");
	//DiningRoom_printUsedTables(res->diningRoom); //help to debug
	fprintf(stdout, "Enter the table id(from 1 - %d): ", res->diningRoom.capacityTables);
	fscanf(stdin, "%d", &tableID);
	RemoveTable(res, tableID);
	fprintf(stdout, "*******************************************************\n\n");
}