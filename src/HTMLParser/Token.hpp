#include <string>
#include <vector>
#include "../Util/key_value.h"

#ifndef Token_hpp
#define Token_hpp

enum Token_type {
    DOCTYPE,
    start_tag,
    end_tag,
    comment,
    character,
    eof
};

class Token {

    private:
        Token_type _type;
        std::string _buffer;

    public:
        Token(Token_type type, std::string buffer);

    bool validate();
    Token_type type();
    std::string buffer();

};

class Doctype_Token : Token {
    private:
        std::string _name; // Defaults to "missing" which is distinct from ""
        std::string _public_identifier; // Defaults to "missing" which is distinct from ""
        std::string _system_identifier; // Defaults to "missing" which is distinct from ""
        bool _force_quirks_flag; // Defaults to false

    public:
        Doctype_Token(std::string buffer);

        std::string name();
        std::string public_identifier();
        std::string system_identifier();
        bool force_quirks_flag();
};

class Start_Tag_Token : Token {
    private:
        std::string _tag_name;
        bool _self_closing; // defaults to false
        std::vector<key_value*> _atrributes;

    public:
        Start_Tag_Token(std::string buffer);

        std::string tag_name();
        bool self_closing();
        std::vector<key_value*> attribute();
};

class End_Tag_Token : Token {
    private:
            std::string _tag_name;
            bool _self_closing; // defaults to false
            std::vector<key_value*> _atrributes;

    public:
        End_Tag_Token(std::string buffer);

        std::string tag_name();
        bool self_closing();
        std::vector<key_value*> attribute();
};

class Comment_Token : Token {
    private:
        std::string _data;

    public:
        Comment_Token(std::string buffer);

        std::string data();
};

class Character_Token : Token {
    private:
            std::string _data;

    public:
        Character_Token(std::string buffer);

        std::string data();
};

class End_of_File_Token : Token {
    public:
        End_of_File_Token();
};



#endif
