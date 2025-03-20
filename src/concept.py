import pygame
from math import floor, ceil

win = pygame.display.set_mode((500,500))

triangle = [
	(0, 0.5),
	(-0.5, -0.25),
	(0.5, -0.5)
]

colors = [
	(255, 255, 0),
	(0, 255, 0),
	(0, 0, 0)
]

def edgefunc(a:tuple[float,float], b:tuple[float,float], p:tuple[float,float]) -> bool:
	return (p[0] - a[0]) * (b[1] - a[1]) - (p[1]-a[1]) * (b[0] - a[0])

def is_in_triangle(t:list, p:tuple[float,float]):
	area = edgefunc(t[0],t[1],t[2])
	w0 = edgefunc(t[0],t[1],p)
	w1 = edgefunc(t[1],t[2],p)
	w2 = edgefunc(t[2],t[0],p)
	if w0 <= 0 and w1 <= 0 and w2 <= 0:
		w0 /= area;
		w1 /= area;
		w2 /= area;
		return (w0,w1,w2)
	else: return None

def interpolate_colors(c:list, b:tuple[float,float,float]) -> tuple[float, float, float]:
	return (
		(c[0][0] * b[0] + c[1][0] * b[1] + c[2][0] * b[2]),
		(c[0][1] * b[0] + c[1][1] * b[1] + c[2][1] * b[2]),
		(c[0][2] * b[0] + c[1][2] * b[1] + c[2][2] * b[2])
	)

def get_triangle_bounds(t:list) -> tuple[float,float,float,float]:
	bounds = (
		floor((min(t[0][0],t[1][0],t[2][0])+0.5)*500),
		ceil((max(t[0][0],t[1][0],t[2][0])+0.5)*500),
		floor((min(t[0][1],t[1][1],t[2][1])+0.5)*500),
		ceil((max(t[0][1],t[1][1],t[2][1])+0.5)*500),
	)
	return bounds

def px_to_space(x,y) -> tuple[float,float]:
	return ((x+0.5)/500-0.5,-((y+0.5)/500-0.5))

def render_triangle(t:list) -> None:
	xmin, xmax, ymin, ymax = get_triangle_bounds(t)
	for y in range(ymin, ymax, 1):
		for x in range(xmin, xmax, 1):
			pt = px_to_space(x,y)
			b = is_in_triangle(t, pt)
			if not b == None:
				win.set_at((x,y),interpolate_colors(colors,b))

while True:
	for e in pygame.event.get():
		if e.type == pygame.QUIT:
			pygame.quit()
			exit()

	win.fill((0,0,0))

	render_triangle(triangle)

	pygame.display.update()