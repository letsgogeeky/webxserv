/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenization.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgabler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 16:06:22 by fgabler           #+#    #+#             */
/*   Updated: 2024/09/09 15:51:02 by fgabler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZATION_HPP
#define TOKENIZATION_HPP

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "NodeToken.hpp"
#include "TypeToken.hpp"
#include "fstream"

class Tokenization {
 public:
  Tokenization(std::ifstream &file);
  Tokenization() = delete;
  Tokenization(const Tokenization &) = delete;
  Tokenization &operator=(const Tokenization &) = delete;
  std::vector<TokenNode> getTokens() const;
  void printTokens();

 private:
  void loadInvalidContextsAndDirectives();
  void openFile(const std::string &filePath, std::ifstream &file);
  void removeCommentsFromLine();
  void separateTokenStringsFromLine();
  void lineNumberAddToLineOfTokens(unsigned int currentLineNumber);
  void identifyTokenLineTypes();
  void checkAndSetSemikolonInToken();
  void removeSemikolonFromToken();
  void bracketIdentification();
  void contextIdentification();
  bool isInvalidContext(const std::string &context);
  bool isInvalidDirective(const std::string &directive);
  void directiveIdentification();
  void valueIdentification();
  bool validDirective(std::string &directive);
  void locationUrlIdentification();
  void addToLineTokensToTokenChain();
  void clearTokenLine();

  std::string _line;
  std::vector<std::unique_ptr<TokenNode> > _tokensFromLine;
  std::vector<std::unique_ptr<TokenNode> > _chainOfTokens;
  std::set<std::string> _invalidContext;
  std::set<std::string> _invalidDirective;
  std::set<std::string> _directiveValid;
};

#endif