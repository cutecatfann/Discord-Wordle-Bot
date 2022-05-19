#ifndef GAME_H_
#define GAME_H_

#include <unordered_set>

typedef std::unordered_set<std::string> Dictionary;

std::unique_ptr<Dictionary> ReadDictionary();

struct GameState {
  std::string word;
  std::vector<std::string> guesses;
};

// Creates a game for a user
void InitGame(std::unordered_map<dpp::snowflake, GameState> &games,const dpp::snowflake &user,const std::unique_ptr<Dictionary> &dictionary);


std::string Guess(dpp::snowflake user, std::string guess, std::unordered_map<dpp::snowflake, GameState> &games,const std::unique_ptr<Dictionary> &dictionary);

void visualDepiction(GameState &game);

void winnerGame(dpp::snowflake user, GameState &games, std::string guess);

#endif
