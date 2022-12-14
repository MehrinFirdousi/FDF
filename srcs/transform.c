/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfirdous <mfirdous@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 13:25:08 by mfirdous          #+#    #+#             */
/*   Updated: 2022/12/14 21:21:14 by mfirdous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	rotate(t_point *point, t_mlx *mlx)
{
	point->x_3d = (point->x * mlx->scale);
	point->y_3d = (point->y * mlx->scale);
	point->z_3d = (point->z * mlx->scale);

	matrix_mul(mlx);
	point->x_3d = (point->x_3d * mlx->r[0][0]) + (point->y_3d * mlx->r[0][1]) + (point->z_3d * mlx->r[0][2]);
	point->y_3d = (point->x_3d * mlx->r[1][0]) + (point->y_3d * mlx->r[1][1]) + (point->z_3d * mlx->r[1][2]);

	point->x_3d += mlx->x_offset;
	point->y_3d += mlx->y_offset;
	// point->z_3d += mlx->y_offset + 5 * mlx->scale;
}

/*
00 01 02	00 01 02
10 11 12	10 11 12
20 21 22	20 21 22
yaw pitch roll
*/
void	matrix_mul(t_mlx *mlx)
{
	double a = deg_to_rad(mlx->a);
	double b = deg_to_rad(mlx->b);
	double c = deg_to_rad(mlx->c);
	

	double r1[3][3] = {	{cos(a),	-sin(a),	0},\
						{sin(a),	cos(a),		0},\
						{0,			0,			1}};
	
	double r2[3][3] = {	{cos(b),	0,			sin(b)},\
						{0,			1,			0},\
						{-sin(b),	0,			cos(b)}};
	
	double r3[3][3] = {	{1,			0,			0},\
						{0,			cos(c),		-sin(c)},\
						{0,			sin(c),		cos(c)}};
	
	mlx->r[0][0] = r1[0][0] * r2[0][0] + r1[0][1] * r2[1][0] + r1[0][2] * r2[2][0];
	mlx->r[0][1] = r1[0][0] * r2[0][1] + r1[0][1] * r2[1][1] + r1[0][2] * r2[2][1];
	mlx->r[0][2] = r1[0][0] * r2[0][2] + r1[0][1] * r2[1][2] + r1[0][2] * r2[2][2];
	
	mlx->r[1][0] = r1[1][0] * r2[0][0] + r1[1][1] * r2[1][0] + r1[1][2] * r2[2][0];
	mlx->r[1][1] = r1[1][0] * r2[0][1] + r1[1][1] * r2[1][1] + r1[1][2] * r2[2][1];
	mlx->r[1][2] = r1[1][0] * r2[0][2] + r1[1][1] * r2[1][2] + r1[1][2] * r2[2][2];
	
	mlx->r[2][0] = r1[2][0] * r2[0][0] + r1[2][1] * r2[1][0] + r1[2][2] * r2[2][0];
	mlx->r[2][1] = r1[2][0] * r2[0][1] + r1[2][1] * r2[1][1] + r1[2][2] * r2[2][1];
	mlx->r[2][2] = r1[2][0] * r2[0][2] + r1[2][1] * r2[1][2] + r1[2][2] * r2[2][2];	
	
	
	mlx->r[0][0] = mlx->r[0][0] * r3[0][0] + mlx->r[0][1] * r3[1][0] + mlx->r[0][2] * r3[2][0];
	mlx->r[0][1] = mlx->r[0][0] * r3[0][1] + mlx->r[0][1] * r3[1][1] + mlx->r[0][2] * r3[2][1];
	mlx->r[0][2] = mlx->r[0][0] * r3[0][2] + mlx->r[0][1] * r3[1][2] + mlx->r[0][2] * r3[2][2];
	
	mlx->r[1][0] = mlx->r[1][0] * r3[0][0] + mlx->r[1][1] * r3[1][0] + mlx->r[1][2] * r3[2][0];
	mlx->r[1][1] = mlx->r[1][0] * r3[0][1] + mlx->r[1][1] * r3[1][1] + mlx->r[1][2] * r3[2][1];
	mlx->r[1][2] = mlx->r[1][0] * r3[0][2] + mlx->r[1][1] * r3[1][2] + mlx->r[1][2] * r3[2][2];
	
	mlx->r[2][0] = mlx->r[2][0] * r3[0][0] + mlx->r[2][1] * r3[1][0] + mlx->r[2][2] * r3[2][0];
	mlx->r[2][1] = mlx->r[2][0] * r3[0][1] + mlx->r[2][1] * r3[1][1] + mlx->r[2][2] * r3[2][1];
	mlx->r[2][2] = mlx->r[2][0] * r3[0][2] + mlx->r[2][1] * r3[1][2] + mlx->r[2][2] * r3[2][2];	

}

void	matrix_mul2(t_mlx *mlx)
{
	double a = deg_to_rad(mlx->a);
	double b = deg_to_rad(mlx->b);
	
	// xy - trans
	// double r1[3][3] = {	{1,			0,			0},\
	// 					{0,			cos(a),		sin(a)},\
	// 					{0,			-sin(a),	cos(a)}};

	// double r2[3][3] = {	{cos(b),	0,			-sin(b)},\
	// 					{0,			1,			0},\
	// 					{sin(b),	0,			cos(b)}};
	
	// xy
	// double r1[3][3] = {	{1,			0,			0},\
	// 					{0,			cos(a),		-sin(a)},\
	// 					{0,			sin(a),		cos(a)}};

	// double r1[3][3] = {	{cos(b),	0,			sin(b)},\
	// 					{0,			1,			0},\
	// 					{-sin(b),	0,			cos(b)}};
	
	// xz
	double r1[3][3] = {	{1,			0,			0},\
						{0,			cos(a),		-sin(a)},\
						{0,			sin(a),		cos(a)}};

	double r2[3][3] = {	{cos(b),	-sin(b),	0},\
						{sin(b),	cos(b),		0},\
						{0,			0,			1}};

	// xz - trans
	// double r1[3][3] = {	{1,			0,			0},\
	// 					{0,			cos(a),		sin(a)},\
	// 					{0,			-sin(a),	cos(a)}};

	// double r2[3][3] = {	{cos(b),	sin(b),		0},\
	// 					{-sin(b),	cos(b),		0},\
	// 					{0,			0,			1}};

	// yz
	// double r1[3][3] = {	{cos(a),	0,			sin(a)},\
	// 					{0,			1,			0},\
	// 					{-sin(a),	0,			cos(a)}};
	// double r2[3][3] = {	{cos(b),	-sin(b),		0},\
	// 					{sin(b),	cos(b),		0},\
	// 					{0,			0,			1}};
	
	// yz - trans
	// double r1[3][3] = {	{cos(a),	0,			-sin(a)},\
	// 					{0,			1,			0},\
	// 					{sin(a),	0,			cos(a)}};
	// double r2[3][3] = {	{cos(b),	sin(b),		0},\
	// 					{-sin(b),	cos(b),		0},\
	// 					{0,			0,			1}};
	
	(void)a;
	(void)b;
	mlx->r[0][0] = r1[0][0] * r2[0][0] + r1[0][1] * r2[1][0] + r1[0][2] * r2[2][0];
	mlx->r[0][1] = r1[0][0] * r2[0][1] + r1[0][1] * r2[1][1] + r1[0][2] * r2[2][1];
	mlx->r[0][2] = r1[0][0] * r2[0][2] + r1[0][1] * r2[1][2] + r1[0][2] * r2[2][2];
	
	mlx->r[1][0] = r1[1][0] * r2[0][0] + r1[1][1] * r2[1][0] + r1[1][2] * r2[2][0];
	mlx->r[1][1] = r1[1][0] * r2[0][1] + r1[1][1] * r2[1][1] + r1[1][2] * r2[2][1];
	mlx->r[1][2] = r1[1][0] * r2[0][2] + r1[1][1] * r2[1][2] + r1[1][2] * r2[2][2];
	
	mlx->r[2][0] = r1[2][0] * r2[0][0] + r1[2][1] * r2[1][0] + r1[2][2] * r2[2][0];
	mlx->r[2][1] = r1[2][0] * r2[0][1] + r1[2][1] * r2[1][1] + r1[2][2] * r2[2][1];
	mlx->r[2][2] = r1[2][0] * r2[0][2] + r1[2][1] * r2[1][2] + r1[2][2] * r2[2][2];	
}