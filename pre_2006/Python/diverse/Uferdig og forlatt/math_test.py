import math
# area of circle

def area_points(xc, yc, xp, yp):
	radius = distance(xc, yc, xp, yp)
	result = area(radius)
	return result
	

def area(radius):
	temp = math.pi * radius**2
	return temp
	
def distance(xc, yc, xp, yp):
	dx = xp - xc
	dy = yp - yc
	#pythagoras
	distance = math.sqrt( dx**2 + dy**2)
	return distance


if __name__ == "__main__":
	print "Arealet av sirkelen definert paa (0,0)(0,1), er "\
	 + str(area_points(0,0,0,1))
