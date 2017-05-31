#include <iostream>
#include "RedBlackTree.h"

using namespace std;

void main()
{
    RedBlackTree a;

    a.Insert(15, "Conan the Destroyer");
    a.Insert(5, "Red Sonja");
    a.Insert(1, "Commando");
    a.Insert(90, "Raw Deal");
    a.Insert(47, "Predator");
    a.Insert(55, "The Running Man");
    a.Insert(57, "The Terminator");
    a.Insert(58, "Terminator 2: Judgment Day");
    a.Insert(59, "Last Action Hero");
    a.Insert(11, "True Lies");
    //a.Insert(0, "Batman and Robin");

    string searchResult(a.Search(57));
    
    cout << endl << searchResult << endl << endl;

    a.Delete(5);
    a.Delete(57);
    a.Delete(58);

    cout << "end" << endl;
}