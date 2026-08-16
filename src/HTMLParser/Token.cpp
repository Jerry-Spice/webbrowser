#include <string>
#include <vector>
#include "../Util/key_value.h"
#include "Token.hpp"

// ---------- Token ----------

Token::Token(Token_type type, std::string buffer) {
    this->_type = type;
    this->_buffer = buffer;
}

bool Token::validate() {
    return false;
}

Token_type Token::type() {
    return this->_type;
}

std::string Token::buffer() {
    return this->_buffer;
}

// ---------- Doctype Token ----------

Doctype_Token::Doctype_Token(std::string buffer) : Token(DOCTYPE, buffer) {
    this->_name = "missing";
    this->_public_identifier = "missing";
    this->_system_identifier = "missing";
    this->_force_quirks_flag = false;
}

bool Doctype_Token::validate() {
    return false;
}

std::string Doctype_Token::name() {
    return this->_name;
}

std::string Doctype_Token::public_identifier() {
    return this->_public_identifier;
}

std::string Doctype_Token::system_identifier() {
    return this->_system_identifier;
}

bool Doctype_Token::force_quirks_flag() {
    return this->_force_quirks_flag;
}

// ---------- Start Tag Token ----------

Start_Tag_Token::Start_Tag_Token(std::string buffer) : Token(start_tag, buffer) {
    this->_tag_name = "missing";
    this->_self_closing = false;
    // Attributes are automatically initialized
}

Start_Tag_Token::~Start_Tag_Token() {
    for (std::vector<key_value*>::iterator it = this->_attributes.end(); it != this->_attributes.begin(); it--) {
        free(*it);
    }
    this->_attributes.clear();
}

bool Start_Tag_Token::validate() {
    return false;
}

std::string Start_Tag_Token::tag_name() {
    return this->_tag_name;
}

bool Start_Tag_Token::self_closing() {
    return this->_self_closing;
}

std::vector<key_value*> Start_Tag_Token::attributes() {
    return this->_attributes;
}

// ---------- End Tag Token ----------

End_Tag_Token::End_Tag_Token(std::string buffer) : Token(end_tag, buffer) {
    this->_tag_name = "missing";
    this->_self_closing = false;
    // Attributes are automatically initialized
}

End_Tag_Token::~End_Tag_Token() {
    for (std::vector<key_value*>::iterator it = this->_attributes.end(); it != this->_attributes.begin(); it--) {
        free(*it);
    }
    this->_attributes.clear();
}

bool End_Tag_Token::validate() {
    return false;
}

std::string End_Tag_Token::tag_name() {
    return this->_tag_name;
}

bool End_Tag_Token::self_closing() {
    return this->_self_closing;
}

std::vector<key_value*> End_Tag_Token::attributes() {
    return this->_attributes;
}

// ---------- Comment Token ----------

Comment_Token::Comment_Token(std::string buffer) : Token(comment, buffer) {
    this->_data = buffer;
}

bool Comment_Token::validate() {
    return false;
}

std::string Comment_Token::data() {
    return this->_data;
}

// ---------- Character Token ----------

Character_Token::Character_Token(std::string buffer) : Token(comment, buffer) {
    this->_data = buffer;
}

bool Character_Token::validate() {
    return false;
}

std::string Character_Token::data() {
    return this->_data;
}

// ---------- End of File Token ----------

End_of_File_Token::End_of_File_Token() : Token(eof, "") {
   // intentionally empty
}

bool End_of_File_Token::validate() {
    return true;
}
