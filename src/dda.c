/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mayeung <mayeung@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:48:23 by yublee            #+#    #+#             */
/*   Updated: 2025/05/08 16:53:21 by mayeung          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	prepare_dda(t_vars *vars, t_dda *dda)
{
	if (!vars || !dda)
		return ;
	dda->map_pos.x = (int)vars->vecset->pos.x;
	dda->map_pos.y = (int)vars->vecset->pos.y;
	dda->step_size.x = 1e30;
	if (fabs(dda->ray_dir.x) > EPSILON)
		dda->step_size.x = fabs(1 / dda->ray_dir.x);
	dda->step_size.y = 1e30;
	if (fabs(dda->ray_dir.y) > EPSILON)
		dda->step_size.y = fabs(1 / dda->ray_dir.y);
	if (dda->ray_dir.x < 0)
	{
		dda->map_step.x = -1;
		dda->travelled_distance.x = (vars->vecset->pos.x - dda->map_pos.x) * dda->step_size.x;
	}
	else
	{
		dda->map_step.x = 1;
		dda->travelled_distance.x = (dda->map_pos.x - vars->vecset->pos.x + 1) * dda->step_size.x;
	}
	if (dda->ray_dir.y < 0)
	{
		dda->map_step.y = -1;
		dda->travelled_distance.y = (vars->vecset->pos.y - dda->map_pos.y) * dda->step_size.y;
	}
	else
	{
		dda->map_step.y = 1;
		dda->travelled_distance.y = (dda->map_pos.y - vars->vecset->pos.y + 1) * dda->step_size.y;
	}
}

static t_lineinfo	calculate_dda_line(t_vars *vars, t_dda *dda)
{
	t_lineinfo	res;

	while (YES)
	{
		if (dda->travelled_distance.x < dda->travelled_distance.y)
		{
			dda->travelled_distance.x += dda->step_size.x;
			dda->map_pos.x += dda->map_step.x;
			if (dda->ray_dir.x > 0)
				dda->hit_dir = EAST;
			else
				dda->hit_dir = WEST;
		}
		else
		{
			dda->travelled_distance.y += dda->step_size.y;
			dda->map_pos.y += dda->map_step.y;
			if (dda->ray_dir.y > 0)
				dda->hit_dir = NORTH;
			else
				dda->hit_dir = SOUTH;
		}
		if (vars->map[dda->map_pos.y][dda->map_pos.x] == 1)
			break ;
	}
	res.distance_info.hit_direction = dda->hit_dir;
	res.distance_info.distance_to_wall = sqrt(dda->travelled_distance.x * dda->travelled_distance.x + dda->travelled_distance.y * dda->travelled_distance.y);
	
	// res.distance_info.
	return (res);
}

static t_lineinfo	calculate_by_dda(t_vars *vars, t_vec raydir)
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
	line_info = calculate_by_dda(vars, raydir);
	return (line_info);
}
