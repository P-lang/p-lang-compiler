##Plang Specification

#### Extension: 

.plg

###Token Rules

```
# Plang File Example

VAR file

WHILE file IS NOT empty
    DO PrintLine
```

 - Comments are single line
    - Start with #
    - Entire line becomes comment
    
 - Variables are initiated with `VAR` keyword
    - Must be first token on line
        - Allowed: `VAR myVar = 55`
        - Not Allowed: `IF file IS NOT empty VAR myVar = 55`
    - `VAR` must be followed by 'WORD' token, which will be the variable name
        - Does require space before word
            - Allowed: `VAR myVar`
            - Not Allowed: `VARmyVar`
    - 'WORD' can optionally be followed by `=`
        - Required to have spaces on both sides
        - Must be followed by either a 'WORD' or a 'NUMBER'
        
    
###Lexer Logic

#####Character Categories

Category | Characters
---------|-----------
ALPHA|`a-z`, `A-Z`
DIGIT|`0-9`
OPERATOR|`+`, `-`, `*`, `/`, `<`, `>`, `=`, `!`
SPACE|` `, `\n`, `\r`, `\t`
COMMENT_INDICATOR|`#` 

#####State flow

The state is the state the system is currently in. If the current scanned char is in the state terminator category, the tokens are made accordingly, and the machine is set into the target state.

In the case of the terminator being an OPERATOR, the next character may be checked to determine the correct token, such as in the event of `+=` or `<=`. 
The currentIndex will be incremented one extra unit to compensate for this additional token char, as to skip the next char from the scan.

State | State Terminator | Target State
----|-----|-----
Normal|ALPHA|Word
Normal|DIGIT|Number
Normal|COMMENT_INDICATOR|Comment
Normal|SPACE|-
Word|SPACE|Normal
Word|COMMENT_INDICATOR|Comment
Word|OPERATOR|Normal
Number|SPACE|Normal
Number|COMMENT_INDICATOR|Comment
Number|OPERATOR|Normal
Comment|SPACE_END_OF_LINE|Normal

###Parser

#####Context Free Grammar

Name | Rule
---- | ----
START | VAR_DEC
VAR_DEC | word word = VALUE
VALUE | word / number
 