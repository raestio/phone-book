# Phone-Book
A program adds new contacts and effectively search for contacts via ternary search tree.

Works interactively, each line on the command line represents one processing instruction. Instruction 
is either adding a new contact or to search for contacts. Giving instructions
ends after reading entire input (reaching EOF).
 
 Commands:
      
      + phoneNumber name
 
             - This command adds contact to phone book.
             - [phoneNumber] : sequence of characters 0-9
             
                             : can not be emty
             - [name]        : sequence of uppercase and lowercase letters and spaces
                             : can not be empty and can not begin nor end with space
             - Character '+', number and name on the input line are separated by just one space.
             - Name and number are not limited.
 
      ? prefix[name||phoneNumber]
  
             - This command searches for contacts either by name or number.
             - We do not have to enter full name or number, we can simply enter its prefix
               and the program searches all possible contacts that match with this prefix and prints them.
             - [name] : sequence of uppercase and lowercase letters and spaces
             - [phoneNumber] : sequence of characters 0-9
             - Character '?' and prefix on the input line are separated by just one space.
             - Empty prefix = print entire phone book.
 
 
 Input in EBNF language:
 
      input        ::= { command '\n' }
      command      ::= '+' ' ' phoneNumber ' ' name 
                     | '?' ' ' phoneNumber
                     | '?' ' ' name
                     | '?' ' '
      phoneNumber  ::= digit { digit }
      name         ::= letter { letter | ' ' } letter 
                     | letter
      digit        ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
      letter       ::= 'A' | 'B' | ... | 'Z' | 'a' | 'b' | ... | 'z'
