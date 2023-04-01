/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sryou <sryou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 13:54:32 by junhelee          #+#    #+#             */
/*   Updated: 2023/04/01 14:44:28 by sryou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_step(t_ray *ray)
{
	if (ray->ray_x < 0)
		ray->step_x = -1;
	else
		ray->step_x = 1;
	if (ray->ray_y < 0)
		ray->step_y = -1;
	else
		ray->step_y = 1;
}

void	set_distance(t_ray *ray)
{
	if (ray->ray_x < 0)
		ray->distance_x = (ray->user_x - ray->map_x) * \
			ray->delta_distance_x;
	else
		ray->distance_x = (ray->map_x + 1 - ray->user_x) * \
			ray->delta_distance_x;
	if (ray->ray_y < 0)
		ray->distance_y = (ray->user_y - ray->map_y) * \
			ray->delta_distance_y;
	else
		ray->distance_y = (ray->map_y + 1 - ray->user_y) * \
			ray->delta_distance_y;
}

void	make_ray(t_conf *conf, t_ray *ray, int x)
{
	double	unit;

	unit = 2 * x / (double)WIDTH - 1;
	ray->ray_x = conf->cam_x + conf->fov_x * unit;
	ray->ray_y = conf->cam_y + conf->fov_y * unit;
	ray->map_x = conf->user_x;
	ray->map_y = conf->user_y;
	ray->user_x = conf->user_x;
	ray->user_y = conf->user_y;
	ray->delta_distance_x = fabs(1 / ray->ray_x);
	ray->delta_distance_y = fabs(1 / ray->ray_y);
}

void	calculate_distance(t_ray *ray)
{
	double	distance;
	
	if (ray->grid == X_DIRECTION)
		distance = (ray->distance_x - ray->delta_distance_x);
	else
		distance = (ray->distance_y - ray->delta_distance_y);
	ray->distance = distance;
}

void	calculate_ray(t_conf *conf, t_ray *ray, int x)
{
	int	hit;

	make_ray(conf, ray, x);
	set_step(ray);
	set_distance(ray);
	hit = FALSE;
	while (hit == FALSE)
	{
		if (ray->distance_x < ray->distance_y)
		{
			ray->distance_x += ray->delta_distance_x;
			ray->map_x += ray->step_x;
			ray->grid = X_DIRECTION;
		}
		else
		{
			ray->distance_y += ray->delta_distance_y;
			ray->map_y += ray->step_y;
			ray->grid = Y_DIRECTION;
		}
		if (conf->char_map[ray->map_y][ray->map_x] > 0)
			hit = TRUE;
	}
}

void	calculate(t_conf *conf)
{
	t_ray	ray;
	int		x;

	draw_map_3d(conf);
	x = 0;
	while (x < WIDTH)
	{
		calculate_ray(conf, &ray, x);
		calculate_distance(&ray);
		draw_object_3d(conf, &ray, WIDTH - x - 1);
		x++;
	}
	draw_map_2d(conf);
	x = 0;
	while (x < WIDTH)
	{
		calculate_ray(conf, &ray, x);
		calculate_distance(&ray);
		if (x % 10 == 0)
			draw_ray_2d(conf, &ray);
		x++;
	}
}
