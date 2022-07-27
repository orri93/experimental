import math

def pizza_area(r, a):
  return math.pi * r * r * a / 360

radius_a = 6
angle_a = 60
price_a = 1.5

radius_b = 7
angle_b = 45
price_b = 1.7

area_a = pizza_area(radius_a, angle_a)
area_b = pizza_area(radius_b, angle_b)

pizza_per_price_a = area_a / price_a
pizza_per_price_b = area_b / price_b

print(pizza_per_price_a)
print(pizza_per_price_b)

crust_radius_ratio_a = 1/11
crust_radius_ratio_b = 1/14

crust_radius_a = radius_a * crust_radius_ratio_a
crust_radius_b = radius_b * crust_radius_ratio_b

pizza_radius_a = radius_a - crust_radius_a
pizza_radius_b = radius_b - crust_radius_b

pizza_area_a = pizza_area(pizza_radius_a, angle_a)
pizza_area_b = pizza_area(pizza_radius_b, angle_b)

crust_area_a = area_a - pizza_area_a
crust_area_b = area_b - pizza_area_b

crust_ratio_a = crust_area_a / area_a
crust_ratio_b = crust_area_b / area_b

print(crust_ratio_a)
print(crust_ratio_b)
