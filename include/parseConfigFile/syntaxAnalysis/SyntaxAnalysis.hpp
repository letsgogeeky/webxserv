#ifndef CONFIG_FILE_HPP
#define CONFIG_FILE_HPP

#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <memory>

#include "ConfigFile.hpp"
#include "NodeToken.hpp"

enum class StoringStates {
  MAIN_CONTEXT = 0,
  HTTPS_CONTEXT = 1,
  SERVER_CONTEXT = 2,
  LOCATION_CONTEXT = 3
};

enum EBracketStatus {
  MAIN_BRACKET = 0,
  HTTP_BRACKET = 1,
  SERVER_BRACKET = 2,
  LOCATION_BRACKET = 3
};

class SyntaxAnalysis {
 public:
  SyntaxAnalysis() = delete;
  SyntaxAnalysis(const SyntaxAnalysis &) = delete;
  SyntaxAnalysis &operator=(const SyntaxAnalysis &other) = delete;
  SyntaxAnalysis(std::vector<std::unique_ptr<TokenNode> > &token);
  ~SyntaxAnalysis();

  void undefinedTokenCheck();
  void setCurrentState();
  void directiveStateSetSave(const StoringStates &state);
  void moveToNextTokenSave();

  void trackBrackets();
  EBracketStatus getCurrentBracketStatus();

 private:
  ConfigFile _config;
  StoringStates _state;
  bool _tokenIsHandled;
  std::vector<std::unique_ptr<TokenNode>>::iterator _it;

  std::stack<char> _bracketStatus[4];
  std::vector<std::unique_ptr<TokenNode>> _tokens;
};

#endif
