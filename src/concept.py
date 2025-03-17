import pygame
import math

pygame.init()

win = pygame.display.set_mode((800,800))
pygame.display.set_caption("Hello world!")

triangle = [
	(0,0.5),
	(-0.5,-0.5),
	(0.5,-0.5)
]

def getfunc(a:tuple[float,float],b:tuple[float,float]) -> tuple[float, float]:
	deltaX = a[1]-b[1]
	if deltaX != 0:
		a = (a[0]-b[0])/(a[1]-b[1])
	else:
		a = 1000000000000
	b = b[0]-a*b[1]
	return (a,b)

def func(f:tuple[float,float], y:float):
	return f[0]*y+f[1]

def minmax(a:tuple[float,float], b:tuple[float,float]) -> tuple[float,float]:
	return (min(a[1],b[1]),max(a[1],b[1]))

def scanline():
	edges = [
		getfunc(triangle[0],triangle[1]),
		getfunc(triangle[1],triangle[2]),
		getfunc(triangle[2],triangle[0])
	]
	edge_lim = [
		minmax(triangle[0],triangle[1]),
		minmax(triangle[1],triangle[2]),
		minmax(triangle[2],triangle[0])
	]
	for y in range(0,800):
		sline=[]
		for i in range(0,3):
			x = func(edges[i], -(y/800-0.5))
			if x >= edge_lim[i][0] and x <= edge_lim[i][1]:
				sline.append(x)
		if len(sline) != 0:
			sline[0] = int((sline[0]+0.5)*800)
			sline[1] = int((sline[1]+0.5)*800)
			for i in range(min(sline[0],sline[1]),max(sline[0],sline[1])):
				win.set_at((i,y),(255,255,255))
while True:
	for e in pygame.event.get():
		if e.type == pygame.QUIT:
			pygame.quit()
			exit()

	win.fill((0,0,0))

	scanline()

	pygame.display.update()