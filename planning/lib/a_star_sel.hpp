//
//  A* with strict expanded list
//
#pragma once

#include <functional>
#include <unordered_set>
#include "priority_queue.hpp"
#include "func.hpp"
#include "node.hpp"
#include "timing.hpp"

using std::size_t;
using std::string;
using std::move;
using std::max;
using std::any_of;
using std::unordered_set;

namespace search
{
    //
    //  Hfunc : heuristic function, (Node) -> h value
    //  ValidateFunc : unary predicate, check if a node is valid, (Node) -> bool
    //  Cfunc : cost function, (Node) -> c value
    //
    template<typename Hfunc, typename ValidateFunc, typename Cfunc = Cost<Node>>
    class AStarSEL
    {
    public:
        using Q = PriorityQueue<Node, Less<Node, Hfunc>>;
        using Expansions = unordered_set<State>;

        struct Get
        {
            Q q;
            size_t const& max_q_size;
            Expansions const& expansions;
            string const& final_path;
            long long const& run_time;
            bool const& is_found;
        } const last_run;

        AStarSEL()
            : last_run{ _q, _max_q_size, _expansions, _final_path, _run_time, _is_found }
        {
            reset();
        }

        auto operator()(State start, State goal, ValidateFunc validate) -> void
        {
            reset();
            Timing timing{ _run_time };
            search(start, goal, validate);
        }

    private:
        //
        //  Data members
        //
        Q _q;
        size_t _max_q_size;
        Expansions _expansions;
        string _final_path;
        long long _run_time;
        bool _is_found;
        //
        //  reset all data members
        //
        auto reset() -> void
        {
            _q.reset();
            _max_q_size = 0;
            _expansions.clear();
            _final_path.clear();
            _run_time = 0;
            _is_found = false;
        }
        //
        //  Core part
        //
        auto search(State start, State goal, ValidateFunc validate) -> void
        {
            for (_q.push({ "", start, goal }); !_q.empty() && _q.top().state() != goal; update_max_q_size())
            {
                auto curr = _q.pop();
                if (!is_expanded(curr))
                {
                    expand(curr);
                    for (auto const& child : curr.children(validate))
                        if (!is_expanded(child))
                            if (!_q.any(SameStateAs{ child.state() }))
                                _q.push(child);
                            else
                                _q.update_with_if(child, SameStateAs{ child.state() });
                }
            }

            if (_q.empty())
            {
                _final_path = "";
                _is_found = false;
            }
            else
            {
                _final_path = _q.top().path();
                _is_found = true;
            }
        }
        //
        //  helper for method search
        //
        auto update_max_q_size() -> void
        {
            _max_q_size = max(_max_q_size, _q.size());
        }
        //
        //  helper for method search
        //
        struct SameStateAs
        {
            State state_to_compare;
            auto operator()(Node const& n) const -> bool
            {
                return n.state() == state_to_compare;
            }
        };
        //
        //  helper for method search
        //
        auto is_expanded(Node const& n) const -> bool
        {
            auto const& e = _expansions;
            return any_of(e.cbegin(), e.cend(), [&](State s) {return s == n.state(); });
        }
        //
        //  helper for method search
        //
        auto expand(Node const& n) -> void
        {
            _expansions.insert(n.state());
        }
    };
}