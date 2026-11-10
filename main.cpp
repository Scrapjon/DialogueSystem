#include <iostream>
#include "dialogue_node.h"
#include <cassert>

int main()
{

    std::shared_ptr<DialogueNode> start = MakeNode("Hello! Welcome to my game!", "Start");

    start
        ->Extend("This is my custom dialogue system, I am testing out how the system works!")
        ->Extend("This implementation looks kinda strange rn but bear with me")
        ->Extend("Do you like it?", {{"Yes", MakeNode("Great! I'm glad you do!")}, {"No", MakeNode("Sleep with one eye open...", "Threaten")}})
        ->GetNext("Yes")
        ->Extend("If you like this so much, would you like to play again?", {{"Yes", GetNode("Start")},
                                                                             {"No", GetNode("Threaten")}});

    std::shared_ptr<DialogueNode> current = start;

    std::string choice;
    while (true)
    {
        if (!current)
            break;
        std::cout << current->ParseNode();
        switch (current->GetNodeType())
        {
        case DialogueNode::TEXT:
            std::cin.get(); // Press button to continue...
            std::cout << "\n";
            current = current->GetNext();
            break;
        case DialogueNode::BRANCH:
            while (true)
            {
                std::getline(std::cin, choice); // Type in choice
                std::cout << "\n";
                if (std::shared_ptr<DialogueNode> next = current->GetNext(choice))
                {
                    current = next;
                    break;
                }
                else
                {
                    std::cout << "Invalid input!\n";
                }
            }

            break;
        default:
            assert(false && "INVALID NODE TYPE");
            return -1;
        }
    }
}