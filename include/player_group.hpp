#pragma once
#include "game_class.hpp"
#include "player_class.hpp"
#include "dbscan.hpp"


class squad_class
{
protected:
    unit_class* leader_;
    std::vector<unit_class*> unit_list_;
    int hit_points_;
    int damage_per_frame_;
    int max_speed_;
    position location_;

public:
    explicit squad_class(unit_class* leader)
        : leader_(leader)
          , hit_points_(leader->hp_get())
          , damage_per_frame_(leader->damage_per_frame_get())
          , max_speed_(leader->max_speed_get())
          , location_(leader->position_get())

    {
    }

    void unit_append(unit_class* unit);
};

class player_group final : public virtual_player_class
{
    void squads_update(const std::vector<unit_class*>& unit_list);
    std::vector<squad_class*> squad_list_;
    int characteristic_size_ = 5;
    int min_pts_ = 3;
    int last_squad_check_ = 0;
    int last_squad_check_delta_ = 24;

public:
    explicit player_group(const player_id id)
        : virtual_player_class(id)
    {
    }

    std::vector<unit_action> moves_get(state_class* s) override;
};
