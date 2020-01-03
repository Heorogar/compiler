#include<string>
#include<fstream>
#include<iostream>
#include<map>
#include<unordered_set>
namespace cmplr_ {

class lexer {

	enum class keywords {
		primitive_types,
		arithmetic_operators,
		identifiers,
		reserved_words,
		assignment_operator,
		quote,
		string_literal,
		relational_operators,
		number,
		eol_operator,
		parenthesis
	};

public:
	struct treeNode {
		treeNode *next;
		std::map<std::string, keywords> symbolTable;
		std::unordered_set<std::string> stringsTable;
	};

//	struct listNode {
//		std::string s;
//		keywords k;
//		listNode *next;
//	};

	lexer();
	treeNode* scanFile(std::string inputFilePath);

	void print(treeNode *n) {

		for (std::pair<std::string, keywords> i : n->symbolTable) {
			std::cout << i.first << std::endl;
//			std::cout << i.second << std::endl;
		}
	}

private:
	std::ifstream* addIdNode(std::ifstream *inputStream, treeNode *root,
			char inputChar);
	treeNode* buildTree(std::ifstream *inputStream, treeNode *root);
}
;

}
