#include "dialogue_node.h"
#include <format>
#include <cassert>

unsigned int DialogueNode::m_node_id = 0;

DialogueNode::DialogueNode(const std::string &text, const std::optional<std::string> &node_name)
    : m_node_type(TEXT), text(text), data()
{
}

DialogueNode::DialogueNode(const std::string &text, const std::initializer_list<ChoicePair> &choices, const std::optional<std::string> &node_name)
    : m_node_type(BRANCH), text(text), data(ChoiceMap{})
{
    for (ChoicePair choice : choices)
    {
        data.choices.emplace(choice);
    }
}

DialogueNode::DialogueNode(const std::string &text, const ChoiceMap &choices, const std::optional<std::string> &node_name)
    : m_node_type(BRANCH), text(text), data(choices)
{
}

DialogueNode::~DialogueNode()
{

    switch (m_node_type)
    {
    case TEXT:
        data.next.~shared_ptr();
        break;
    case BRANCH:
        data.choices.~map<std::string, std::shared_ptr<DialogueNode>>();
        break;
    default:
        break;
    }
}

std::string DialogueNode::ParseNode() const
{
    std::string options = "";
    unsigned int i = 0;
    switch (m_node_type)
    {
    case TEXT:
        return text;
    case BRANCH:

        for (auto choice : data.choices)
        {
            options += '\t' + choice.first + '\n';
        }
        return text + '\n' + options;
    default:
        assert(false && "INVALID NODE TYPE");
        return "INVALD NODE TYPE";
    }
}

std::shared_ptr<DialogueNode> DialogueNode::GetNext()
{
    assert(m_node_type == TEXT && "Getting next without choice on a branch node");
    if (m_node_type != TEXT)
        return nullptr;

    return data.next;
}

std::shared_ptr<DialogueNode> DialogueNode::GetNext(std::string choice)
{
    assert(m_node_type == BRANCH && "Getting next with choice on text node");
    if (m_node_type != BRANCH)
        return nullptr;
    if (!data.choices.contains(choice))
        return nullptr; // invalid input
    return data.choices[choice];
}
std::shared_ptr<DialogueNode> DialogueNode::Extend(const std::string &text, const std::optional<std::string> &node_name)
{
    assert(m_node_type == TEXT && "Attempted to extend non-text node");
    data.next = MakeNode(text, node_name);
    return data.next;
}

std::shared_ptr<DialogueNode> DialogueNode::Extend(const std::string &text, const std::initializer_list<ChoicePair> &choices, const std::optional<std::string> &node_name)
{
    assert(m_node_type == TEXT && "Attempted to extend non-text node");
    data.next = MakeNode(text, choices, node_name);
    return data.next;
}

std::shared_ptr<DialogueNode> DialogueNode::Link(const std::string name)
{
    assert(m_node_type == TEXT && "Attempted to link non-text node");
    data.next = GetNode(name);
    return data.next;
}

std::shared_ptr<DialogueNode> DialogueNode::Link(const std::shared_ptr<DialogueNode> other)
{
    assert(m_node_type == TEXT && "Attempted to link non-text node");
    data.next = other;
    return data.next;
}

std::shared_ptr<DialogueNode> DialogueNode::Link(const std::string name1, const std::string &name2)
{
    std::shared_ptr<DialogueNode> node1 = GetNode(name1), node2 = GetNode(name2);
    assert(node1->m_node_type == TEXT && node2->m_node_type == TEXT && "Attempted to link non-text node");
    return node1->Link(node2);
}

std::shared_ptr<DialogueNode> DialogueNode::Link(const std::shared_ptr<DialogueNode> node1, const std::shared_ptr<DialogueNode> node2)
{
    assert(node1->m_node_type == TEXT && node2->m_node_type == TEXT && "Attempted to link non-text node");
    return node1->Link(node2);
}

std::shared_ptr<DialogueNode> DialogueNode::MakeNode(const std::string &text, const std::optional<std::string> &node_name)
{
    std::shared_ptr<DialogueNode> node = std::make_shared<DialogueNode>(text, node_name);

    if (node_name)
    {
        nodes.emplace(*node_name, node);
    }
    else
    {
        const std::string anon_name = std::format("T{}", node->m_node_id++);
        nodes.emplace(anon_name, node);
    }
    return node;
}

std::shared_ptr<DialogueNode> DialogueNode::MakeNode(const std::string &text, const std::initializer_list<DialogueNode::ChoicePair> &choices, const std::optional<std::string> &node_name)
{
    std::shared_ptr<DialogueNode> node = std::make_shared<DialogueNode>(text, choices, node_name);
    if (node_name)
    {
        nodes.emplace(*node_name, node);
    }
    else
    {
        const std::string anon_name = std::format("B{}", node->m_node_id++);
        nodes.emplace(anon_name, node);
    }
    return node;
}

std::shared_ptr<DialogueNode> DialogueNode::MakeNode(const std::string &text, const DialogueNode::ChoiceMap &choices, const std::optional<std::string> &node_name)
{
    std::shared_ptr<DialogueNode> node = std::make_shared<DialogueNode>(text, choices, node_name);
    if (node_name)
    {
        nodes.emplace(*node_name, node);
    }
    else
    {
        const std::string anon_name = std::format("B{}", node->m_node_id++);
        nodes.emplace(anon_name, node);
    }
    return node;
}

std::shared_ptr<DialogueNode> DialogueNode::GetNode(const std::string &name)
{
    assert(nodes.contains(name) && "Node does not exist");
    return nodes[name];
}
