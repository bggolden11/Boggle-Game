
/* 

    Running this program looks like:

        Welcome to the game of Boggle, where you play against the clock
        to see how many words you can find using adjacent letters on the
        board.  Each letter can be used only once for a given word.
 
 
        The dictionary total number of words is: 263533
        Number of words of the right length is:  259709

        Some random characters are: a c r l n e a p p u

        Do word lookups for 10 seconds.

        Enter a word: fish
        fish is in the dictionary.
        There are 3 seconds left.

        Enter a word: cat
        cat is in the dictionary.
        There are 3 seconds left.

        Enter a word: dog
        dog is in the dictionary.
        There are 1 seconds left.

        Enter a word: entropy
        entropy is in the dictionary.
        There are -3 seconds left.
        I let you finish your last move. Time is up!
 
*/
#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <cassert>    // For the assert statement
#include <cctype>     // Allows using the tolower() function
#include <ctime>      // Timer functions
#include <cstring>    // For strlen
using namespace std;

// Global constants
const char DictionaryFileName[] = "dictionary.txt";
const int MaxNumberOfWords = 263533; // Number of dictionary words
const int MinWordLength = 3;         // Minimum dictionary word size to be stored
const int MaxWordLength = 16;	     // Max word size.  Add 1 for null
const int MaxUserInputLength = 81;   // Max user input length
const int NumberOfLetters = 26;      // Letters in the alphabet
const int TotalSecondsToPlay = 60;   // Total number of seconds to play per board


//--------------------------------------------------------------------------------
// Display name and program information
void displayIdentifyingInformation()
{
    printf("\n");
    printf("Author: Brian Goldenberg        \n");
    printf("Program: #5, Boggle        \n");
    printf("TA: Grace Hopper, Tues 10-11  \n");
    printf("Nov 5, 2017                \n");
    printf("\n");
}//end displayIdentifyingInformation()


//--------------------------------------------------------------------------------
// Display instructions
void displayInstructions()
{
    printf("Welcome to the game of Boggle, where you play against the clock   \n");
    printf("to see how many words you can find using adjacent letters on the  \n");
    printf("board.  Each letter can be used only once for a given word.       \n");
    printf("  \n");
    printf("When prompted to provide input you may also:                      \n");
    printf("     Enter 'r' to reset the board to user-defined values.         \n");
    printf("     Enter 's' to solve the board and display all possible words. \n");
    printf("     Enter 't' to toggle the timer on/off.                        \n");
    printf("     Enter 'x' to exit the program.                               \n");
	printf("  \n");
}//end displayInstructions()


//---------------------------------------------------------------------------
// Read in dictionary
//    First dynamically allocate space for the dictionary.  Then read in words
// from file.  Note that the '&' is needed so that the new array address is
// passed back as a reference parameter.
void readInDictionary(
          char ** &dictionary,                      // dictionary words
          long int &numberOfWords)                  // number of words stored
{
    // Allocate space for large array of C-style strings
    dictionary = new char*[ MaxNumberOfWords];
    
    // For each array entry, allocate space for the word (C-string) to be stored there
    for (int i=0; i < MaxNumberOfWords; i++) {
        dictionary[i] = new char[ MaxWordLength+1];
        // just to be safe, initialize C-strings to all null characters
        for (int j=0; j < MaxWordLength; j++) {
            dictionary[i][j] = '\0';
        }//end for (int j=0...
    }//end for (int i...
    
    // Now read in the words from the file
    ifstream inStream;                 // declare an input stream for my use
    numberOfWords = 0;                   // Row for the current word
    inStream.open( DictionaryFileName);
    assert( ! inStream.fail() );       // make sure file open was OK
    
    // Keep repeating while input from the file yields a word
    char theWord[ 81];    // declare input space to be clearly larger than largest word
    while( inStream >> theWord) {
        int wordLength = (int)strlen( theWord);
        if( wordLength >= MinWordLength && wordLength <= MaxWordLength) {
            strcpy( dictionary[ numberOfWords], theWord);
            // increment number of words
            numberOfWords++;
        }
    }//end while( inStream...
    
    cout << "The dictionary total number of words is: " << MaxNumberOfWords << endl;
    cout << "Number of words of the right length is:  " << numberOfWords << endl;
    
    // close the file
    inStream.close();
}//end readInDictionary()


//--------------------------------------------------------------------------------------
// Use binary search to look up the search word in the dictionary array, returning index
// if found, -1 otherwise
int binarySearch( const char searchWord[ MaxWordLength+1], // word to be looked up
                  char **dictionary)               // the dictionary of words
{
    int low, mid, high;     // array indices for binary search
    int searchResult = -1;  // Stores index of word if search succeeded, else -1
    
    // Binary search for word
    low = 0;
    high = MaxNumberOfWords - 1;
    while ( low <= high)  {
        mid = (low + high) / 2;
        // searchResult negative value means word is to the left, positive value means
        // word is to the right, value of 0 means word was found
        searchResult = strcmp( searchWord, dictionary[ mid]);
        if ( searchResult == 0)  {
            // Word IS in dictionary, so return the index where the word was found
            return mid;
        }
        else if (searchResult < 0)  {
            high = mid - 1; // word should be located prior to mid location
        }
        else  {
            low = mid + 1; // word should be located after mid location
        }
    }
    
    // Word was not found
    return -1;
}//end binarySearch()


//---------------------------------------------------------------------------
// Get random character
//    Find random character using a table of letter frequency counts.
// Iterate through the array and find the first position where the random number is
// less than the value stored.  The resulting index position corresponds to the
// letter to be generated (0='a', 1='b', etc.)
char getRandomCharacter()
{
    // The following table of values came from the frequency distribution of letters in the dictionary
    float letterPercentTotals[ NumberOfLetters] = {
                0.07680,  //  a
                0.09485,  //  b
                0.13527,  //  c
                0.16824,  //  d
                0.28129,  //  e
                0.29299,  //  f
                0.32033,  //  g
                0.34499,  //  h
                0.43625,  //  i
                0.43783,  //  j
                0.44627,  //  k
                0.49865,  //  l
                0.52743,  //  m
                0.59567,  //  n
                0.66222,  //  o
                0.69246,  //  p
                0.69246,  //  q
                0.76380,  //  r
                0.86042,  //  s
                0.92666,  //  t
                0.95963,  //  u
                0.96892,  //  v
                0.97616,  //  w
                0.97892,  //  x
                0.99510,  //  y
                1.00000}; //  z
    
    // generate a random number between 0..1
    // Multiply by 1.0 otherwise integer division truncates remainders
    float randomNumber = 1.0 * rand() / RAND_MAX;
    
    // Find the first position where our random number is less than the
    // value stored.  The index corresponds to the letter to be returned,
    // where 'a' is 0, 'b' is 1, and so on.
    for( int i=0; i<NumberOfLetters; i++) {
        if( randomNumber < letterPercentTotals[ i]) {
            // we found the spot.  Return the corresponding letter
            return (char) 'a' + i;
        }
    }
    
    // Sanity check
    cout << "No alphabetic character generated.  This should not have happened. Exiting program.\n";
    exit( -1);
    return ' ';   // should never get this
}//end getRandomCharacter

//---------------------------------------------------------------------------
//FUcntion to set board values to random character
void resetBoardRandom(char board[])
	{
		for(int i = 0; i < 36; i++)
			{
				if (i < 6 || i > 28)//Case of top and bottom of board to be '*'
					{
						board[i]='*';
					}
				else if( i == 6 || i == 11 || i == 12 || i == 17 || i == 18 || i == 23|| i == 24 ) //Case for very left an very right to be '*'
				{
					board[i]= '*';
				}
				else
				{	
					board[i]=getRandomCharacter();
				}
			}
			
	}//end resetBoardRando,
//----------------------------------------------------------------
void resetBoard(char board[])
	{
		char temp;
		for(int i =0; i < 36; i++)
			{
				if(i < 6 || i > 28)
					{
						board[i]='*';
					}
				else if( i == 6 || i == 11 || i == 12 || i == 17 || i == 18 || i == 23|| i == 24 )
				{
					board[i]= '*';
				}
				else
				{	
				cin >> temp;
				board[i] = temp;
				}
			}
	}
//----------------------------------------------------------------------------
//Function to display the playing board
void displayBoard(char board[])
	{
		for (int i = 7; i < 29; i++)
			{
				if (i % 6 == 0 )
					{
						cout << endl;
					}
				if (board[i]!='*')
					{
					cout << board[i] << " ";
					}	
			}
	} //End displayBoard
//---------------------------------------------------------------------------
//Function to list all the words found in proper order
void displayWordsFound(bool wordsFound[],char** dictionary)
	{
		for(int x = 3; x<17; x++)
		for(int i = 0; i < 263533; i++)
			{
				if(wordsFound[i]==true && strlen(dictionary[i])==x)
					{
						cout << dictionary[i] << " ";
					}
			}
		
	}//end displayWordsFound
//---------------------------------------------------------------------------
bool checkForWord(char word[], char board[], int locationOnBoard, int locationinArr)
	{
		int offset[] = {-1,-7,-6,-5,1,7,6,5}; 
		bool check = false;
		if (locationinArr == strlen(word))//case Case when end of word is found
			{
				check = true;
			}
			
		else
		{
			for(int i = 0; i < 8; i++) //loops through 8 spots in array offset
				{
					if(word[locationinArr] == board[(locationOnBoard + offset[i] ) ] ) //Checks to see if next letter in array is found at the spot 
						{
							char store;
							store = board[locationOnBoard];
							board[locationOnBoard] = ' '; //Blanks out spot on baord so the spot cannot be used again 
							check = checkForWord(word, board, locationOnBoard + offset[i], ++locationinArr); //Recursive call of the function until en of the word is found
							board[locationOnBoard] = store; //restores blanked out value on board
							if(check == 1) break;
						}
				}
		}
	return check; //Returns true when word is found
	}
//------------------------------------------------------------------------
//Function to find first letter in userInput
void firstLetterFind(char board[], char userInput[], bool& wordFound) 
	{
	char store;
		    for(int i =0; i < 36; i++)
		    	{
		    		if (board[i]==userInput[0])
		    			{
							store = board[i];
							board[i]= ' '; //Blanks out space in board so spot cannot be reused 	
		    				if(checkForWord(userInput, board, i, 1) == 1)// Calls function to find the whole word
		    					{
		    					wordFound = true; //if found will set word found to true
		    					board[i]=store;
		    					break;
		    					}
		    				board[i] =store;
						}
				}
				
	}
//-----------------------------------------------------------------------------------
//Function to display all possible words on board in proper order
void displayAllWords(char** dictionary, int min, int max, char board[]) 
	{
		bool temp = false;
		for(int x = min; x <= max; x++)
		{
			for(int i=0; i < 263533; i++)
				{
					firstLetterFind(board, dictionary[i], temp);
					if(temp && (strlen(dictionary[i]) == x))
						{
							cout << dictionary[i] << " ";
						}
					temp = false;
				}
			temp = false;
		}
	}//End displayAllWords
//---------------------------------------------------------------------------------------
int main()
{
    // declare variables
    char **dictionary;                // 2d array of dictionary words, dynamically allocated
    long int numberOfWords;           // how many words actually found in dictionary
    char userInput[ MaxUserInputLength]; 
    char board[36]; 
    bool wordsFound[263533]; //Boolean array in order to assist in displaying words found
    int storeTime; //Variable to assist in resuming timer
    bool toggle = false; //Toggle timer varaible
    int score = 0; //Variable to remember score
    //For loop to set default all values on wordFound to false
	for(int i =0; i < 263533; i++)
		{
			wordsFound[i]= false;
		}
    // Declare a variable to hold a time, and get the current time
    time_t startTime = time( NULL);
    displayIdentifyingInformation();
    displayInstructions();
    int x = 60;
    resetBoardRandom(board); //reset board to random characters
    cout << endl << endl;
    cout << endl;
    // read in dictionary.  numberOfWords returns the actual number of words found
    readInDictionary( dictionary, numberOfWords);
    cout << endl << endl;
    // Loop to while there is still time left
	int elapsedSeconds = 0;
    int count = 1;
    while( elapsedSeconds < TotalSecondsToPlay) {
    	if (toggle == false)
    		cout << "   " << x << " Seconds Remaining" << endl;
    	displayBoard(board);
    	 cout << endl << "     Score: " << score << endl;
        // Prompt for and get user input
        cout << count << ".   Enter a word: ";
        count++;
        cin >> userInput;
        if((userInput[0] == 's' || userInput[0] == 'S') && strlen(userInput)==1) //Checks to see if user wished to display all possible words that can be found
        	{
        		cout << "Enter min and max word lengths to display: ";
        		int min;
        		int max;
        		cin >> min;
        		cin >> max;
        		displayAllWords(dictionary, min, max, board);
        		cout << endl << "Exiting the program.";
        		return 0;
			}
		if((userInput[0] == 't' || userInput[0] == 'T') && strlen(userInput)==1) //Option to toggle timer on and off
			{
				if(toggle == false)
				{
				storeTime = difftime( time( NULL), startTime);
				cout << "Timer has been shut off" << endl << endl;;
				elapsedSeconds = 0;
				toggle = true;
				}
				else{
					startTime = time(NULL) - storeTime; //Restores timer to value that it was left at
					toggle = false;
					cout << "Timer is back on" << endl << endl;
				}
				continue;
			}
        if((userInput[0] == 'r' || userInput[0] == 'R') && strlen(userInput)==1) //Option to reset board based of the user input
        	{
        		cout << endl << "Enter 16 characters to be used to set the board: " << endl;
        		resetBoard(board);
        		elapsedSeconds = 0;
        		cout << endl << endl;
        		count = 1;
        		score = 0; 
        		continue;
			}
		bool wordFound = false; //Variable to assist in find if word was found
		firstLetterFind(board, userInput, wordFound);
        if( binarySearch( userInput, dictionary) != -1 &&
			 wordFound && 
			 wordsFound[binarySearch(userInput,dictionary)] == false) 
		{
        	wordsFound[binarySearch(userInput,dictionary)] = true;	
           	if(strlen(userInput)>2 && strlen(userInput)<5)
           		{
           			cout << "Worth " << strlen(userInput)-2 << " points." << endl;
           			score = score + (strlen(userInput) - 2);  
				}
			else if(strlen(userInput)==5)
				{
					cout << "Worth " << 4 << " points." << endl;
					score = score + 4;	
				}
			else{
				cout << "Worth " << strlen(userInput) << " points." << endl;
				score = score + (strlen(userInput));
			}
        }
        else if(wordsFound[binarySearch(userInput,dictionary)] == true) //Checks if user input word was already found
			{
	        	cout << "Sorry, that word was already previously found." << endl;
	        }
        else if(binarySearch( userInput, dictionary) == -1) //checks to see if userWord is in the dictionary
        	{
        		cout << userInput << " was not found in the dictionary";
			}
		else //Last case to see if word is in the dictinoary
			{
				cout << userInput << " cannot be formed on this board.";
			}
		cout << endl << "Words so far are: ";
		displayWordsFound(wordsFound, dictionary);
		cout << endl;
        // Calculate how many seconds have elapsed since we started the timer.
        elapsedSeconds = difftime( time( NULL), startTime);
        if(toggle == true)
        	elapsedSeconds = 0;
        //if(toggle == false)
        //	continue;
    
    //cout << "There are " << TotalSecondsToPlay - elapsedSeconds << " seconds left." << endl << endl;
    if(toggle == false)
    x= TotalSecondsToPlay - elapsedSeconds;
    }
    cout << "I let you finish your last move. Time is up!" << endl;
    return 0;   
}//end main()
