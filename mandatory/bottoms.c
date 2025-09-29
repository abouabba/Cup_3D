#include "cub.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(int n)
{
	if (n == -2147483648)
	{
		write(1, "-2147483648", 11);
		return;
	}
	if (n < 0)
	{
		ft_putchar('-');
		n = -n;
	}
	if (n >= 10)
		ft_putnbr(n / 10);
	ft_putchar((n % 10) + '0');
}

void update_position(t_game *game)
{
	game->player.x = game->player.x + 0.5;
	game->player.y = game->player.y + 0.5;
}

#define MOVE_SPEED 1
#include <math.h>

// void move_player(t_game *game, double dx, double dy)
// {
//     // Force dx and dy to be at most 1 pixel step
//     if (dx > 0) dx = 1;
//     else if (dx < 0) dx = -1;
//     else dx = 0;

//     if (dy > 0) dy = 1;
//     else if (dy < 0) dy = -1;
//     else dy = 0;

//     double new_x = game->player.x + dx;
//     double new_y = game->player.y + dy;
//     int map_x = (int)ceil(new_x);
//     int map_y = (int)ceil(new_y);

//     int curr_x = (int)(game->player.x / (TILE_SIZE - 8));
//     int curr_y = (int)(game->player.y / (TILE_SIZE - 8));

//     printf("11Trying to move to [%d][%d]\n", map_y, map_x);
//     printf("11Trying to move to [%f][%f]\n", new_x, new_y);

//     if (map_y >= 0 && map_y < game->map_height &&
//         map_x >= 0 && map_x < game->map_width)
//     {
//         if (game->map[(int)ceil(new_y)][(int)ceil(new_x)] != '1')
//         {
//             game->player.x = new_x;
//             game->player.y = new_y;
//         }
//     }

//     printf("11Tgame->map[(int)map_y][(int)dx] t %c\n", game->map[(int)map_y][(int)map_x]);
//     printf("11Tgame->map[(int)map_y][(int)dx] t %d %d\n", (int)map_y, (int)map_x);
// }
void move_player(t_game *game, double dx, double dy)
{
    // Clamp dx, dy to max 1 pixel step
    if (dx > 1) dx = 1;
    else if (dx < -1) dx = -1;

    if (dy > 1) dy = 1;
    else if (dy < -1) dy = -1;

    double new_x = game->player.x + dx;
    double new_y = game->player.y + dy;

    // Convert pixel coords → tile coords
    int map_x = (int)ceil(new_x / TILE_SIZE);
    int map_y = (int)ceil(new_y / TILE_SIZE);

    printf("Trying to move to pixel: [%.2f][%.2f]\n", new_x, new_y);
    printf("Tile coords: [%d][%d]\n", map_y, map_x);

    // Check bounds
    if (map_y >= 0 && map_y < game->map_height &&
        map_x >= 0 && map_x < game->map_width)
    {
        // Only move if not a wall
        if (game->map[map_y][map_x] != '1')
        {
            game->player.x = new_x;
            game->player.y = new_y;
        }
    }

    printf("Tile value at [%d][%d] = %c\n", map_y, map_x, game->map[map_y][map_x]);
}

void move_pRRlayer(t_game *game, double dx, double dy)
{
    // candidate new position (in pixels)
    double new_x = game->player.x + dx * MOVE_SPEED;
    double new_y = game->player.y + dy * MOVE_SPEED;

    // check map cell for new_x/new_y
    int map_x = (int)(new_x / TILE_SIZE);
    int map_y = (int)(new_y / TILE_SIZE);

    int curr_x = (int)(game->player.x / TILE_SIZE);
    int curr_y = (int)(game->player.y / TILE_SIZE);

    printf("From pixel (%.2f, %.2f) → (%.2f, %.2f)\n", 
           game->player.x, game->player.y, new_x, new_y);
    printf("From tile [%d][%d] → [%d][%d]\n", curr_y, curr_x, map_y, map_x);

    // keep inside map bounds
    if (map_y >= 0 && map_y < game->map_height &&
        map_x >= 0 && map_x < game->map_width)
    {
        // wall collision check
        if (game->map[map_y][map_x] != '1')
        {
            game->player.x = new_x;
            game->player.y = new_y;
        }
    }
}


#define MOVE_SPEED      3.0            // pixels per tick (tune)
#define PLAYER_RADIUS   8.0            // < TILE_SIZE/2 (tune)

#define ROTATION_SPEED 0.09
#define ROTATION_SPEED 0.06

// int bottoms(int keycode, t_game *game)
// {
//     double dir_x = 0.0, dir_y = 0.0;

//     if (keycode == LEFT)
//         game->angle -= ROTATION_SPEED;
//     else if (keycode == RIGHT)
//         game->angle += ROTATION_SPEED;

//     if (game->angle < 0) game->angle += 2.0 * M_PI;
//     if (game->angle >= 2.0 * M_PI) game->angle -= 2.0 * M_PI;

//     // Build direction (unit-ish). No MOVE_SPEED here.
//     if (keycode == KEY_W) { // forward
//         dir_x += cos(game->angle);
//         dir_y += sin(game->angle);
//     }
//     if (keycode == KEY_S) { // backward
//         dir_x -= cos(game->angle);
//         dir_y -= sin(game->angle);
//     }
//     if (keycode == KEY_A) { // strafe right
//         dir_x += sin(game->angle);
//         dir_y -= cos(game->angle);
//     }
//     if (keycode == KEY_D) { // strafe left
//         dir_x -= sin(game->angle);
//         dir_y += cos(game->angle);
//     }
//     if (keycode == 65307) exit(0);

//     move_player(game, dir_x, dir_y);

//     mlx_clear_window(game->helper->mlx, game->helper->win);
//     render_map(game);
//     return 0;
// }
#define MOVE_SPEED     1.0     // pixels per tick
#define PLAYER_RADIUS  8.0     // must be < TILE_SIZE/2

// #define MOVE_SPEED     4.0
// #define PLAYER_RADIUS  8.0
// #define MAX_SUBSTEP    10.0   // <= 10px per micro-step
// #define EPS            0.01

// --- helpers (use your versions if you already have them) ---
static inline int is_block_cell(const t_game *g, int tx, int ty)
{
    if (ty < 0 || ty >= g->map_height) return 1;
    int row_len = (int)strlen(g->map[ty]);   // if rows are ragged
    if (tx < 0 || tx >= row_len) return 1;
    char c = g->map[ty][tx];
    return (c == '1' || c == ' ');
}

static inline int is_wall_at(const t_game *g, double wx, double wy)
{
    int tx = (int)floor(wx / TILE_SIZE);
    int ty = (int)floor(wy / TILE_SIZE);
    return is_block_cell(g, tx, ty);
}

static inline int collides_at(const t_game *g, double x, double y, double r)
{
    // 4 samples are usually enough; add diagonals if you want tighter collision
    return is_wall_at(g, x - r, y) ||
           is_wall_at(g, x + r, y) ||
           is_wall_at(g, x, y - r) ||
           is_wall_at(g, x, y + r);
}

// --- main move ---
// void move_player(t_game *g, double dir_x, double dir_y)
// {
//     // normalize input direction
//     double len = sqrt(dir_x*dir_x + dir_y*dir_y);
//     if (len == 0.0) return;
//     dir_x /= len; dir_y /= len;

//     // frame move vector in pixels
//     double dx = dir_x * MOVE_SPEED;
//     double dy = dir_y * MOVE_SPEED;

//     // split into sub-steps so each is <= MAX_SUBSTEP px long
//     double total = hypot(dx, dy);
//     int steps = (int)ceil(total / MAX_SUBSTEP);
//     if (steps < 1) steps = 1;

//     double sx = dx / steps;
//     double sy = dy / steps;

//     for (int s = 0; s < steps; ++s) {
//         // X axis first (slide)
//         double nx = g->player.x + sx;
//         if (!collides_at(g, nx, g->player.y, PLAYER_RADIUS)) {
//             g->player.x = nx;
//         } else {
//             // optional: clamp to the nearest grid boundary to avoid sinking
//             if (sx > 0) {
//                 double grid = (floor((g->player.x + PLAYER_RADIUS) / TILE_SIZE) + 1) * TILE_SIZE;
//                 g->player.x = fmin(nx, grid - PLAYER_RADIUS - EPS);
//             } else if (sx < 0) {
//                 double grid = floor((g->player.x - PLAYER_RADIUS) / TILE_SIZE) * TILE_SIZE;
//                 g->player.x = fmax(nx, grid + PLAYER_RADIUS + EPS);
//             }
//         }

//         // Y axis next (slide)
//         double ny = g->player.y + sy;
//         if (!collides_at(g, g->player.x, ny, PLAYER_RADIUS)) {
//             g->player.y = ny;
//         } else {
//             if (sy > 0) {
//                 double grid = (floor((g->player.y + PLAYER_RADIUS) / TILE_SIZE) + 1) * TILE_SIZE;
//                 g->player.y = fmin(ny, grid - PLAYER_RADIUS - EPS);
//             } else if (sy < 0) {
//                 double grid = floor((g->player.y - PLAYER_RADIUS) / TILE_SIZE) * TILE_SIZE;
//                 g->player.y = fmax(ny, grid + PLAYER_RADIUS + EPS);
//             }
//         }
//     }
// }
int bottoms(int keycode, t_game *game)
{
    double dx = 0.0;
    double dy = 0.0;

    if (keycode == LEFT)
        game->angle -= ROTATION_SPEED;
    else if (keycode == RIGHT)
        game->angle += ROTATION_SPEED;

    // normalize angle into [0, 2*PI)
    if (game->angle < 0)
        game->angle += 2.0 * M_PI;
    if (game->angle >= 2.0 * M_PI)
        game->angle -= 2.0 * M_PI;

    if (keycode == KEY_W) 
	{ // forward
        printf("this is the forwaar moing |\n");
        dx = cos(game->angle) * MOVE_SPEED;
        dy = sin(game->angle) * MOVE_SPEED;
    }
	if (keycode == KEY_S) { // backward
        dx = -cos(game->angle) * MOVE_SPEED;
        dy = -sin(game->angle) * MOVE_SPEED;
    } if (keycode == KEY_D) { // strafe left
        dx = -sin(game->angle) * MOVE_SPEED;
        dy =  cos(game->angle) * MOVE_SPEED;
    } if (keycode == KEY_A) { // strafe right
        dx =  sin(game->angle) * MOVE_SPEED;
        dy = -cos(game->angle) * MOVE_SPEED;
    } if (keycode == 65307) {
        exit(0);
    }

    move_player(game, dx, dy);
    printf("kkkkkkk1212\n");
    mlx_clear_window(game->helper->mlx, game->helper->win);
    printf("kkkkkkk1212121212121212\n");
    // segfault
    render_map(game); // ensure this uses player.x/player.y for drawing
    printf("kkkkkkk333333\n");

    return 0;
}
