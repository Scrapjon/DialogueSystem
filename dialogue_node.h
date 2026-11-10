#pragma once
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <optional>

struct DialogueNode : public std::enable_shared_from_this<DialogueNode>
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

    static std::shared_ptr<DialogueNode> MakeNode(const std::string &text, const std::optional<std::string> &node_name = std::nullopt);

    static std::shared_ptr<DialogueNode> MakeNode(const std::string &text, const std::initializer_list<DialogueNode::ChoicePair> &choices, const std::optional<std::string> &node_name = std::nullopt);

    static std::shared_ptr<DialogueNode> MakeNode(const std::string &text, const DialogueNode::ChoiceMap &choices, const std::optional<std::string> &node_name = std::nullopt);

    static std::shared_ptr<DialogueNode> GetNode(const std::string &name);

    // TODO: Look into enforcing use of MakeNode factory rather than constructor

    // Text Node Contructor
    DialogueNode(const std::string &text, const std::optional<std::string> &node_name = std::nullopt);

    // Branch Node Constructor
    DialogueNode(const std::string &text, const std::initializer_list<ChoicePair> &choices, const std::optional<std::string> &node_name = std::nullopt);

    DialogueNode(const std::string &text, const ChoiceMap &choices, const std::optional<std::string> &node_name = std::nullopt);
    ~DialogueNode();

public:
    std::string text;

    NodeData data;

    NodeType GetNodeType() const { return m_node_type; }

    std::string ParseNode() const;

    std::shared_ptr<DialogueNode> GetNext();

    std::shared_ptr<DialogueNode> GetNext(std::string choice);

    std::shared_ptr<DialogueNode> Extend(const std::string &text, const std::optional<std::string> &node_name = std::nullopt);
    std::shared_ptr<DialogueNode> Extend(const std::string &text, const std::initializer_list<ChoicePair> &choices, const std::optional<std::string> &node_name = std::nullopt);
    std::shared_ptr<DialogueNode> Link(const std::string name);
    std::shared_ptr<DialogueNode> Link(const std::shared_ptr<DialogueNode> other);
    static std::shared_ptr<DialogueNode> Link(const std::string name1, const std::string &name2);
    static std::shared_ptr<DialogueNode> Link(const std::shared_ptr<DialogueNode> node1, const std::shared_ptr<DialogueNode> node2);

private:
    const NodeType m_node_type;
    static unsigned int m_node_id; // Only increments for anonymous nodes
};

inline static std::map<std::string, std::shared_ptr<DialogueNode>> nodes = {};