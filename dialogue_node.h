#pragma once
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <optional>
#include <iostream>

struct DialogueNode
{
    using ChoiceMap = std::map<std::string, std::shared_ptr<DialogueNode>>;
    using ChoicePair = std::pair<std::string, std::shared_ptr<DialogueNode>>;

    enum NodeType
    {
        TEXT,
        BRANCH
    };

    union NodeData
    {
        NodeData(const std::shared_ptr<DialogueNode> &next = nullptr) : next(next) {};
        NodeData(const ChoiceMap &choices) : choices(choices) {};

        ~NodeData() {};
        ChoiceMap choices;
        std::shared_ptr<DialogueNode> next;
    };

    // Text Node Contructor
    DialogueNode(const std::string &text, const std::optional<std::string> &node_name = std::nullopt);

    // Branch Node Constructor
    DialogueNode(const std::string &text, const std::initializer_list<ChoicePair> &choices, const std::optional<std::string> &node_name = std::nullopt);

    DialogueNode(const std::string &text, const ChoiceMap &choices, const std::optional<std::string> &node_name = std::nullopt);
    ~DialogueNode();

    std::string text;

    NodeData data;

    NodeType GetNodeType() const { return m_node_type; }

    std::string ParseNode() const;

    std::shared_ptr<DialogueNode> GetNext();

    std::shared_ptr<DialogueNode> GetNext(std::string choice);

    std::shared_ptr<DialogueNode> Extend(const std::string &text, const std::optional<std::string> &node_name = std::nullopt);
    std::shared_ptr<DialogueNode> Extend(const std::string &text, const std::initializer_list<ChoicePair> &choices, const std::optional<std::string> &node_name = std::nullopt);

private:
    const NodeType m_node_type;
    static unsigned int m_node_id; // Only increments for anonymous nodes
};

std::shared_ptr<DialogueNode> MakeNode(const std::string &text, const std::optional<std::string> &node_name = std::nullopt);

std::shared_ptr<DialogueNode> MakeNode(const std::string &text, const std::initializer_list<DialogueNode::ChoicePair> &choices, const std::optional<std::string> &node_name = std::nullopt);

std::shared_ptr<DialogueNode> MakeNode(const std::string &text, const DialogueNode::ChoiceMap &choices, const std::optional<std::string> &node_name = std::nullopt);

std::shared_ptr<DialogueNode> GetNode(const std::string &name);

static std::map<std::string, std::shared_ptr<DialogueNode>> nodes = {};