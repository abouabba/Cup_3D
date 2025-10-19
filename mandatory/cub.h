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
#endif

# define FOV 60.0 * (M_PI / 180)
# define ROTATE_SPEED 3
# define KEY_W 13      // W
# define KEY_S 1       // S
# define KEY_A 0       // A
# define KEY_D 2       // D
# define ESC 53        // Escape

# define SCREEN_WIDTH 1048
# define SCREEN_HEIGHT 640

# define UP 126        // Up arrow
# define DOWN 125      // Down arrow
# define RIGHT 124     // Right arrow
# define LEFT 123      // Left arrow
// # define TILE_P 8
# define TILE_SIZE 60

# define TWO_PI 6.28318530718

#define MOVE_SPEED 37
#define ROTATION_SPEED 0.08


# define EAST_TEXTURE 3
# define WEST_TEXTURE 1
# define SOUTH_TEXTURE 2
# define NORTH_TEXTURE 0



typedef struct s_malloc
{
	void			*toalloc;
	struct s_malloc	*next;
}					t_malloc;


typedef struct s_loopvars
{
	int line_height;
	int draw_start;
	int draw_end;
	int color;
}				t_loopvars;


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


typedef struct s_textures {
	char	*north;
	char	*south;
	char	*west;
	char	*east;
}	t_textures;

typedef struct s_player {
	double		x;
	double		y;
	double		dir_y;
	double		dir_x;
	int		move;
	int		c_count;
	char	direction;
}	t_player;

typedef struct s_line {
	char			*line;
	struct s_line	*next;
}	t_line;


typedef struct s_ray
{
    double ray_x;
    double ray_y;
    double ray_angle; // allready exist
    double ray_dir_x;
    double ray_dir_y;
    double delta_dist_x;
    double delta_dist_y;
    int map_x;
    int map_y;
	double perp_wall_dist;
	double hit_x;
	double hit_y;
	double side;
} t_ray;

typedef  struct s_mlx
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}		t_mlx;


typedef struct s_game 
{
	t_txtu		txtu[4];
	t_textures	textures;
	char		**txt;
	t_mlx		*helper;
	double 		angle;
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
	t_ray   ray;
}	t_game;

typedef struct s_casting
{
	double 	dist;
    int     hited;
    int     side;
    double  side_dist_x;
    double  side_dist_y;
    int     step_x;
    int     step_y;
}   t_casting;  

// ---- Function prototypes ----
void	check_argument(int ac, char **av);
void	init_game_struct(t_game *game);
int		is_config_line(char *line);
int		is_empty_line(char *line);
void	parse_line(t_game *game, char *line);
void	parse_config_line(t_game *game, char *line);
void	save_map_line(t_game *game, char *line);
int		in_valid_ber_file(char *file_name);
void	parse_config_line(t_game *game, char *line);
int		parse_color(char *line);
void    finalize_map(t_game *game);
void 	print_map(char **map);
void	validate_map(t_game *game);
void	print_error(const char *message);


// helper
char	*ft_strdup(const char *s);
char	*get_next_line(int fd); // must be included from your GNL
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strchr(char *s, int c);
int		ft_atoi(const char	*str);
int		ft_isdigit(int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	**ft_split(char const *s, char c);
void	*ft_memset(void *b, int c, size_t len);
void	*ft_memcpy(void *dst, const void *src, size_t n);
int		ft_strncmp(const char *s1, const char *s2, size_t n);



//reyn's part
void 	init_game(t_game *game);
int 	close_window(t_game *game);
int 	render_map(t_game *game);
void 	my_mlx_pixel_put(t_game *game, int x, int y, int color);
int 	bottoms(int keycode, t_game *game);
void    the_3dview(t_game *game);
void	*ft_malloc(unsigned int size, int flag);


#endif
