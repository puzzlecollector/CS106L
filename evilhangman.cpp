/* 
Evil Hangman Program 
by Luke Minsuk Kim 
Sorry for the messiness; did not have too much time to work on it. 
But the functionality is good. 
To breifly explain the algorithm: 
1. Read dictionary.txt 
2. Prompt the user for word length 
3. Prompt user for nubmer of guesses  
4. Prompt user if he/she wants a running total of the number of words remaining in the word list. 
5. Construct a list of all words in the English language whose length matches the input length.  
6. Proceed with evil hangman algorithm described in spec (i.e. parititioning and selecting the largest group)
7. Check if the user wants to play the game again. 
*/ 
#include <iostream> 
#include <cstdlib> 
#include <algorithm> 
#include <vector> 
#include <map> 
#include <stack> 
#include <queue> 
#include <numeric> 
#include <utility> 
#include <cstring> 
#include <string> 
#include <sstream> 
#include <cctype> 
#include <fstream> 
using namespace std;  

// global array. The program is small enough so it's not so confusing 
// to declare a global array.  
// it is given in the spec that the maximum word length is 29.  
bool lengthExists[30]; 

// function definitions 
bool chkDone(string &s){
	for (int i = 0; i < s.size(); i++){
		if (s[i] == '-') return false;  
	}
	return true;  
}

bool chkIfNumber(string &s){
	for (int i = 0; i < s.size(); i++){
		if (!(s[i] >= '0' && s[i] <= '9')) return false;  
	}
	return true;  
}

int toInt(string &s){
	istringstream iss (s);  
	int n; 
	iss >> n; 
	return n;  
}

void getMasterList(vector<string> &MasterList){
	ifstream fin("dictionary.txt");   
	string word;  
	while (fin >> word){
		MasterList.push_back(word);  
		lengthExists[(int)word.size()] = true;  
	}
}

void getLength(int &len){
	while (true){
		cout << "Please enter the length of the word: ";  
		cin >> len; 
		if (len >= 1 && len <= 29 && lengthExists[len]){
			return; 
		}else{
			cout << "sorry, invalid word length." << endl; 
		}
	}
}

void getGuesses(string &guesses){
	while (true){
		cout << "Enter the number of guesses you want to make." << endl;
		cout << "Of course, having >= 26 guesses means that you are a cheater" << endl; 
		cout << "and you winning does not actually mean that you won!! HAHAHAHAHA" << endl;   
		cin >> guesses;  
		if (!chkIfNumber(guesses) || toInt(guesses) <= 0){
			cout << "You must enter a positive integer" << endl; 
		}else{
			return; 
		}
	}
}

void promptKeepRunningTotal(bool &keep_running_total){
	cout << "Do you want to keep the running total of the number of words remaining in the word list?" << endl;
	cout << "Please enter \"yes\" or \"no\": ";  
	string response; 
	cin >> response;  
	while (response != "yes" && response != "no"){
		cout << "invalid response" << endl; 
		cout << "Please enter \"yes\" or \"no\": "; 
		cin >> response;  
	}
	if (response == "yes"){
		keep_running_total = true;  
	}
}

void constructCollection(vector<string> &MasterList,vector<string> &collection,int len){
	for (int i = 0; i < MasterList.size(); i++){
		if (MasterList[i].size() == len){
			collection.push_back(MasterList[i]);  
		}	
	}
}

bool playAgain(){
	cout << "Do you want to play again?" << endl; 
	cout << "Please enter \"yes\" or \"no\": ";  
	string response; 
	cin >> response; 
	while (response != "yes" && response != "no"){
		cout << "invalid response" << endl; 
		cout << "Please enter \"yes\" or \"no\": ";  
		cin >> response;  
	}
	return (response == "yes");  
}

// main function 
int main(){
	ios_base::sync_with_stdio(false); 
	cin.tie(NULL);  
	vector<string> MasterList;  // the list that contains all the words in dictionary.txt 
	getMasterList(MasterList);  
	while (true){
		int len; // the length of the word the user selected. 
		getLength(len);  
		string guesses; // get the input for guesses as a string. 
		getGuesses(guesses);  
		int numGuesses = toInt(guesses);  // convert this quantity to integer. 
		bool keep_running_total = false;  // checking if the user wants to keep the running total or not. 
		promptKeepRunningTotal(keep_running_total);  
	// the Evil Hangmang Algorithm 
		vector<string> collection;  
		constructCollection(MasterList,collection,len); // construct a list of all words whose length matches the input length.
	// print out how many guesses the user has remaining 
	// print out any letters the player has guessed and the current blanked out version of the word.  
	// if the user chose to see the number of words remaining, print that out too.  
		bool flag = true;  
		vector<char> guessList;  
		string status = string(len,'-');  
		for (int i = numGuesses; i >= 1; i--){
			if (keep_running_total){
				cout << "number of words remaining in list: " << collection.size() << endl; 
			}
			cout << "number of remaining guesses:" << i << endl; 
			if (!guessList.empty()){
				cout << "guessed " << guessList.size() << " letters: " << endl; 
				for (int i = 0; i < guessList.size(); i++){
					cout << guessList[i] << " ";  
				}
				cout << endl; 
			}
			cout << status << endl; 
			cout << "Enter a single letter to guess: ";  
			string input;  
			cin >> input;  
			char c; 
			c = input[0];  
			while (!isalpha(c) || input.size() > 1){
				cout << "invalid response. Please enter a single character: ";  
				cin >> input; 
				c = input[0];  
			}
			guessList.push_back(c);  
			map<string,vector<string> > mp;  
			for (int j = 0; j < collection.size(); j++){
				string temp = status;  
				for (int k = 0; k < collection[j].size(); k++){
					if (collection[j][k] == c){
						temp[k] = c;  
					}
				}
				mp[temp].push_back(collection[j]);  
			}
			int maxSize = 0;  
			string temp;  
			for (map<string,vector<string> >::iterator it = mp.begin(); it != mp.end(); it++){
				if (it->second.size() > maxSize){
					temp = it->first;  
					maxSize = it->second.size();  
				} 
			}
			status = temp;  
			collection = mp[temp];
			if (chkDone(status)){
				cout << status << endl; 
				cout << "you won! wow congrats" << endl;
				flag = false;  
				break; 
			}
			if (i == 1){
				cout << status << endl; 
			}
		}	
		if (flag){
			cout << "you ran out of guesses." << endl;
			cout << "the correct answer is " << collection[rand()%(int)collection.size()] << endl; 
		}
		if (!playAgain()) break;  
	}
	cout << "Thanks for playing! Please play with me again next time!" << endl; 
	return 0;  
}
