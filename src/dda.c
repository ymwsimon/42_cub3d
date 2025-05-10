/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:48:23 by yublee            #+#    #+#             */
/*   Updated: 2025/05/10 17:17:43 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	prepare_dda(t_vars *vars, t_dda *dda)
{
	// double	d;

	if (!vars || !dda)
		return ;
	dda->map_pos.x = (int)vars->vecset->pos.x;
	dda->map_pos.y = (int)vars->vecset->pos.y;
	dda->step_size.x = 1e30;
	if (fabs(dda->ray_dir.x) > EPSILON)
	{
		dda->step_size.x = 1.0 / fabs(dda->ray_dir.y / dda->ray_dir.x);
		// printf("%f %f %f\n", 1.0 / fabs(dda->ray_dir.y / dda->ray_dir.x) * dda->sign.x, fabs(1 / dda->ray_dir.x), sqrt(1 + (dda->ray_dir.y * dda->ray_dir.y) / (dda->ray_dir.x * dda->ray_dir.x)));
	}
	dda->step_size.y = 1e30;
	if (fabs(dda->ray_dir.y) > EPSILON)
		dda->step_size.y = fabs(dda->ray_dir.y / dda->ray_dir.x);
	if (dda->ray_dir.x < 0)
	{
		dda->map_step.x = -1;
		dda->travelled_distance.x = vars->vecset->pos.x - dda->map_pos.x;
		// dda->travelled_distance.x *= dda->step_size.x;
		dda->travelled_distance.x /= fabs(dda->ray_dir.y / dda->ray_dir.x);
	}
	else
	{
		dda->map_step.x = 1;
		dda->travelled_distance.x = dda->map_pos.x + 1 - vars->vecset->pos.x;
		// dda->travelled_distance.x *= dda->step_size.x;
		dda->travelled_distance.x /= fabs(dda->ray_dir.y / dda->ray_dir.x);
	}
	// d = (int)vars->vecset->pos.y;
	// if (dda->sign.y > 0)
	// 	d = (int)vars->vecset->pos.y - 1;
	// d = fabs(d - vars->vecset->pos.y);
	// dda->travelled_distance.x = vars->vecset->pos.x + d / fabs(dda->ray_dir.y / dda->ray_dir.x) * dda->sign.x;
	if (dda->ray_dir.y < 0)
	{
		dda->map_step.y = -1;
		dda->travelled_distance.y = vars->vecset->pos.y - dda->map_pos.y;
		// dda->travelled_distance.y *= dda->step_size.y;
		dda->travelled_distance.y *= fabs(dda->ray_dir.y / dda->ray_dir.x);
	}
	else
	{
		dda->map_step.y = 1;
		dda->travelled_distance.y = dda->map_pos.y + 1 - vars->vecset->pos.y;
		// dda->travelled_distance.y *= dda->step_size.y;
		dda->travelled_distance.y *= fabs(dda->ray_dir.y / dda->ray_dir.x);
	}
	// d = (int)vars->vecset->pos.x;
	// if (dda->sign.x > 0)
	// 	d = (int)vars->vecset->pos.x + 1;
	// d = fabs(d - vars->vecset->pos.x);
	// dda->travelled_distance.y = vars->vecset->pos.y + d * fabs(dda->ray_dir.y / dda->ray_dir.x);// * dda->sign.y;
}

t_lineinfo	calculate_dda_line(t_vars *vars, t_dda *dda)
{
	t_lineinfo	res;

	while (YES)
	{
		if (dda->travelled_distance.x < dda->travelled_distance.y)
		{
			dda->map_pos.x += dda->map_step.x;
			dda->hit_dir = WEST;
			if (dda->ray_dir.x > 0)
				dda->hit_dir = EAST;
			if (vars->map[dda->map_pos.y][dda->map_pos.x] == '1')
				break ;
			dda->travelled_distance.x += dda->step_size.x;
		}
		else
		{
			dda->map_pos.y += dda->map_step.y;
			dda->hit_dir = NORTH;
			if (dda->ray_dir.y > 0)
				dda->hit_dir = SOUTH;
			if (vars->map[dda->map_pos.y][dda->map_pos.x] == '1')
				break ;
			dda->travelled_distance.y += dda->step_size.y;
		}
	}
	double	x;
	double	y;

	res.distance_info.hit_direction = dda->hit_dir;
	res.distance_info.distance_to_wall = dda->travelled_distance.x;
	if (dda->hit_dir == NORTH || dda->hit_dir == SOUTH)
		res.distance_info.distance_to_wall = dda->travelled_distance.y;
	// printf("posx:%f posy:%f\n", vars->vecset->pos.x, vars->vecset->pos.y);
	// printf("dx:%f dy:%f\n", dda->travelled_distance.x, dda->travelled_distance.y);
	// res.distance_info.distance_to_wall
	// 	= sqrt(dda->travelled_distance.x * dda->travelled_distance.x
	// 		+ dda->travelled_distance.y * dda->travelled_distance.y);
	// res.distance_info.distance_to_wall = get_line_to_point_distance(
	// 		vars->vecset->plane.y / vars->vecset->plane.x, vars->vecset->pos,
	// 		(t_vec){.x = dda->travelled_distance.x * dda->sign.x + vars->vecset->pos.x,
	// 			.y = dda->travelled_distance.y * dda->sign.y + vars->vecset->pos.y});
	x = 1 - fabs(dda->travelled_distance.y - (int)dda->travelled_distance.y);
	if (dda->sign.x > 0)
		x = fabs(dda->travelled_distance.y - (int)dda->travelled_distance.y);
	y = fabs(dda->travelled_distance.x - (int)dda->travelled_distance.x);
	if (dda->sign.y > 0)
		y = 1 - fabs(dda->travelled_distance.x - (int)dda->travelled_distance.x);
	res.distance_info.distance_to_grid = sqrt(x * x + y * y);
	// res.line_height = 0;
	// if (res.distance_info.distance_to_wall > 0)
		res.line_height = WINDOW_HEIGHT / res.distance_info.distance_to_wall;
	return (res);
}

t_lineinfo	calculate_by_dda(t_vars *vars, t_vec raydir)
{
	t_vecset	vecset;
	t_grid		sign;
	t_distinfo	distance_x;
	t_distinfo	distance_y;
	t_lineinfo	line_info;

	vecset = *vars->vecset;
	sign.x = raydir.x / fabs(raydir.x);
	sign.y = raydir.y / fabs(raydir.y);
	distance_x = get_dist_info_x(vars, vecset, raydir, sign);
	distance_y = get_dist_info_y(vars, vecset, raydir, sign);
	line_info.distance_info = choose_dist(distance_x, distance_y, raydir);
	line_info.line_height = 0;
	if (line_info.distance_info.distance_to_wall)
		line_info.line_height
			= WINDOW_HEIGHT / line_info.distance_info.distance_to_wall;
	printf("rdirx:%f, rdiry:%f\n", raydir.x, raydir.y);
	printf("x-- dg:%f, dw:%f \n", distance_x.distance_to_grid, distance_x.distance_to_wall);
	printf("y-- dg:%f, dw:%f \n", distance_y.distance_to_grid, distance_y.distance_to_wall);
	printf("l-- dg:%f, dw:%f hdir:%d h:%d\n", line_info.distance_info.distance_to_grid, line_info.distance_info.distance_to_wall, line_info.distance_info.hit_direction, line_info.line_height);
	return (line_info);
}

// DDA: Digital Differential Analysis - line drawing algorithm in graphics
// raydir = dir + k * plane (k: -1 ~ 1)
t_lineinfo	get_line_info_by_dda(int i, t_vars *vars)
{
	t_vecset	vecset;
	t_vec		raydir;
	double		k;
	t_lineinfo	line_info;
	t_lineinfo	new_line_info;
	t_dda		dda;

	vecset = *vars->vecset;
	k = -1 + 2 * (double)i / (double)WINDOW_WIDTH;
	raydir.x = vecset.dir.x + k * vecset.plane.x;
	raydir.y = vecset.dir.y + k * vecset.plane.y;
	dda.ray_dir.x = vecset.dir.x + k * vecset.plane.x;
	dda.ray_dir.y = vecset.dir.y + k * vecset.plane.y;
	dda.sign.x = raydir.x / fabs(raydir.x);
	dda.sign.y = raydir.y / fabs(raydir.y);
	prepare_dda(vars, &dda);
	(void)dda;
	(void)new_line_info;
	(void)raydir;
	(void)line_info;
	new_line_info = calculate_dda_line(vars, &dda);
	// line_info = calculate_by_dda(vars, raydir);
	// printf("old-- height:%d dgrid:%f dwall:%f dir:%d\n",
	// 	line_info.line_height,
	// 	line_info.distance_info.distance_to_grid,
	// 	line_info.distance_info.distance_to_wall,
	// 	line_info.distance_info.hit_direction);
	// printf("new-- height:%d dgrid:%f dwall:%f dir:%d tdx:%f tdy:%f ssx:%f ssy:%f px:%d py:%d\n",
	// 	new_line_info.line_height,
	// 	new_line_info.distance_info.distance_to_grid,
	// 	new_line_info.distance_info.distance_to_wall,
	// 	new_line_info.distance_info.hit_direction,
	// 	dda.travelled_distance.x,
	// 	dda.travelled_distance.y,
	// 	dda.step_size.x,
	// 	dda.step_size.y,
	// 	dda.map_pos.x,
	// 	dda.map_pos.y);
	(void)line_info;
	return (new_line_info);
}
