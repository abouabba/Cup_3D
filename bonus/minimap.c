/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 12:24:06 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/17 22:16:21 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void    draw_clipped_square(t_game *game, int x, int y, int size, int color)
{
    int i, j;
    int draw_start_x, draw_start_y;
    int draw_end_x, draw_end_y;

    int clip_x_min = MINIMAP_OFFSET_X + 1;
    int clip_y_min = MINIMAP_OFFSET_Y + 1;
    int clip_x_max = MINIMAP_OFFSET_X + MINIMAP_SIZE - 1;
    int clip_y_max = MINIMAP_OFFSET_Y + MINIMAP_SIZE - 1;

    draw_start_x = (x > clip_x_min) ? x : clip_x_min;
    draw_start_y = (y > clip_y_min) ? y : clip_y_min;

    draw_end_x = (x + size < clip_x_max) ? (x + size) : clip_x_max;
    draw_end_y = (y + size < clip_y_max) ? (y + size) : clip_y_max;

    for (i = draw_start_y; i < draw_end_y; i++)
    {
        for (j = draw_start_x; j < draw_end_x; j++)
        {
            my_mlx_pixel_put(game, j, i, color);
        }
    }
}

static void draw_minimap_frame(t_game *game)
{
    int i;
    int frame_x = MINIMAP_OFFSET_X;
    int frame_y = MINIMAP_OFFSET_Y;
    int size = MINIMAP_SIZE;

    // Draw top and bottom horizontal lines
    for (i = 0; i < size; i++)
    {
        my_mlx_pixel_put(game, frame_x + i, frame_y, FRAME_COLOR);
        my_mlx_pixel_put(game, frame_x + i, frame_y + size - 1, FRAME_COLOR);
    }
    
    // Draw left and right vertical lines (skip corners, already drawn)
    for (i = 1; i < size - 1; i++)
    {
        my_mlx_pixel_put(game, frame_x, frame_y + i, FRAME_COLOR);
        my_mlx_pixel_put(game, frame_x + size - 1, frame_y + i, FRAME_COLOR);
    }
}


void draw_minimap(t_game *game)
{
    draw_minimap_frame(game); // Call optimized function

    double player_x = game->player.x;
    double player_y = game->player.y;

    // Calculate map coordinates for the top-left corner of the minimap
    // Using 2.0 ensures floating-point division
    double start_x = player_x - (MINIMAP_SIZE / (2.0 * MINIMAP_TILE_SIZE));
    double start_y = player_y - (MINIMAP_SIZE / (2.0 * MINIMAP_TILE_SIZE));
    
    // Calculate map coordinates for the bottom-right (for loop boundary)
    double end_x = start_x + (MINIMAP_SIZE / (double)MINIMAP_TILE_SIZE);
    double end_y = start_y + (MINIMAP_SIZE / (double)MINIMAP_TILE_SIZE);

    for (int map_y = (int)start_y; map_y < (int)end_y + 1; map_y++)
    {
        for (int map_x = (int)start_x; map_x < (int)end_x + 1; map_x++)
        {
            if (map_y < 0 || map_y >= game->map_height ||
                map_x < 0 || map_x >= game->map_width)
                continue; // Skip out-of-bounds map tiles

            char tile = game->map[map_y][map_x];
            int color;

            if (tile == '1')
                color = WALL_COLOR;
            else if (tile == 'D') // door
                color = 0x808000;
            else
                color = FLOOR_COLOR; // Includes '0', 'N', 'S', etc.

            // Calculate screen position for the top-left of this tile
            int draw_x = MINIMAP_OFFSET_X + (int)((map_x - start_x) * MINIMAP_TILE_SIZE);
            int draw_y = MINIMAP_OFFSET_Y + (int)((map_y - start_y) * MINIMAP_TILE_SIZE);

            // Draw the tile, the function will handle clipping
            // This REPLACES your buggy 'if' condition
            draw_clipped_square(game, draw_x, draw_y, MINIMAP_TILE_SIZE, color);
        }
    }

    // Draw player at center
    int player_screen_x = MINIMAP_OFFSET_X + (MINIMAP_SIZE / 2);
    int player_screen_y = MINIMAP_OFFSET_Y + (MINIMAP_SIZE / 2);

    if (game->map[(int)player_y][(int)player_x] != '1' &&
        game->map[(int)player_y][(int)player_x] != ' ')
    {
        // Draw a 4x4 player dot, centered on the player's exact spot
        // This will also be correctly clipped by the new function
        draw_clipped_square(game,
            player_screen_x - 2,
            player_screen_y - 2,
            4,
            PLAYER_COLOR);
    }
}