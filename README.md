# Stack Calculator
This projects makes use of ***Stack*** data structure as well as infix and postfix expression structure in order to calculate **single-digit operand** experessions.

* The program can handle the main operands `(^, *, /, %, +, -)` and works with any size expression.
* Groupings can also be made with parentheses/braces/brackets `() {} []` and also they can be nested `{()+[]} - ()`. The program will check the balance of the parentheses to ensure that they are valid.

___

In its current state, the program cannot handle: 
* double-digit number operations of any form, such as `(24-3)`, or
* operations with negative numbers, such as `(-2+8)`. The latter will be flagged as an error in the calculator.

Such fuctionality will be worked on and added in the future.

