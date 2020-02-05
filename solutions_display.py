import os
import matplotlib.pyplot as plt

SOLUTIONS_DIR = "solutions/"
SOLUTIONS_EXT = ".res"

PROBLEM = "p01"

def print_route(route, locations, nb_customer):
  color = ['r', 'b', 'g', 'y', 'c', 'm', 'k', 'g', 'r', 'c']#, 'orange', 'pink', 'purple', 'brown']
  for i in range(0,len(route)-1):
    #print([customers[route[i]][0], customers[route[i+1]][0]], [customers[route[i]][1], customers[route[i+1]][1]])
    plt.plot([locations[route[i]][0], locations[route[i+1]][0]], [locations[route[i]][1], locations[route[i+1]][1]], color[route[0]-1-nb_customer]+'-o')
    #plt.plot(locations[route[i]][0], locations[route[i]][1], color=color[route[0]-1-nb_customer], linestyle='dotted')

def print_depot(locations, nb_customer, nb_depot):
  for i in range(1,nb_depot+1):
    #print("depot ", i, ": ", locations[nb_customer+i][0], locations[nb_customer+i][1])
    plt.plot(locations[nb_customer+i][0], locations[nb_customer+i][1], 'ks')

def get_locations(pb):
  f = open("testing-data/"+pb, "r")
  index = 0
  locations = {}
  for line in f.readlines():
    i = line.split(" ")
    while(" " in i) : 
      i.remove(" ")
    while("" in i) :
      i.remove("")
    i[len(i)-1] = i[len(i)-1].rstrip('\n')
    if index == 0:
      nb_customer = int(i[1])
      nb_depot = int(i[2])
    elif (index > nb_depot):
      locations[int(i[0])] = (int(i[1]), int(i[2])) # dict with key = ID and value = (x, y) coordonates
    index += 1
  return nb_depot, nb_customer, locations

def read_solution_file(pb, nb_customer):
  """
  all_routes is a list of list containing all the routes of a solution.
  One route is a list of integers that starts and ends with the depot number.
  """
  f = open(SOLUTIONS_DIR+ pb + SOLUTIONS_EXT, "r")
  index = 0
  all_routes = [[]]
  for line in f.readlines():
    route=[]
    if index == 0:
      total_dist = line
    else:
      i = line.replace('\t', ' ').split(" ")
      i[len(i)-1] = i[len(i)-1].rstrip('\n')
      while(" " in i) : 
        i.remove(" ")
      while("" in i) :
        i.remove("")
      route.append(int(i[0])+nb_customer)
      for j in range(5, len(i)):
        route.append(int(i[j]))
      route.append(int(i[0])+nb_customer)
      all_routes.append(route)
    index += 1
  f.close()
  all_routes.remove(all_routes[0])
  return total_dist, all_routes


nb_depot, nb_customer, locations = get_locations(PROBLEM)
dist, all_routes = read_solution_file(PROBLEM, nb_customer)
for route in all_routes:
  print_route(route, locations, nb_customer)
print_depot(locations, nb_customer, nb_depot)
plt.show()
