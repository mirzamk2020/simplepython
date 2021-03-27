/*main.cpp*/
//
// Author: Mirza MK Baig, UIC, Spring 2021
// A Simple Python Program
// Feburary 26, 2021
// This program is an interpreter for SimplePy. It only supports int and string data types, print() function, int() function,
// type() function, str() function, input() function, and if-elif-else conditions. 
// The SimplePy BNF is shown below and represents some of the features of python
// 
/*
<simplepy>  ::= <stmt>+
<stmt>      ::= <blockstmt>
              | <basestmt>
<blockstmt> ::= <if>
<basestmt>  ::= <funcall>
              | <assign>

<if>        ::= if <mathexpr> ":" <basestmt>* <elif>* <else>?
<elif>      ::= elif <mathexpr> ":" <basestmt>*
<else>      ::= else ":" <basestmt>*

<funcall>   ::= <functname> "(" <params> ")"
<params>    ::= ( <param> ("," <param>)* )?
<param>     ::= <element>

<assign>    ::= <varname> "=" <expr>
<expr>      ::= <funcall>
              | <mathexpr>
<mathexpr>  ::= <element> (<op> <element>)?
<element>   ::= <varname>
              | <literal>

<functname> ::= <identifier>
<varname>   ::= <identifier>
<literal>   ::= <integer>
              | <string>

<identifier> ::= (<letter> | "_") (<letter> | <digit> | "_")*
<integer>    ::= <digit>+
<string>     ::=  "\"" <char>* "\""

<letter> ::= ([a-z] | [A-Z])
<digit>  ::= [0-9]
<char>   ::= (<letter> | <digit> | " " | "," | "." | ...)  // too many to list:

<op> ::= "+"
       | "-"
       | "*"
       | "/"
       | "%"
       | "**"
       | "<"
       | "<="
       | ">"
       | ">="
       | "=="
       | "!="
*/
// 
//
