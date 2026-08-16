#include <string>
#include <memory>

#include "Token.hpp"

#ifndef Tokenizer_hpp
#define Tokenizer_hpp

enum Tokenizer_State {
    data_state,
    character_reference_in_data_state,
    rcdata_state,
    character_reference_in_rcdata_state,
    rawtext_state,
    script_data_state,
    plaintext_state,
    tag_open_state,
    end_tag_open_state,
    tag_name_state,
    rcdata_less_than_sign_state,
    rcdata_end_tag_open_state,
    rcdata_end_tag_name_state,
    rawtext_less_than_sign_state,
    rawtext_end_tag_open_state,
    rawtext_end_tag_name_state,
    script_data_less_than_sign_state,
    script_data_end_tag_open_state,
    script_data_end_tag_name_state,
    script_data_escape_start_state,
    script_data_escape_start_dash_state,
    script_data_escaped_state,
    script_data_escaped_dash_state,
    script_data_escaped_dash_dash_state,
    script_data_escaped_less_than_sign_state,
    script_data_escaped_end_tag_open_state,
    script_data_escaped_end_tag_name_state,
    script_data_double_escape_start_state,
    script_data_double_escaped_state,
    script_data_double_escaped_dash_state,
    script_data_double_escaped_dash_dash_state,
    script_data_double_escaped_less_than_sign_state,
    script_data_double_escape_end_state,
    before_attribute_name_state,
    attribute_name_state,
    after_attribute_name_state,
    before_attribute_value_state,
    attribute_value_double_quoted_state,
    attribute_value_single_quoted_state,
    attribute_value_unquoted_state,
    character_reference_in_attribute_value_state,
    after_attribute_value_quoted_state,
    self_closing_start_tag_state,
    bogus_comment_state,
    markup_declaration_open_state,
    comment_start_state,
    comment_start_dash_state,
    comment_state,
    comment_end_dash_state,
    comment_end_state,
    comment_end_bang_state,
    doctype_state,
    before_doctype_name_state,
    doctype_name_state,
    after_doctype_name_state,
    after_doctype_public_keyword_state,
    before_doctype_public_identifier_state,
    doctype_public_identifier_double_quoted_state,
    doctype_public_identifier_single_quoted_state,
    after_doctype_public_identifier_state,
    between_doctype_public_and_system_identifiers_state,
    after_doctype_system_keyword_state,
    before_doctype_system_identifier_state,
    doctype_system_identifier_double_quoted_state,
    doctype_system_identifier_single_quoted_state,
    after_doctype_system_identifier_state,
    bogus_doctype_state,
    cdata_section_state,
};

class Tokenizer {
    private:
        std::string::iterator current;

        Tokenizer_State _state;

        std::unique_ptr<Token> _emitted_token;
        bool _token_emitted;

        void data_state();
        void character_reference_in_data_state();
        void rcdata_state();
        void character_reference_in_rcdata_state();
        void rawtext_state();
        void script_data_state();
        void plaintext_state();
        void tag_open_state();
        void end_tag_open_state();
        void tag_name_state();
        void rcdata_less_than_sign_state();
        void rcdata_end_tag_open_state();
        void rcdata_end_tag_name_state();
        void rawtext_less_than_sign_state();
        void rawtext_end_tag_open_state();
        void rawtext_end_tag_name_state();
        void script_data_less_than_sign_state();
        void script_data_end_tag_open_state();
        void script_data_end_tag_name_state();
        void script_data_escape_start_state();
        void script_data_escape_start_dash_state();
        void script_data_escaped_state();
        void script_data_escaped_dash_state();
        void script_data_escaped_dash_dash_state();
        void script_data_escaped_less_than_sign_state();
        void script_data_escaped_end_tag_open_state();
        void script_data_escaped_end_tag_name_state();
        void script_data_double_escape_start_state();
        void script_data_double_escaped_state();
        void script_data_double_escaped_dash_state();
        void script_data_double_escaped_dash_dash_state();
        void script_data_double_escaped_less_than_sign_state();
        void script_data_double_escape_end_state();
        void before_attribute_name_state();
        void attribute_name_state();
        void after_attribute_name_state();
        void before_attribute_value_state();
        void attribute_value_double_quoted_state();
        void attribute_value_single_quoted_state();
        void attribute_value_unquoted_state();
        void character_reference_in_attribute_value_state();
        void after_attribute_value_quoted_state();
        void self_closing_start_tag_state();
        void bogus_comment_state();
        void markup_declaration_open_state();
        void comment_start_state();
        void comment_start_dash_state();
        void comment_state();
        void comment_end_dash_state();
        void comment_end_state();
        void comment_end_bang_state();
        void doctype_state();
        void before_doctype_name_state();
        void doctype_name_state();
        void after_doctype_name_state();
        void after_doctype_public_keyword_state();
        void before_doctype_public_identifier_state();
        void doctype_public_identifier_double_quoted_state();
        void doctype_public_identifier_single_quoted_state();
        void after_doctype_public_identifier_state();
        void between_doctype_public_and_system_identifiers_state();
        void after_doctype_system_keyword_state();
        void before_doctype_system_identifier_state();
        void doctype_system_identifier_double_quoted_state();
        void doctype_system_identifier_single_quoted_state();
        void after_doctype_system_identifier_state();
        void bogus_doctype_state();
        void cdata_section_state();

    public:

    Tokenizer(std::string source_string);

    void iterate();

    Tokenizer_State state();

    std::unique_ptr<Token> emitted_token();
    bool token_emitted();

};


#endif
