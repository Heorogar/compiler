#include<string>
#include<fstream>
#include<iostream>
//#include<forward_list>
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
	struct listNode {
		std::string s;

		keywords k;
		listNode *next;
	public:
		listNode(std::string s, keywords k, listNode *next) {

			this->s = s;
			this->k = k;
			this->next = next;
			std::cout << "constructor" << std::endl;
		}
		//add insertion method here
	};
	struct treeNode {
		struct childNode {

			treeNode *child;
			childNode *next;
		};
		childNode *childList;
		listNode *symbolTable;
		std::unordered_set<std::string> stringsTable;
	};
	lexer();
	treeNode* scanFile(std::string inputFilePath);

	void print(treeNode *n) {
		if (n == NULL)
			return;
		listNode *temp = n->symbolTable;
		while (temp != NULL) {

//			std::cout << i.k << std::endl;
			std::cout << temp->s;
			temp = temp->next;
		}
		std::cout << std::endl;
		print(n->childList->child);
	}

private:
	std::ifstream* addIdNode(std::ifstream *inputStream, treeNode **root,
			char inputChar, listNode **temp);
	treeNode* buildTree(std::ifstream *inputStream, treeNode *root);
	listNode* insert(listNode *head, treeNode *root, std::string s, keywords k);
}
;

}
