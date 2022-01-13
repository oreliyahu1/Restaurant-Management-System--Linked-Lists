#include "Restaurant.h"

///createRestaurant
//get tables(int) - number of tables
//return (Restaurant)
Restaurant createRestaurant(int tables)
{
	Restaurant res;
	res.diningRoom = createDiningRoom(tables);
	if (res.diningRoom.tables) //if make tables array
		res.kitchen = createKitchen();
	else
		unexpectedError(&res, "createRestaurant", "cannot create tables for dining room (Memory allocation failed)");
	return res;
}

///createKitchen
//return (Kitchen)
Kitchen createKitchen()
{
	Kitchen kt = { 0, NULL };
	return kt;
}

///createDynamicString
//get (char *)str - string to copy and make dynamic memory
//return (char *) - pointer to dynamic memory of the string
char * createDynamicString(char *str)
{
	char *r = (char*)malloc(sizeof(char) * (strlen(str) + 1)); // +1 becuase of \0
	return (r) ? strcpy(r, str) : NULL; //file to create return null else copy string to the new memory allocation
}

///createDiningRoom
//get (int)tables - size of arr table
//return (DiningRoom), return empty dining room if failed
DiningRoom createDiningRoom(int tables)
{
	int i;
	DiningRoom dr;
	dr.tables = (P_Table)malloc(sizeof(Table) * tables); //create table arr
	dr.capacityTables = (dr.tables) ? tables : 0;
	for (i = 0; i < dr.capacityTables; i++) //Initializing all tables
	{
		dr.tables[i].ID = i + 1;
		dr.tables[i].amountPay = TABLE_AVAILABLE;
		dr.tables[i].invitedItems = NULL;
	}
	dr.takenTables = DININGROOM_EMPTY;
	return dr;
}

///CreateProduct
//get (Item)i - data of product
//return (P_Product) - node of product list (return null if failed)
P_Product CreateProduct(Item i) //create produt
{
	P_Product p = (P_Product)malloc(sizeof(Product)); //create the node
	if (p)
	{
		p->item.name = createDynamicString(i.name);
		if (p->item.name)
		{
			p->item.price = i.price;
			p->item.quantity = i.quantity;
			p->invitedCount = 0;
			p->next = NULL;//del trash pointer
		}
		else //string memory allocation failed 
		{
			free(p);
			p = NULL;
		}
	}
	return p;
}

///createInvitedItems
//get (Item)i - data of Invited Items
//return (P_InvitedItems) - node of Invited Items (return null if failed)
P_InvitedItems createInvitedItems(Item i) //create Invited Item
{
	P_InvitedItems ii = (P_InvitedItems)malloc(sizeof(InvitedItems));
	if (ii)
	{
		ii->item.name = i.name; //COPY POINTER NAME //dont need to new memory allocation
		ii->item.price = i.price;
		ii->item.quantity = i.quantity;
		ii->next = ii->perv = NULL;//del trash pointer
	}
	return ii;
}

///freeRestaurant
//get (P_Restaurant)res - restaurant
//free kitchen and dining room of restaurant
void freeRestaurant(P_Restaurant res)
{
	freeKitchen(&res->kitchen);
	freeDiningRoom(&res->diningRoom);
}

///freeKitchen
//get (P_Kitchen)kp - kitchen
//free all kitchen memory allocation (products list)
void freeKitchen(P_Kitchen kp)
{
	P_Product tmp;
	while (kp->products)
	{
		tmp = kp->products;
		GO_NEXT(kp->products);
		free(tmp->item.name);
		tmp->item.name = NULL; //Prevent errors (Pointer copied to invitedItems.Item)
		free(tmp);
		kp->size--;
	}
}

///freeDiningRoom
//get (P_DiningRoom)dr - dining room
//free all dining room memory allocation (tables array)
void freeDiningRoom(P_DiningRoom dr)
{
	while (dr->capacityTables)
	{
		freeTable(&dr->tables[dr->capacityTables - 1]);
		dr->capacityTables--;
	}
	if (dr->tables)
		free(dr->tables);
	dr->takenTables = DININGROOM_EMPTY;
	dr->tables = NULL;
}

///freeTable
//get (P_Table)t - table
//free all table memory allocation (invited items)
void freeTable(P_Table t)
{
	P_InvitedItems tmp;
	while (t->invitedItems)
	{
		tmp = t->invitedItems;
		GO_NEXT(t->invitedItems);
		//free(t->invitedItems->item.name); Dont freeeeeeee!!! its copied pointer from kicthen product!!!!!
		free(tmp);
	}
	t->amountPay = TABLE_AVAILABLE;
}

///printError
//get (STATUS)error - error define num
//print the appropriate error
void printError(STATUS error)
{
	switch (error)
	{
	case ITEM_NAME_EXISTS:
		error_msg("Exists product on kitchen");
		break;
	case ITEM_NAME_UNEXISTS:
		error_msg("Unexists product on kitchen");
		break;
	case ITEM_QUANTITY_NEGATIVE:
		error_msg("Less than 0 quantity");
		break;
	case ITEM_EXCEPTION_QUANTITY:
		error_msg("More than have quantity");
		break;
	case ITEM_PRICE_NEGATIVE:
		error_msg("Less than 0 price");
		break;
	case DININGROOM_TABLE_UNEXISTS:
		error_msg("ID table not exists");
		break;
	case TABLE_AVAILABLE:
		error_msg("The table is availabe");
		break;
	case DININGROOM_EMPTY:
		error_msg("All tables are available");
		break;
	case TABLE_INVITEDITEM_UNEXISTS:
		error_msg("Not found this product on table");
		break;
	default:
		break;
	}
}

///unexpectedError
//get (P_Restaurant)res - resestaurant, (char *)func - function name, (char *)msg- message to user
//show to user the problem, ree allocation memory and close the program
void unexpectedError(P_Restaurant res, char *func, char *msg)
{
	fprintf(stdout, "\n===Unexpected Error===\nFunction: %s\nError: %s\n======================\n", func, msg);
	freeRestaurant(res);
	exit(1);
}

///error_msg
//get (char *)msg- message to user
//show to user the error message
void error_msg(char *msg)
{
	fprintf(stdout, "==================================\nError: %s\n==================================\n", msg);
}

///Item_checkItem
//get (Item)i- data item
//check data item (price & quantity)
//return (STATUS) depending on the results
STATUS Item_checkItem(Item i) /////make change
{
	if (i.quantity <= 0)
		return ITEM_QUANTITY_NEGATIVE;
	else if (i.price <= 0)
		return ITEM_PRICE_NEGATIVE;
	return TRUE;
}

///fgetf_KitchenProducts
//get (P_Restaurant)res- restaurant to add products, (FILE *)in - file to reading products
//read all file, creates products accordingly to file and puts on kitchen products list(according to the instructions)
void fgetf_KitchenProducts(P_Restaurant res, FILE *in)
{
	int runsCounter; //number of runs on file
	STATUS eRROR; //error status
	Item iTmp; //temp data
	char name[ITEM_NAME_MAX_LENGTH + 1]; //name temp for reading connecting iTmp.name
	P_Product pTmp; //product node temp
	for (iTmp.name = name, runsCounter = 0; fscanf(in, "%s %d %d", iTmp.name, &iTmp.quantity, &iTmp.price) != EOF; runsCounter++, eRROR = TRUE)
	{
		if ((eRROR = Item_checkItem(iTmp)) == TRUE)
		{
			if ((pTmp = CreateProduct(iTmp)) == NULL) //failed memory allocation
				unexpectedError(res, "fgetf_KitchenProducts", "cannot create product (Memory allocation failed)");

			if (Kitchen_addProduct(&res->kitchen, pTmp) == NULL) //same name
			{//throws node
				free(pTmp->item.name);
				free(pTmp);
				eRROR = ITEM_NAME_EXISTS; //update error
			}
		}

		if (eRROR != TRUE)
		{
			fprintf(stdout, "cannot add %s product\n", iTmp.name);
			printError(eRROR);
		}
	}
	fprintf(stdout, "\nSuccessfully received %d from file\n\
Error for %d products\n", res->kitchen.size, (runsCounter - res->kitchen.size));
}

///DiningRoom_existsTable
//get (DiningRoom)dr- Dining Room, (int)tableNumber - id of table
//return (P_Table) - the table ID(tableNumber), return null if unexists table
P_Table DiningRoom_existsTable(DiningRoom dr, int tableNumber)
{
	if ((tableNumber > 0) && (tableNumber <= dr.capacityTables))
		return &dr.tables[tableNumber - 1];
	return NULL;
}

///DiningRoom_printUnPaidTables
//get (DiningRoom)dr- Dining Room
//print all tables that sitting on dining room
void DiningRoom_printUnPaidTables(DiningRoom dr)
{
	int i;
	fprintf(stdout, "Unpaid Table\
\n--------------------------------------------------\n");
	for (i = 0; i < dr.capacityTables; i++)
	{
		if (dr.tables[i].amountPay != TABLE_AVAILABLE) //only sitting table
		{
			fprintf(stdout, "Unpaid table number %d \n", dr.tables[i].ID);
			Table_printBillTable(dr.tables[i]);
		}
	}
	if (dr.takenTables == DININGROOM_EMPTY)
		printError(DININGROOM_EMPTY);
	fprintf(stdout, "--------------------------------------------------\n");
}

///Kitchen_addProduct
//get (P_Kitchen)kt- kitchen, (P_Product)p node to add
//add (P_Product)p to kitchen product list (alphabetical order, error at duplicate name)
//return (P_Product), return null if have the same product on kicthen
P_Product Kitchen_addProduct(P_Kitchen kt, P_Product p)
{
	int r;
	P_Product pkp;
	if (kt->products == NULL) //empty
		kt->products = p;
	else
	{
		if ((r = strcmp(kt->products->item.name, p->item.name)) == 0)
			return NULL;
		else if (r > 0) //add as first
		{
			p->next = kt->products;
			kt->products = p;
		}
		else
		{
			pkp = kt->products;
			while ((pkp->next) && ((r = strcmp(pkp->next->item.name, p->item.name)) < 0)) //search place to add
				GO_NEXT(pkp);
			if (r == 0)
				return NULL;
			p->next = pkp->next;
			pkp->next = p;
		}
	}
	kt->size++;
	return p;
}

///Kitchen_existsProduct
//get (Kitchen)kt- kitchen, (char *)nameProduct
//search if nameProduct is exists product 
//return (P_Product), return null if unexists product
P_Product Kitchen_existsProduct(Kitchen kt, char *nameProduct)
{
	while (kt.products)
	{
		if (strcmp(kt.products->item.name, nameProduct) == 0) //exists product !!!
			return kt.products;
		GO_NEXT(kt.products);
	}
	return kt.products; //will return null
}

///Kitchen_printRequestedInvitedProducts
//get (Kitchen)kt- kitchen
//print the requested invited products
void Kitchen_printRequestedInvitedProducts(Kitchen kt)
{
	int maxInvited = 0;
	P_Product kps = kt.products;
	while (kps) //seacrh the max invited number
	{
		maxInvited = MAX(maxInvited, kps->invitedCount);
		GO_NEXT(kps);
	}
	kps = kt.products; //Initializing pointer to head
	if (maxInvited) //if have max inited
	{
		fprintf(stdout, "\nRequested Order Product\
\nItem Name	|	Quantity\
\n--------------------------------------------------\n");
		while (kps)
		{
			if (kps->invitedCount == maxInvited)
				fprintf(stdout, "%s		|	%d\n", kps->item.name, kps->invitedCount);
			GO_NEXT(kps);
		}
		fprintf(stdout, "--------------------------------------------------\n");
	}
	else
		error_msg("No requested invited products");
}

///Kitchen_printMenu
//get (Kitchen)kt- kitchen, (STATUS)q - print quantity request
//print the kitchen menu (whether or not quantity according to q)
void Kitchen_printMenu(Kitchen kt, STATUS q)
{
	fprintf(stdout, "\n====================Kitchen menu====================\
\nItem Name	|%s	Price\
\n--------------------------------------------------\n", (q == TRUE) ? "	Quantity	|" : "");
	while (kt.products)
	{
		fprintf(stdout, "%s		|", kt.products->item.name);
		if (q == TRUE)
			fprintf(stdout, "	%d		|", kt.products->item.quantity);
		fprintf(stdout, "	%d\n", kt.products->item.price);
		GO_NEXT(kt.products);
	}
	fprintf(stdout, "===================================================\n");
}

///Kitchen_printMissingProducts
//get (Kitchen)kt- kitchen
//print all missing products in kitchen
void Kitchen_printMissingProducts(Kitchen kt)
{
	STATUS f = FALSE; //flag to have missing product
	fprintf(stdout, "Missing products in the kitchen\
\nItem Name	|	Quantity	|	Price\
\n--------------------------------------------------\n");
	while (kt.products)
	{
		if (kt.products->item.quantity == 0)
		{
			f = TRUE;
			fprintf(stdout, "%s		|	%d		|	%d\n", kt.products->item.name, kt.products->item.quantity, kt.products->item.price);
		}
		GO_NEXT(kt.products);
	}
	if (kt.size == 0)
		fprintf(stdout, "no have products in Kitchen\n");
	else if (f == FALSE)
		fprintf(stdout, "have all products in kitchen\n");
	fprintf(stdout, "--------------------------------------------------\n");
}

///Table_addInvitedItem
//get (P_Table)t- table, (Item)i - data item to add invited items, (int)quantity - to add
//return (P_InvitedItems) - the invited item that added quantity return null if unexists have this item name and falied at memory alloction
P_InvitedItems Table_addInvitedItem(P_Table t, Item i, int quantity) //create produt
{
	P_InvitedItems ii = Table_existsInvitedItem(*t, i.name);
	if (ii)
		ii->item.quantity += quantity;
	else
	{
		i.quantity = quantity; //dont a fraid its not change the original item of kitchen
		if (ii = createInvitedItems(i)) //create and add to list invited item on table
		{
			if (t->invitedItems == NULL)
				t->invitedItems = ii;
			else
			{
				t->invitedItems->perv = ii;
				ii->next = t->invitedItems;
				t->invitedItems = ii;
			}
		}
	}
	return ii;
}

///Table_existsInvitedItem
//get (Table)t- table, (char *)productName - product name to search
//return (P_InvitedItems) - the invited item with same product name, return null if unexists
P_InvitedItems Table_existsInvitedItem(Table ti, char *productName)
{
	while (ti.invitedItems)
	{
		if (strcmp(ti.invitedItems->item.name, productName) == 0)
			return ti.invitedItems;
		GO_NEXT(ti.invitedItems);
	}
	return ti.invitedItems;
}

///Table_removeInvitedItem
//get (P_Table)t- table, (P_InvitedItems)ii - node to remove
//remove&free the ii node from table nvitedItems list
void Table_removeInvitedItem(P_Table t, P_InvitedItems ii)
{
	if (t->invitedItems == ii)
	{
		GO_NEXT(t->invitedItems);
		if (t->invitedItems)
			t->invitedItems->perv = NULL;
	}
	else
	{
		ii->perv->next = ii->next;
		if (ii->next)
			ii->next->perv = ii->perv;
	}
	free(ii);
}

///Table_printBillTable
//get (Table)t- table
//print the current bill of table
void Table_printBillTable(Table t)
{
	if (t.amountPay != TABLE_AVAILABLE)
	{
		fprintf(stdout, "Items ordered\
\nItem Name	|	Quantity	|	Price	|	Total\
\n--------------------------------------------------------------------------\n");
		while (t.invitedItems)
		{
			fprintf(stdout, "%s		|	%d		|	%d	|	%d\n\
", t.invitedItems->item.name, t.invitedItems->item.quantity, t.invitedItems->item.price, (t.invitedItems->item.price * t.invitedItems->item.quantity));
			GO_NEXT(t.invitedItems);
		}
		fprintf(stdout, "--------------------------------------------------------------------------\n");
		if (t.amountPay == 0)
			fprintf(stdout, "No products were ordered\n");
		fprintf(stdout, "Amount to pay: %d\n", t.amountPay);
	}
	else
		printError(TABLE_AVAILABLE);

}

///AddItems - to kicthen products (2)
//get (P_Restaurant)res- restaurant, (char *)productName, (int)quantity - to add
//add quantity to productName on kitchen products list
//return (STATUS) the result function, TRUE if ok, else another STATUS of failed
STATUS AddItems(P_Restaurant res, char *productName, int quantity)
{
	P_Product p;
	STATUS eRROR = FALSE;
	if (quantity <= 0)
		eRROR = ITEM_QUANTITY_NEGATIVE;
	else if ((p = Kitchen_existsProduct(res->kitchen, productName)) == NULL)
		eRROR = ITEM_NAME_UNEXISTS;
	RESERROR(eRROR);
	p->item.quantity += quantity;
	return TRUE;
}

///OrderItem - to table invited items (3)
//get (P_Restaurant)res- restaurant, (int)tableNumber - table id, (char *)productName, (int)quantity - to add
//add product name to table InvitedItems list on table according to quantity
//return (STATUS) the result function, TRUE if ok, else another STATUS of failed
STATUS OrderItem(P_Restaurant res, int tableNumber, char *productName, int quantity)
{
	P_Product p;
	P_Table t;
	P_InvitedItems ii;
	STATUS eRROR = FALSE;
	if ((t = DiningRoom_existsTable(res->diningRoom, tableNumber)) == NULL)
		eRROR = DININGROOM_TABLE_UNEXISTS;
	else if ((p = Kitchen_existsProduct(res->kitchen, productName)) == NULL)
		eRROR = ITEM_NAME_UNEXISTS;
	else if (quantity <= 0)
		eRROR = ITEM_QUANTITY_NEGATIVE;
	else if (quantity > p->item.quantity)
		eRROR = ITEM_EXCEPTION_QUANTITY;
	else if ((ii = Table_addInvitedItem(t, p->item, quantity)) == NULL)
		unexpectedError(res, "OrderItem", "cannot create invited item (Memory allocation failed)");

	RESERROR(eRROR);

	if (t->amountPay == TABLE_AVAILABLE) //a new table need to open
	{
		if (res->diningRoom.takenTables == DININGROOM_EMPTY) //no longer an empty dining room
			res->diningRoom.takenTables = 0;
		res->diningRoom.takenTables++;
		t->amountPay = 0;
		fprintf(stdout, "Open table id %d\n", t->ID);
	}
	p->invitedCount += quantity;
	p->item.quantity -= quantity;
	t->amountPay += ii->item.price * quantity;
	fprintf(stdout, "\nSuccessfully added %s product (* %d quantity) to table id: %d\
\nCurrent amount pay: %d\n\n", p->item.name, quantity, t->ID, t->amountPay);
	return TRUE;
}

///RemoveItem - remove item from invited items list on table (4)
//get (P_Restaurant)res- restaurant, (int)tableNumber - table id, (char *)productName, (int)quantity - to remove
//remove product name from table InvitedItems list on table according to quantity
//return (STATUS) the result function, TRUE if ok, else another STATUS of failed
STATUS RemoveItem(P_Restaurant res, int tableNumber, char *productName, int quantity)
{
	P_Product p;
	P_Table t;
	P_InvitedItems ii;
	STATUS eRROR = FALSE;
	if ((t = DiningRoom_existsTable(res->diningRoom, tableNumber)) == NULL)
		eRROR = DININGROOM_TABLE_UNEXISTS;
	else if (t->amountPay == TABLE_AVAILABLE)
		eRROR = TABLE_AVAILABLE;
	else if ((p = Kitchen_existsProduct(res->kitchen, productName)) == NULL)
		eRROR = ITEM_NAME_UNEXISTS;
	else if ((ii = Table_existsInvitedItem(*t, productName)) == NULL)
		eRROR = TABLE_INVITEDITEM_UNEXISTS;
	else if (quantity <= 0)
		eRROR = ITEM_QUANTITY_NEGATIVE;
	else if (quantity > ii->item.quantity)
		eRROR = ITEM_EXCEPTION_QUANTITY;
	RESERROR(eRROR);

	ii->item.quantity -= quantity;
	t->amountPay -= ii->item.price * quantity;
	if (ii->item.quantity == 0) //delete all item quantity, need remove node from list
		Table_removeInvitedItem(t, ii);
	p->invitedCount -= quantity; //No longer requested as past
	//if (t->amountPay == 0) //this is nottt TABLE_AVAILABLE
	return TRUE;
}

///RemoveTable - close table (5)
//get (P_Restaurant)res- restaurant, (int)tableNumber - table id
//close the table by id and print the bill
//return (STATUS) the result function, TRUE if ok, else another STATUS of failed
STATUS RemoveTable(P_Restaurant res, int tableNumber)
{
	P_Table t;
	STATUS eRROR = FALSE;
	if ((t = DiningRoom_existsTable(res->diningRoom, tableNumber)) == NULL)
		eRROR = DININGROOM_TABLE_UNEXISTS;
	else if (t->amountPay == TABLE_AVAILABLE)
		eRROR = TABLE_AVAILABLE;
	RESERROR(eRROR);

	fprintf(stdout, "Close table number %d \n", t->ID);
	Table_printBillTable(*t);

	res->diningRoom.takenTables--;
	if (res->diningRoom.takenTables == 0) //the dining room is empty
		res->diningRoom.takenTables = DININGROOM_EMPTY;
	freeTable(t); //bye bye
	return TRUE;
}

///REPORT MENU (6)
//get (Restaurant)res- restaurant
//ask user the type of report to make
void Report(Restaurant res)
{
	char userSelect;
	fprintf(stdout, "\n=============================REPORTS MENU=============================\
\nOptions	| Description\
\n--------------------------------------------------\
\n+ (A)	| Print Requested products\
\n+ (B)	| Missing products in the kitchen\
\n+ (C)	| Print unpaid products(for each table)\
\n--------------------------------------------------\
\n\n\nEnter your choice: \
");
	userSelect = _getche(); //DONT NEED FFLUSH
	fprintf(stdout, "\n");
	//while ((getchar()) != '\n') {} //alternative FLUSHALL function (visual 2015 problem)
	switch (userSelect)
	{
	case 'A':
		Kitchen_printRequestedInvitedProducts(res.kitchen); //(Print Requested products)
		break;
	case 'B':
		Kitchen_printMissingProducts(res.kitchen); //(Missing products in the kitchen)
		break;
	case 'C':
		DiningRoom_printUnPaidTables(res.diningRoom); //(Print unpaid products(for each table))
		break;
	default:
		error_msg("You can only select existing menu options, try again");
		Report(res);
		break;
	}
}
