/**************************************************************
*************
This is to certify that this project is my own work, based on my personal efforts
in studying and applying the concepts learned. I have constructed the
functions and their respective algorithms and corresponding code by myself.
The program was run, tested, and debugged by my own efforts. I further
certify that I have not copied in part or whole or otherwise plagiarized the
work of other students and/or persons.
                                          <Jericho G. Ocampo>, DLSU ID# <12413453>
***************************************************************
************/

/*
	Description: The program simulates a rng based game. At the start of every week (Sunday), the player is asked to generate cubes using energons wherein the price of cubes
	varies from 80 - 120 energons. The player is only allowed to generate the cubes in stacks (10 cubes). After generating the cubes, the 2nd day will start and during this day,
	the player can sell the generated cubes for certain amounts of energon based on the specific trend. The selling of cubes will continue until day 7 wherein all generated cubes 
	that are not sold yet after day 7 will be automatically recycled for 200 energons per stack. The second week will then start and the loop will continue until the 10th week 
	wherein if the player manages to get a minimum of 1 million energons, the player wins.
	
	Programmed by: Jericho G. Ocampo
	
	Last modified: 11/24/2024
	
	Version: TDM-GCC 4.9.2 64-bit Release
	
	Acknowledgements: https://developer.gnome.org/documentation/guidelines/programming/coding-style.html
					  https://www.tutorialspoint.com/rand-and-srand-in-c
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void devFeature (int *energon, int *week);
void cubeGeneration (int* energon, int generationCost, int* producedCubes);
void weekStart (int* energon, int week);
void swindle (int* energon, int* producedCubes, int price);
void recycle (int* producedCubes, int* energon);
int genCost ();
int findTrend ();
int randomPrice (int min, int max);
void endGame (int energon);

/*
	The main function includes the initiation of the main game. Before starting the game, the player is asked if he/she wants to open developer mode wherein the player can change
	the values of the energon and the week number. If chosen not to open developer mode, the game will start. Aside from the initiation of the main game, the main function also
	checks if the player is still eligible to continue by checking the week number and the amount of energon left. Lastly, the main function also calls the endGame() function that
	checks if the player has won or not at the end of each game cycle.
*/

int main () {
	
	int energon = 10000;							//Initial energon amount to start the game.  			
	int week = 1; 									//Starting week is set to 1.		
	char startOption;								//Stores the address of the user input and is the basis to start developer mode.			
	
	srand(time(0)); 								//Sets a random seed for the syntax rand. This ensure that every time rand is called, it is truly random.	
	
	printf("Welcome to the Energon Cube Production Game!\n");
    printf("\nEnter 'd' to access Developer Mode or any other integer/character to start the game: ");
    scanf(" %c", &startOption);
	
	if (startOption == 'd' || startOption == 'D') {	//Checks if the player wants to enter developer mode.
		devFeature(&energon, &week);
	}
	
	while (week <= 10 && energon >= 800) { 		    //Checks if the player is still eligible to continue the game. If not, the loop will terminate.
		weekStart(&energon, week);  				//Calls the function weekStart()		
		week++; 									//Increments week every time weekStart finished a loop. This ensures that while is not an infinite loop.						
	}
	
	if (energon < 800) {							//Checks if player has less then 800 energons and will printf a statement telling them they lost.
		printf("\nNot enough energon to produce any stacks! The game is now ending!\n");		
	}
	
	endGame(energon); 						   	    //Calls the function endGame() to determine if the user has won or not.						
	
	return 0;
}

/*
	If the player choose to enter developer mode, this function will be called. In this function, the player can modify the starting energons and the starting week. Do note that the
	function will not accept any invalid inputs. For example, if the player choose to modify the energons, it will not accept negative amounts. The same goes for modifying the 
	starting week wherein the only valid week numbers are 1 - 10.
	Precondition: The player should only input the correct data type that is asked for. Integer or character only.
	@param int* energon - An integer pointer that contains the address of energon to update the amount of starting energons.
	@param int* week - An integer pointer that contains the address of week to update the starting week.
*/

void devFeature (int* energon, int* week) {
	
    int choice;									    //Will be the basis of the switch statement to know what modification the player wants to do.

    printf("\n=== Developer Mode ===\n");
    
    do {
        printf("\n1. Modify energon amount\n");
        printf("2. Modify current week\n");
        printf("3. Exit Developer Mode\n");
        printf("\nEnter the number of choice: ");
        scanf("%d", &choice);

        switch(choice){
            case 1:							        //Case 1 modifies the amount of energon.
                printf("\nCurrent Energon: %d\n", *energon);
                printf("Enter new Energon amount: ");
                scanf("%d", energon);
                
                if (*energon < 0) {                 //If user inputs a negative amount of energon, prints a warning and resets energon to default.
                	printf("\n\n!!!! Invalid amount of Energon! Resetting to 10000 Energons. !!!!\n\n");
                	*energon = 10000;               //Resets the amount of energon to default.
				}
				else{
					printf("Energon updated to: %d\n", *energon);
				}
                break;
                
            case 2:							        //Case 2 modifies the starting week.
                printf("\nCurrent week: %d\n", *week);
                printf("Enter new week (1-10): ");
                scanf("%d", week);
                
                if (*week < 1 || *week > 10) {      //Checks if user has inputted a valid week. Week should not be less than 0 and should not be greater than 10.
                    printf("\n\n!!!! Invalid week! Resetting to week 1. !!!!\n\n");
                    *week = 1;				        //Resets week to 1.
                } 
				else{
                    printf("Week updated to: %d\n", *week);
                }
                break;
                
            case 3:							        //Case 3 exits the developer mode.
                printf("\nExiting Developer Mode.\n");
                break;
                
            default:						        //Default will appear if the user choose an invalid input.
                printf("\n\n!!!! Invalid choice! Try again. !!!!\n\n");
                break;
        }
    }while (choice != 3);    				        //The loop will continue until the user inputs 3.
}

/*
	The function weekStart() controls the events of the days within the week and manages the trends. During day 1, the function cubeGeneration() will be called to start 
	the cube production. If the player inputs 0, the current week will be skipped. If the player inputs a VALID number (non-negative and sufficient energon to produce the stacks), 
	the stacks will be produced and will move to day 2. From day 2 to 7, random prices for selling the stacks will be implemented based on the trend and random prices. The trend will
	remain constant all throughout the week while the price will vary day to day.
	@param int* energon - Holds the current amount of energon to be modified.
	@param week - The current week the game is in.
*/

void weekStart (int* energon, int week) {
	
	int producedCubes = 0;					//Initial amount of cubes produced starting from day 1.
	int generationCost =  genCost();	    //Calls in function genCost() to randomize a cost for generating energon cubes that ranges from 80 - 120.
	int trend = findTrend();				//Calls in function findTrend() to randomize a number that ranges from 0 - 2 which is then assigned to a specific trend.
	int day;								//The current day. Starts from day 1 and ends at day 7.
	int price;								//The current price for selling stacks of cubes. Will call the function randomPrice() to randomize the price based on the current trend. This changes daily.
	int minPrice, maxPrice;					//The minimum and maximum price for selling a stack of cube. Will depend on the current trend.
	
	if (trend == 0) {						//Scrap trend
		minPrice = 20;
		maxPrice = generationCost - 10;
	}
	else if (trend == 1) {					//Nominal trend
		minPrice = 80;
		maxPrice = generationCost * 1.05;
	}
	else {									//Primus trend
		minPrice = generationCost;
		maxPrice = generationCost * 4;
	}
	
	printf("\n---------------------------------------------------------------------------\n");
	printf("\nWeek %d begins!\n", week);
	printf("\n=== Week %d Day 1 ===\n\n", week);
	printf("Available Energon: %d\n", *energon);
	printf("Cost per stack (1 stack = 10 cubes): %d\n", generationCost * 10);
	
	cubeGeneration(energon, generationCost, &producedCubes);    //Calls in cubeGeneration() to start the cube generation process.
	
	if(producedCubes == 0) {									//If user inputs 0 in cubeGeneration(), producedCubes will be updated to 0 and the whole week will skip.									
		printf("Skipping the rest of the week!\n");
	}
	else{														//Week will continue if the user inputs a valid number.
		for (day = 2; day <= 7; day++) {						//Will start at day 2 because day 1 is generation day and it will end at day 7.
			printf("\n=== Week %d Day %d ===\n", week, day);
			price = randomPrice(minPrice, maxPrice);    		//Sets price to random based on the minimum and maximum price set by the trends. The variable price changes daily but the min and max price is constant all throughout the week due to trends.
			swindle(energon, &producedCubes, price);    		//Calls the function swindle() to start the selling process of the energon cubes.
		}
		recycle(&producedCubes, energon);						//At the end of each week the function recycle() will be called to recycle the unsold stacks.
	}
	printf("\nWeek %d ends!\n", week);		
}
	
/*
	This function responsible for the cube generation at the start of each week. This function asks the player the number of stacks to be generated and will check if it is valid. If
	valid, the function will ask for a confirmation and once confirmed, the cubes will be generated and the function will end. If the player inputs an invalid amount, the function
	will keep looping until a valid input is entered.
	Precondition: The player should only input the correct data type that is asked for. Integer or character only.
	@param int* energon - Holds the current amount of energon to be modified.
	@param generationCost - The current generation cost for cubes based on the randomizer.
	@param int* producedCubes - The amount of produced cubes.
*/

void cubeGeneration (int* energon, int generationCost, int* producedCubes) {
	
	int stacks;																	//Number of stacks of be generated. Will store the address of the user input and will be checked if valid or not.
	int cost;																	//Variable responsible for calculating the cost based on the number of stacks and generation cost.
	char confirmation;															//The validation to whether or not to continue with the transaction.
	
	do{
		printf("\nEnter number of stacks to be generated (Enter 0 to skip the week): ");
		scanf("%d", &stacks);
		
		if (stacks < 0) {											  			//The player is not allowed to input a negative amount of stacks.
			printf("\n\n!!!! Invalid input! Please try again. !!!!\n\n");
		}
		else if (stacks == 0) {									  				//If user inputs 0 stacks, the week will be skipped.
			printf("\nSkipping cube production.\n");
		}
		else{													  				//Else, the week will continue.
			cost = stacks * generationCost * 10;								//Sets the value for cost. 
			
			if (*energon >= cost) {								  				//Will check if the player has enough energon to produce the specified amount of stacks.
				printf("\nThis will cost %d Energon. Confirm? (y/n): ", cost);
                scanf(" %c", &confirmation);
                
                if (confirmation == 'y' || confirmation == 'Y') {   			//Will check if the player wants to continue with the transaction. 
                	*energon -= cost;											//Subtracts the equivalent amount of cost to *energon.
                	*producedCubes = stacks * 10;								//Generated cubes are added.
                	printf("\nSuccessfully generated %d cubes (%d stacks).\n", *producedCubes, stacks);
				}
				else if (confirmation == 'n' || confirmation == 'N') {
					printf("\nTransaction canceled. Please re-enter your input.\n");
				}
				else{
					printf("\n\n!!!! Invalid input! Please try again. !!!!\n\n");
				}
			}
			else{												  				//Will prompt a warning if the amount of energon is not sufficient enough to generate the cubes.
				printf("\nNot enough Energon to generate that many stacks. Try again.\n");
			}
		} 
		
	}while (*producedCubes == 0 && stacks != 0);					  			//The loop will only terminate if the user choose to skip the week or the user has successfully generated stacks.
}

/*
	This function is called during days 2 - 7 whenever the player is to sell energon cube stacks. The function will only accept 0 and positive integers. If given a negative number,
	the function will continue to loop. Based on the player input, this function will deduct energon cube stacks and will add energons to the players total. This transaction is still
	based on the current trend and randomized price for the day.
	Precondition: The player should only input the correct data type that is asked for. Integer or character only.
	@param int* energon - Holds the current amount of energon to be modified.
	@param int* producedCubes - Holds the current amount of cubes produced to be modified.
	@param price - The current price for selling stacks of energon cubes.
*/

void swindle (int* energon, int* producedCubes, int price) {
	
	int stacks; 												//Stores the player input of how many stacks the player wants to sell.
	int earnings;												//The total earnings from selling energon cubes stacks.
	int continueLoop = 0;										//Responsible for the loop. Will be updated when a transaction is complete to terminate the loop.
	char confirmation;											//Responsible for confirming if the player wants to proceed with the transaction.
	
	do{
		printf("\nEnergon storage: %d\n", *energon);
		printf("Energon cubes available: %d (stacks: %d)\n", *producedCubes, *producedCubes / 10);
		printf("Price per cube for today: %d\n", price);
		printf("Price per stack for today: %d\n", price * 10);
		printf("\nEnter the number of stacks to sell: ");
		scanf("%d", &stacks);
		
		if (stacks < 0) {										//Will prompt a warning if the player inputs a negative amount of stacks.
			printf("\n\n!!!! Invalid input! Please try again. !!!!\n\n");
		}
		else if (stacks * 10 <= *producedCubes) { 				//If the player gives a valid input, it will checked if the amount of stacks the player wants to sell is less than or equal to the current amount of stacks the player holds.
			earnings = stacks * price * 10;						//Sets value for variable earnings.
			printf("\nThis will earn %d Energon. Confirm? (y/n): ", earnings);
            scanf(" %c", &confirmation);
            
            if (confirmation == 'y' || confirmation == 'Y') {   //Will check if the user wants to procede with the transaction.
            	*energon += earnings;							//After transaction, earnings is added to the total amount of energon.
                *producedCubes -= stacks * 10;					//Stacks of cubes is reduced after the transaction.
                printf("Sold %d stacks for %d Energon.\n", stacks, earnings);
                continueLoop = 1;								//Updates the variable continueLoop. This is to terminate the loop
			}
			else if (confirmation == 'n' || confirmation == 'N') {
				printf("\nTransaction canceled. Re-enter your input.\n");
			}
			else{
				printf("\n\n!!!! Invalid input! Please try again. !!!!\n\n");
			}
		}
		else{													//A warning will be displayed if the user inputs an invalid amount of stacks.
			printf("\nNot enough cubes to sell that many stacks. Try again.\n");
		}
		
	}while (continueLoop != 1);									//Loop will continue until the variable continueLoop is unchanged.
}

/*
	Function responsible for recycling unsold stacks of cubes at the end of day 7 of each week. The unsold stacks will be automatically sold for 200 energons per stack.
	@param int* producedCubes - Holds the current amount left over cubes.
	@param int* energon - Holds the current amount of energon to be modified.
*/

void recycle (int* producedCubes, int* energon) {
	
	int stacks;								//Stores the amount of stacks of cube left by dividing the producedCubes variable by 10.
	int recycledEnergon;					//A temporary variable that stores the recycled energon. This will be added to the energon total.
	
	if (*producedCubes > 0) {				//Will check if there are any produced cubes left and will convert the remaining cubes to 200 energons per stack.
		stacks = *producedCubes / 10;       //Sets value to stacks.
		recycledEnergon = stacks * 200;     //Sets value to recycledEnergon.
		*energon += recycledEnergon;        //Energon amount is added equivalent to the amount of recycledEneron.
		*producedCubes = 0;					//Resets cube amount to 0 to start a new week.
		printf("\nRecycled %d stacks for %d Energons!\n", stacks, recycledEnergon);
	}
	else{								    //Function will not modify the amount of energon if there are no cubes left.
		printf("\nNo cubes to recycle.\n");
	}
}

/*
	Function responsible for randomizing the generation cost of a singular energon cube.
	@return - Returns a number from 80 - 120.
*/

int genCost () {
	return rand() % 41 + 80;				//The randomizer will start from 0. 80 is added to offset the randomizer resulting the randomizer to return a value ranging from 80 - 120.
}

/*
	Function responsible for randomizing the trend.
	@return - Returns a random number from 0 - 2.
*/

int findTrend () {
	return rand() % 3; 						//As stated, it will return a random number from 0 - 2.						   
}

/*
	Function responsible for randomizing the price based on the trend.
	@param min - Minimum price based on the current trend.
	@param max - Maximum price based on the current trend.
	@return - Returns a random number based on the minimum and maximum price.
*/

int randomPrice (int min, int max) {
	return rand() % (max - min + 1) + min; //The maximum price is subtracted to the minimum price to get the lower boundary and it is added by 1 because rand() always starts with 0.
}										   //This is then added to the minimum price to offset the whole equation.

/*
	This function is responsible for checking if the player has won or not.
	@param energon - The current energon the user has.
*/

void endGame (int energon) {
	
	if (energon >= 1000000) {			   //If the current energon is greater than or equal to 1 million, the player wins.
		printf("\n---------------------------------------------------------------------------\n");
		printf("\nSoundwave produced a total of %d Energons!\n", energon);
		printf("\nCongratulations, you won!"); 
		printf(" ^_^\n");
	}
	else{							       //If the current energon is less than 1 million, the player loses.	
		printf("\n---------------------------------------------------------------------------\n");
		printf("\nSoundwave only produced a total of %d Energons!\n", energon);
		printf("\nYou lost!"); 
		printf(" >_<\n");		   
	}
}
