# PERSEUS COMPILER

## LEXER

Parts:
1. Lexer.h
2. Lexer.cpp

Lexer.h is the header file and contains declarations.
Lexer.cpp is the implementation file and contains the definitions.

First some lexer theory. It's purpose is to generate a structure called the symbol table based on the input(text file).
The symbol table contains tokens or lexemes which are syntactic parts of an expression. E.g. int a=5; The token are int, a, =, 5, ;. 
Along with those tokens the symbol table must contain their type, such as variable, specific operator etc.

My conceptual design of the symbol table is as follows:
The main structure is a tree with a node representing a scope i.e. code between two scope resolution operators ({ and }). A node can contain n children.
Each node contains a list the nodes of which are lexemes as well as a hashed set with every identifier appearing in that scope.

On to the code!

Everything is encapsulated inside the lexer class.
Token types are represented by a scoped enum called keywords.
The tree node struct besides the hashed set and the list of lexemes(symbolTableNode) contains a pointer to a list of its children. The nodes of that list are implemented as a struct with a pointer to the next list node and a tree node child.
The main access point to the tree is the tree node pointer lexerRootNode which points to the root of the tree. It is built from the constructor by calling the method scan file which opens the file in the path supplied to the constructor and calls the method buildTree.
Now buildTree is the core of the entire lexer. It scans the input file char by char and based on that char instantiates an object from a hierarchy of classes meant to encapsulate all the different algorithms for inserting the correct lexeme in the tree and thereby trying to follow the open-closed principle (open for extension closed for modification). If a new scope is found it adds a new node at the right place recursively.
The print method is there to test if the lexer has done its job.

