#include <dpp/dpp.h>
#include <iostream>
#include <random>
#include <utility>
#include <unordered_set>
#include <sstream>
#include <vector>
#include "game.h"
#include <cctype>
#include <string>

using namespace std;

unique_ptr<Dictionary> ReadDictionary(){
    unordered_set <string> stringSet; 
    ifstream input_file("5LetterWords.txt");

    for (string line; getline(input_file, line);) {
        string x;
        stringstream ss(line);
        ss >> x;
        stringSet.insert(x);
    }

    return make_unique <Dictionary> (stringSet);
}

void InitGame(std::unordered_map<dpp::snowflake, GameState> &games,const dpp::snowflake &user,const std::unique_ptr<Dictionary> &dictionary){
    GameState g;

    // pick a random word from the dictionary
    std::mt19937 gen(std::random_device{}());
    std::string element;
    std::sample(dictionary->begin(), dictionary->end(), &element, 1, gen);

    // update the word field of GameState with random word
    g.word = element;
    games[user] = g;
    cout << element << " is the random word" << endl;
}

string Guess(dpp::snowflake user, string guess, std::unordered_map<dpp::snowflake, GameState> &games, const unique_ptr<Dictionary> &dictionary){
    GameState g;
    string winner;

    if (games.find(user) != games.end()){
        g = games.at(user);
    }
    string word = g.word;
    if (guess.size() == 5){
        
        if(dictionary->find(guess) != dictionary->end()){
            for (int j = 0; j < 5; j++){
                if(islower(guess[j])){ // check to see if lowercase letter
                    if(guess[j] == word[j]){
                        winner.append(":green_square: "); 
                    } else if (word.find(guess[j]) != std::string::npos){ // find if letter in word
                        winner.append(":yellow_square: ");
                    } else{
                        winner.append(":red_square: ");
                    }
                } else { // change uppercase letter to lowercase letter
                    char c = tolower(guess[j]);
                    if(c == word[j]){
                        winner.append(":green_square: ");
                    } else if (word.find(guess[j]) != std::string::npos){ // find if letter in word
                        winner.append(":yellow_square: ");
                    } else{
                        winner.append(":red_square: ");
                    }
                }
            }   
        } else {
            winner = "The word was not in the dictionary :disappointed: Please enter a new word: ";
        }
    } else {
        winner = "Sorry, your word was not the right length :disappointed: Please enter a new word: ";
        
    }

    return winner;
}
