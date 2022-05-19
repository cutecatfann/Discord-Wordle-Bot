#include <dpp/dpp.h>
#include <iostream>
#include <random>
#include <utility>
#include <unordered_set>
#include <vector>

#include "game.h"

using namespace std;

int main(){
    // stores user values in struct defined in game.h
    unique_ptr<Dictionary> dictionary = make_unique<Dictionary>();
    unordered_map<dpp::snowflake, GameState> games;

    // command to run: export WORDLE_TOKEN="$(cat secret)"
    const char *token = getenv("WORDLE_TOKEN");
    if (!token) {
        cout << "Empty token." << endl;
        return 1;
    }

    dpp::cluster bot(token); 
    bot.on_ready([&bot](const auto & event) {
        cout << "Logged in as " << bot.me.username << "!\n";
    });

    dictionary = ReadDictionary();

    bot.on_message_create([&bot, &games, &dictionary](const auto& event) {
        const dpp::snowflake user = event.msg.author.id;
        if(event.msg.author.username  ==  "wordle-cutecatfann"){ return; }

        if (event.msg.content == "!play") {
            dictionary = ReadDictionary();
            bot.message_create(dpp::message(event.msg.channel_id, "Welcome to Wordle Bot! The game has now begun... Will you suceed? Type your guesses, or !quit to end the game. "));
            bot.message_create(dpp::message(event.msg.channel_id, "Key: :green_square: = you got the letter, :yellow_square: = it's in the word but wrong position, :red_square: = not in the word"));
            bot.message_create(dpp::message(event.msg.channel_id, "Enter a real word that is five letters long: "));
            InitGame(games, user, dictionary);
            
        } else if (event.msg.content == "!quit"){
            bot.message_create(dpp::message(event.msg.channel_id, "Sorry to see you go, come back soon!"));
            games.erase(user);
            
        } else {
            string guess = event.msg.content;
            string winner = Guess(user, guess, games, dictionary);
            games[user].guesses.push_back(winner);
            
            if(winner.find("word") != std::string::npos){
                bot.message_create(dpp::message(event.msg.channel_id, winner));
                games[user].guesses.pop_back();
            }

            if(games[user].guesses.size() == 6){
                for (int j = 0; j < games[user].guesses.size(); j++){
                    bot.message_create(dpp::message(event.msg.channel_id, games[user].guesses[j]));
                }
                bot.message_create(dpp::message(event.msg.channel_id, "Oh no! You lost! :smirk_cat: The correct word was"));
                bot.message_create(dpp::message(event.msg.channel_id, games[user].word));
                bot.message_create(dpp::message(event.msg.channel_id, "Better luck next time! If you would like to play again, type !play"));

                games.erase(user);

            } else {
                for (int j = 0; j < games[user].guesses.size(); j++){
                    bot.message_create(dpp::message(event.msg.channel_id, games[user].guesses[j]));
                }

                int n = count(winner.begin(), winner.end(), 'g');
                if(n == 5){
                   bot.message_create(dpp::message(event.msg.channel_id, "You won! Good job! :star_struck: :clap:")); 
                   bot.message_create(dpp::message(event.msg.channel_id, "To play again, just type !play "));
                   games.erase(user);
                }   
            }
        }

    });

    bot.start(false);
    return 0;
}
