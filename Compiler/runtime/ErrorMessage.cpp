/*
 * This file is part of OpenModelica.
 *
 * Copyright (c) 1998-2010, Link�pings University,
 * Department of Computer and Information Science,
 * SE-58183 Link�ping, Sweden.
 *
 * All rights reserved.
 *
 * THIS PROGRAM IS PROVIDED UNDER THE TERMS OF THIS OSMC PUBLIC
 * LICENSE (OSMC-PL). ANY USE, REPRODUCTION OR DISTRIBUTION OF
 * THIS PROGRAM CONSTITUTES RECIPIENT'S ACCEPTANCE OF THE OSMC
 * PUBLIC LICENSE.
 *
 * The OpenModelica software and the Open Source Modelica
 * Consortium (OSMC) Public License (OSMC-PL) are obtained
 * from Link�pings University, either from the above address,
 * from the URL: http://www.ida.liu.se/projects/OpenModelica
 * and in the OpenModelica distribution.
 *
 * This program is distributed  WITHOUT ANY WARRANTY; without
 * even the implied warranty of  MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH
 * IN THE BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS
 * OF OSMC-PL.
 *
 * See the full OSMC Public License conditions for more details.
 *
 */

#include <list>
#include <string>
#include <sstream>
#include <iostream>

#include "ErrorMessage.hpp"

  /* Implementation of ErrorMessage class. */


  ErrorMessage::ErrorMessage(long errorID,
           ErrorType type,
           ErrorLevel severity,
           const std::string &message,
           const TokenList &tokens)
    : errorID_(errorID),
      messageType_(type),
      severity_(severity),
      message_(message),
      tokens_(tokens)

{
  startLineNo_ = 0;
  startColumnNo_ = 0;
  endLineNo_ = 0;
  endColumnNo_ = 0;
  isReadOnly_ = false;
  filename_ = std::string("");
  shortMessage = getMessage_();
  fullMessage = getFullMessage_();
}

ErrorMessage::ErrorMessage(long errorID,
         ErrorType type,
         ErrorLevel severity,
         const std::string &message,
         const TokenList &tokens,
         long startLineNo,
         long startColumnNo,
         long endLineNo,
         long endColumnNo,
         bool isReadOnly,
         const std::string &filename)
    :
    errorID_(errorID),
    messageType_(type),
    severity_(severity),
    startLineNo_(startLineNo),
    startColumnNo_(startColumnNo),
    endLineNo_(endLineNo),
    endColumnNo_(endColumnNo),
    isReadOnly_(isReadOnly),
    filename_(filename),
    message_(message),
    tokens_(tokens)
{
  shortMessage = getMessage_();
  fullMessage = getFullMessage_();
}

std::string ErrorMessage::getMessage_()
{
  std::string::size_type str_pos = 0;
  TokenList::iterator tok = tokens_.begin();
  char index_symbol;
  int index;

  while((str_pos = message_.find('%', str_pos)) != std::string::npos)
  {
    index_symbol = message_[str_pos + 1];

    if(index_symbol == 's')
    {
      if(tok == tokens_.end())
      {
        std::cerr << "Internal error: no tokens left to replace %s with.\n";
        std::cerr << "Given message was: " << message_ << "\n";
        return "";
      }
      message_.replace(str_pos, 2, *tok);
      str_pos += tok->size() + 1; 
      *tok++;
    }
    else if(index_symbol >= '0' || index_symbol <= '9')
    {
      index = index_symbol - '0' - 1;

      if(index >= tokens_.size() || index < 0)
      {
        std::cerr << "Internal error: Invalid positional index %" << index + 1 
          << " in error message.\n";
        std::cerr << "Given message was: " << message_ << "\n";
        return "";
      }

      message_.replace(str_pos, 2, tokens_[index]);
      str_pos += tokens_[index].size() + 1;
    }
  }

  std::string ret_msg;

  if(filename_ == "" && startLineNo_ == 0 && startColumnNo_ == 0 &&
      endLineNo_ == 0 && endColumnNo_ == 0)
  {
    ret_msg = ErrorLevel_toStr[severity_] + (": " + message_);
  }
  else
  {
    std::stringstream str;
    str << "[" << filename_ << ":" << startLineNo_ << ":" << startColumnNo_ <<
      "-" << endLineNo_ << ":" << endColumnNo_ << ":" <<
      (isReadOnly_ ? "readonly" : "writable") << "] " << ErrorLevel_toStr[severity_] << ": ";
    std::string positionInfo = str.str();
    ret_msg = positionInfo + message_;
  }

  return ret_msg;
}

std::string ErrorMessage::getFullMessage_()
{
  std::stringstream strbuf;

  strbuf << "{\"" << shortMessage << "\", \"" <<
    ErrorType_toStr[messageType_] << "\", \"" <<
    ErrorLevel_toStr[severity_] << "\", \"" <<
    errorID_ <<  "\"}";

  return strbuf.str();
}
