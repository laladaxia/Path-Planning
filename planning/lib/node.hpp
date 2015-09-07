#pragma once

#include <string>
#include <map>
#include <functional>
#include <vector>

namespace search
{
    class Node
    {
    public:
        using Size = std::size_t;
        using Path = std::string;
        using Children = std::vector<Node>;
        struct Coordinate
        {
            Coordinate(Size x, Size y)
                : x{ x }, y{ y }
            {}

            Coordinate& operator = (Coordinate const& other)
            {
                x = other.x;
                y = other.y;
                return *this;
            }

            Size x, y;
        };
        using Functions = std::map< char, std::function< Coordinate(Coordinate) >>;

        //
        //  ctor
        //
        Node(Path const& path) 
            : _path{ path }
        { }

        auto path() const -> Path const& 
        {
            return _path; 
        }

        auto coordinate(Coordinate start) const -> Coordinate
        {
            Coordinate c = start;
            for (auto direction : _path)
                c = Node::goes.at(direction)(c);
            return c;
        }

        template<typename ValidateFunc>
        auto children(ValidateFunc validate) const -> Children
        {
            Children children;
            for(auto direction = '1'; direction <= '8'; ++direction)
            {
                auto child = Node{_path + direction};
                if(validate(child))
                    children.push_back(child);
            }
            return children;
        }

        const static Functions goes;
    private:
        Path const  _path;
    };

    auto operator==(Node::Coordinate lhs, Node::Coordinate rhs) -> bool
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    Node::Functions const Node::goes 
    {
        { '1', [](Coordinate c) -> Coordinate{ return{ c.x - 1, c.y - 1 }; } },
        { '2', [](Coordinate c) -> Coordinate{ return{ c.x - 0, c.y - 1 }; } },
        { '3', [](Coordinate c) -> Coordinate{ return{ c.x + 1, c.y + 1 }; } },
        { '4', [](Coordinate c) -> Coordinate{ return{ c.x - 1, c.y + 0 }; } },
        { '5', [](Coordinate c) -> Coordinate{ return{ c.x + 1, c.y + 0 }; } },
        { '6', [](Coordinate c) -> Coordinate{ return{ c.x - 1, c.y - 1 }; } },
        { '7', [](Coordinate c) -> Coordinate{ return{ c.x - 0, c.y - 1 }; } },
        { '8', [](Coordinate c) -> Coordinate{ return{ c.x + 1, c.y - 1 }; } }
    };
}