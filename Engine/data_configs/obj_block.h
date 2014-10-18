/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2014 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OBJ_BLOCK_H
#define OBJ_BLOCK_H

#include "../graphics/graphics.h"

#include <QString>
#include <QPixmap>

struct obj_block
{
    unsigned long id;
        QString image_n;
        QString mask_n;
        //QPixmap image;
        //QPixmap mask;

    bool isInit;
    PGE_Texture * image;
    GLuint textureID;
    long textureArrayId;
    long animator_ID;

    QString name;
    //    grid=32				; 32 | 16 Default="32"
    unsigned int grid;
    QString group;
    QString category;
    bool sizable;
    int danger;
    int collision;
    bool slopeslide;
    int phys_shape;
    bool lava;
    bool destroyable;
    bool destroyable_by_bomb;
    bool destroyable_by_fireball;
    bool spawn; //split string by "-" in != "0"
        int spawn_obj; // 1 - NPC, 2 - block, 3 - BGO
        unsigned long spawn_obj_id;
    unsigned long effect;
    bool bounce;
    bool hitable;
    unsigned long transfororm_on_hit_into;
    unsigned long algorithm;

    unsigned int view;
    bool animated;
    bool animation_rev; //Reverse animation
    bool animation_bid; //Bidirectional animation
    unsigned int frames;
    int framespeed;

    unsigned int frame_h; //Hegth of the frame. Calculating automatically

    unsigned int display_frame;

    //Editor defaults
    bool default_slippery; //Slippery flag
    bool default_slippery_value;

    bool default_invisible; //Invisible flag
    bool default_invisible_value;

    bool default_content; //Content value
    long default_content_value;

};


#endif // OBJ_BLOCK_H
