/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 05:28:43 by rlamlaik          #+#    #+#             */
/*   Updated: 2025/10/23 12:01:12 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
# define CUB_H

# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <limits.h>
# include "mlx.h"
# include <math.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# define FOV (60.0 * (M_PI / 180.0))
# define ROTATE_SPEED 3

# define KEY_W 13
# define KEY_S 1
# define KEY_A 0
# define KEY_D 2
# define ESC 53

# define SCREEN_WIDTH 1048
# define SCREEN_HEIGHT 640

# define UP 126
# define DOWN 125
# define RIGHT 124
# define LEFT 123

# define TILE_SIZE 60
# define TWO_PI 6.28318530718

# define MOVE_SPEED 28
# define ROTATION_SPEED 0.08

# define EAST_TEXTURE 3
# define WEST_TEXTURE 1
# define SOUTH_TEXTURE 2
# define NORTH_TEXTURE 0
# define DOOR_TEXTURE 4
# define DOOR 'D'

# define MINIMAP_SIZE 150
# define MINIMAP_TILE_SIZE 8
# define MINIMAP_OFFSET_X 30
# define MINIMAP_OFFSET_Y 30

# define FRAME_COLOR 0x555555
# define WALL_COLOR 0x800000
# define FLOOR_COLOR 0x000000
# define PLAYER_COLOR 0x00FF00

# define MINIMAP_X 20
# define MINIMAP_Y 20

typedef struct s_clip
{
	int	sx;
	int	sy;
	int	ex;
	int	ey;
}	t_clip;

typedef struct s_tile
{
	int		x;
	int		y;
	double	sx;
	double	sy;
}	t_tile;

typedef struct s_malloc
{
	void			*toalloc;
	struct s_malloc	*next;
}	t_malloc;

typedef struct s_txtu
{
	void	*txture_p;
	void	*data_add;
	int		bbp;
	int		endian;
	int		width;
	int		height;
	int		line_len;
}	t_txtu;

typedef struct s_loopvars
{
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		color;
	t_txtu	*texture;
}	t_loopvars;

typedef struct s_textures
{
	char	*north;
	char	*south;
	char	*west;
	char	*east;
}	t_textures;

typedef struct s_player
{
	double	x;
	double	y;
	double	dir_y;
	double	dir_x;
	int		move;
	int		c_count;
	char	direction;
}	t_player;

typedef struct s_line
{
	char			*line;
	struct s_line	*next;
}	t_line;

typedef struct s_ray
{
	double	ray_x;
	double	ray_y;
	double	ray_angle;
	double	ray_dir_x;
	double	ray_dir_y;
	double	delta_dist_x;
	double	delta_dist_y;
	int		map_x;
	int		map_y;
	double	perp_wall_dist;
	double	hit_x;
	double	hit_y;
	double	side;
}	t_ray;

typedef struct s_mlx
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}	t_mlx;

typedef struct s_sprite
{
	char	*img;
	int		width;
	int		height;
}	t_sprite;

typedef struct s_game
{
	t_txtu		txtu[5];
	t_textures	textures;
	char		**txt;
	t_mlx		*helper;
	double		angle;
	int			no;
	int			so;
	int			we;
	int			ea;
	int			f;
	int			c;
	int			floor_color;
	int			ceiling_color;
	t_line		*map_lines;
	char		**map;
	int			map_started;
	int			map_ended;
	int			map_width;
	int			map_height;
	t_player	player;
	t_ray		ray;
	t_sprite	gun[17];
	int			frame;
	int			flage;
}	t_game;

typedef struct s_mov
{
	double	new_x;
	double	new_y;
	int		map_y;
	int		map_x;
}	t_mov;

typedef struct s_casting
{
	double	dist;
	int		hited;
	int		side;
	double	side_dist_x;
	double	side_dist_y;
	int		step_x;
	int		step_y;
}	t_casting;

typedef struct s_texthelper
{
	int		tex_width;
	int		tex_height;
	double	wall_x;
	double	step;
	double	tex_pos;
	int		tex_x;
	int		y;
	int		x;
}	t_texthelper;

/* --------- function prototypes --------- */

void		check_argument(int ac, char **av);
void		init_game_struct(t_game *game);
int			is_config_line(char *line);
int			is_empty_line(char *line);
void		parse_line(t_game *game, char *line);
void		parse_config_line(t_game *game, char *line);
void		save_map_line(t_game *game, char *line);
int			in_valid_ber_file(char *file_name);
int			parse_color(char *line);
void		finalize_map(t_game *game);
void		print_map(char **map);
void		validate_map(t_game *game);
void		print_error(const char *message);

char		*ft_strdup(const char *s);
char		*get_next_line(int fd);
size_t		ft_strlen(const char *s);
char		*ft_strdup(const char *s1);
char		*ft_strjoin(char *s1, char *s2);
char		*ft_strchr(char *s, int c);
int			ft_atoi(const char *str);
int			ft_isdigit(int c);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		**ft_split(char const *s, char c);
void		*ft_memset(void *b, int c, size_t len);
void		*ft_memcpy(void *dst, const void *src, size_t n);
int			ft_strncmp(const char *s1, const char *s2, size_t n);

void		init_game(t_game *game);
void		my_mlx_pixel_put(t_game *game, int x, int y, int color);
void		the_3dview(t_game *game);
void		*ft_malloc(unsigned int size, int flag);
void		load_gun_sprite(t_game *game);
void		draw_minimap(t_game *game);
void		init_player_direction(t_game *game, char dir);
int			render_map(t_game *game);
int			bottoms(int keycode, t_game *game);
int			game_gun(t_game *game);
int			mouse_move_hook(int x, int y, t_game *game);

t_casting	*loop_helper(t_game *game, t_ray *ray, t_casting *holder);
void		move_side_dda(t_ray *ray, t_casting *holder);
double		cast_dda(t_game *game, t_ray *ray, t_casting *holder);
double		normalize_angle(double angle);
t_ray		prepare_vars(t_ray ray, t_game *game, int i_loop);

void		texture_loop(t_loopvars *vars, t_game *game, t_ray ray,
				t_texthelper *helper);
void		texture_pass(t_game *game, int x, t_ray ray, t_loopvars *vars);
void		helper_draw_ceilling(t_loopvars *vars);
void		draw_floor_and_ceiling(t_game *game, int x, t_loopvars *vars);

void		draw_square_clipped(t_game *game, int x, int y, int s_color[2]);
int			is_digit_str(char *s);
int			invalid_rgb(int r, int g, int b);
void		check_commas(char *line);

#endif
