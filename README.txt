******SPLIT IMPLEMENTATION****

***INPUT***

any std:istream 

***PARAMETERS***

optional numbers of parameters: 

-R-values (must be chars)
-L-values

rules: 
	two L-values must never be adjacent
	R-value is of type char
	
***FUNCTIONALITY***

function read input and ties variables(L-values) with current text
(see example in Source.cpp)

e.g. "some istream" >> split(x,'=','!',y)
-suppose that x and y are strings
-function stores whole string from input until reach delimiter '=' (error otherwise)
-function reads next character and check if it's '!' (if it's not present raise error)
-function stores rest of the text to y (until EOF)