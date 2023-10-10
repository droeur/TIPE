#pragma once
#include "player_class.hpp"
#include "db_scan.hpp"
#include "mcts.hpp"


class squad_class
{
    mcts *mcts_;
    unit_class& leader_;
    std::vector<unit_class*> unit_list_;
    int hit_points_;
    int damage_per_frame_;
    int max_speed_;
    position location_;

public:
    explicit squad_class(unit_class& leader, map_class* map)
        : leader_(leader)
        , hit_points_(leader.hp_get())
        , damage_per_frame_(leader.damage_per_frame_get())
        , max_speed_(leader.max_speed_get())
        , location_(leader.position_get())
    {
        mcts_ = new mcts(10, map, leader.player_get());
    }

    void unit_append(unit_class* unit);
    void moves_generate(const map_class* map, state_class* state) const;
    [[nodiscard]] unit_class& leader_get() const;
};

class player_group final : public virtual_player_class
{
    void squads_update(const std::vector<unit_class>& unit_list);
    std::vector<squad_class*> squad_list_;
    int characteristic_size_ = 5;
    int min_pts_ = 3;
    int last_squad_check_ = 0;
    int last_squad_check_delta_ = 1;
    int last_squad_move_check_ = 0;
    int last_squad_move_check_delta_ = 1;
    map_class *map_ = nullptr;

public:
    explicit player_group(const player_id id)
        : virtual_player_class(id, player_type::group)
    {
    }

    void moves_get(const game_class* game, state_class* state) override;
};
