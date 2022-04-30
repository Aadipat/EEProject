from asyncio import constants
from cmath import sqrt
from turtle import color
from matplotlib import markers
import numpy as np
import math
from numpy import linalg as la, power
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
from matplotlib import cm
import random
import plotly.graph_objects as go
import plotly.figure_factory as ff
from fractions import Fraction as frac
# class for colour
class Colour:
    def __init__(self,vector,colour):
        self.vector = vector
        self.colour = colour
#class for standard cube piece
class Piece:
    def __init__(self,x,y,z, n):
        self.x = x
        self.y = y
        self.z = z
        self.colour = [random.random(),random.random(),random.random()]
        self.initial_vector = np.array([x,y,z])
        self.vector = np.array([x,y,z])
        self.colours = []
        if(self.x == (n-1)/2):
            self.colours.append(Colour(np.array([1,0,0]),'red'))
        if(self.x == -(n-1)/2):
            self.colours.append(Colour(np.array([-1,0,0]),'orange'))
        if(self.y == (n-1)/2):
            self.colours.append(Colour(np.array([0,1,0]),'white'))
        if(self.y == -(n-1)/2):
            self.colours.append(Colour(np.array([0,-1,0]),'yellow'))
        if(self.z == (n-1)/2):
            self.colours.append(Colour(np.array([0,0,1]),'blue'))
        if(self.z == -(n-1)/2):
            self.colours.append(Colour(np.array([0,0,-1]),'green'))
#function multiply matrices
def multiply_pieces_matrix(pieces, matrix):
    for v in pieces:
        v.vector = np.matmul(v.vector,matrix)
        for c in v.colours:
            c.vector = np.matmul(c.vector,matrix)
    return pieces

class Transform:
    def __init__(self, rotation_plane_coefficients, direction):
        self.rotation_plane_coefficients = rotation_plane_coefficients
        self.direction = direction
    def find_rotation_matrix(self,c):
        rotation_matrix = np.empty([3,3])
        # print(c)
        if(c == 0):
            rotation_matrix = np.array([[1,0,0],
                                        [0,0,-1],
                                        [0,1,0]])
            if(self.direction == -1):
                rotation_matrix = la.inv(rotation_matrix)
        elif(c == 1):
            rotation_matrix = np.array([[0,0,1],
                                        [0,1,0],
                                        [-1,0,0]])
            if(self.direction == -1):
                rotation_matrix = la.inv(rotation_matrix)
        elif(c == 2):    
            rotation_matrix = np.array([[0,-1,0],
                                        [1,0,0],
                                        [0,0,1]])
            if(self.direction == -1):
                rotation_matrix = la.inv(rotation_matrix)
        return rotation_matrix

def select_random_transformations(transformations, number):
    transformations_to_be_done = []
    for i in range(number):
        index = random.randint(0,len(transformations)-1)
        transformations_to_be_done.append(transformations[index])
    return transformations_to_be_done

class Rubiks_Cube:
    def generatePieces(self,n):
        mod_extremity = ((self.n-1)/2)
        i = -mod_extremity
        # num_pieces = math.pow(n,3) - math.pow(n-2,3)\
        interval_dist = 1
        # print(interval_dist)
        pieces = []
        # Not necessary, just because python does not work in exact arithmetic. 
        max_val = mod_extremity
        while(i <= max_val):
            j = -mod_extremity
            while(j <= max_val):
                k = -mod_extremity
                while(k <= max_val):
                    if(abs(i) < mod_extremity and abs(j) < mod_extremity and abs(k) < mod_extremity):
                        # print(i,j,k)
                        k+=interval_dist
                        continue
                    else:
                        p = Piece(i,j,k,n)
                        pieces.append(p)
                    k+=interval_dist
                j+=interval_dist
            i+=interval_dist
        pieces = np.flip(np.array(pieces))
        return pieces
    def generate_Possible_Transformations(self):
        possible_transformations = []
        interval_dist = 1
        for n in range(self.n):
            i = - ((self.n - 1)/2) + n*interval_dist
            if(i != 0):
                T1 = Transform([i,0,0],1)
                T2 = Transform([i,0,0],-1)
                possible_transformations.append(T1)
                possible_transformations.append(T2)
        for n in range(self.n):
            j = - ((self.n - 1)/2) + n*interval_dist
            if(j != 0):
                T1 = Transform([0,j,0],1)
                T2 = Transform([0,j,0],-1)
                possible_transformations.append(T1)
                possible_transformations.append(T2)
        for n in range(self.n):
            k = - ((self.n - 1)/2) + n*interval_dist
            if(k != 0):
                T1 = Transform([0,0,k],1)
                T2 = Transform([0,0,k],-1)
                possible_transformations.append(T1)
                possible_transformations.append(T2)
        possible_transformations = np.array(possible_transformations)
        return possible_transformations
    def build_layers(self, pieces,n):
        all_layers = []
        axes = 0
        for axes in range(3):
            layers_from_one_axis = []
            val = -((self.n - 1)/2)
            while(val <= +((self.n - 1)/2)):
                layer = []
                for e in range(len(pieces)):
                    if(pieces[e].vector[axes] == val):
                        layer.append(pieces[e])
                layers_from_one_axis.append(np.array(layer, dtype=object))
                val+=1
            all_layers.append(np.array(layers_from_one_axis, dtype=object))
            axes+=1
        for i in range(len(all_layers)):
            all_layers[i] = all_layers[i]
        return all_layers
    def update_position_vectors(self,position):
        position_vectors = []
        for axes in position:
            axes_perspective = []
            for l in axes:
                layer = []
                for p in l:
                    layer.append(p.vector)
                layer = np.array(layer)
                axes_perspective.append(layer)
            axes_perspective = np.array(axes_perspective, dtype=object)
            position_vectors.append(axes_perspective)
        return position_vectors
    def update_piece_vectors(self,pieces):
        piece_vectors = []
        for p in self.pieces:
            piece_vectors.append(np.array(p.vector))
        return np.array(piece_vectors)   
    def __init__(self, n):
        self.n = n
        self.pieces = self.generatePieces(self.n)
        self.piece_vectors = []
        
        self.piece_vectors = np.array(self.update_piece_vectors(self.pieces))
        # print(self.pieces)
        # print(len(self.pieces))
        self.position = self.build_layers(self.pieces, self.n)
        self.position_vectors = self.update_position_vectors(self.position)
        
        # print(self.position)
        self.solved_position = self.position

        self.Transformations_possible = self.generate_Possible_Transformations()

        self.transformations_from_solution = []
    def transform(self,transformation):
        c = 0
        while(c<len(transformation.rotation_plane_coefficients)):
            if(transformation.rotation_plane_coefficients[c] != 0):
                break
            c+=1
        rotation_matrix = transformation.find_rotation_matrix(c)

        if(len(self.transformations_from_solution) > 0):
            latest_rotation_matrix = self.transformations_from_solution[len(self.transformations_from_solution) - 1].find_rotation_matrix(c)
            if(np.all(rotation_matrix) != np.all(la.inv(latest_rotation_matrix))):
                self.transformations_from_solution.append(transformation)
            else:
                self.transformations_from_solution.pop()
        else:
            self.transformations_from_solution.append(transformation)
        operating_perspective = self.position[c]
        # print(len(operating_perspective))
        l_index = 0
        try:
            while(l_index < len(operating_perspective)):
                # print("Perspective")
                # print(len(self.position[c][l_index]))
                if(operating_perspective[l_index][0].vector[c] == transformation.rotation_plane_coefficients[c]):
                    # print(rotation_matrix)
                    operating_perspective[l_index] = multiply_pieces_matrix(operating_perspective[l_index],rotation_matrix)
                    break  
                l_index += 1
        except:
            print("Problem", len(operating_perspective[l_index]))   
            print(c, l_index, len(operating_perspective[l_index]), len(self.position_vectors), len(self.pieces))
      
        updated_pieces_from_one_perspective = []
        if(c != 2 and c != 1):
            zval = -((self.n - 1)/2)
            while(zval<=((self.n - 1)/2)):
                for l in operating_perspective:
                    # print("Need to append vectors")
                    for v in l:
                        # print(v)
                        if(v.vector[2] == zval):
                            updated_pieces_from_one_perspective.append(v)
                zval+=1
        else:
            for l in operating_perspective:
                for v in l:
                    updated_pieces_from_one_perspective.append(v)
        self.position = self.build_layers(updated_pieces_from_one_perspective, self.n)
        self.position_vectors = self.update_position_vectors(self.position)
        self.piece_vectors = self.update_piece_vectors(self.pieces)
    def scramble(self,transforms):
        for t in transforms:
            print(t.rotation_plane_coefficients, t.direction)
            self.transform(t)
    def show(self):
        # matplotlib
        fig = plt.figure()
        # syntax for 3-D projection
        ax = fig.gca(projection ='3d')
        x = []
        y = []
        z = []
        colours_ix = []
        colours_iy = []
        colours_iz = []
        colours_fx = []
        colours_fy = []
        colours_fz = []
        # p_colours = []
        c_colours = []
        for i in self.pieces:
            x.append(i.vector[0])
            y.append(i.vector[1])
            z.append(i.vector[2])
            # p_colours.append(i.colour)
            for c in i.colours:
                colours_ix.append(i.vector[0])
                colours_iy.append(i.vector[1])
                colours_iz.append(i.vector[2])
                colours_fx.append(c.vector[0])
                colours_fy.append(c.vector[1])
                colours_fz.append(c.vector[2])
                c_colours.append(c.colour)
        
        # Matplotlib
        xLabel = ax.set_xlabel('X-axis', linespacing=3.2)
        yLabel = ax.set_ylabel('Y-axis', linespacing=3.1)
        zLabel = ax.set_zlabel('Z-Axis', linespacing=3.4)
        # plotting
        # print(len(x),len(y), len(z), len(colours_fx), len(colours_fy), len(colours_fz))
        ax.scatter(x, y, z, c = 'black')
        ax.quiver(colours_ix,colours_iy,colours_iz,colours_fx,colours_fy,colours_fz, color = c_colours, length=0.5, arrow_length_ratio = 0)
        plt.show()

# Planes of rotation. For 3x3
# R: 1x + 0y + 0z = 1
# L: 1x + 0y + 0z = -1
# U: 0x + 1y + 0z = 1
# D: 0x + 1y + 0z = -1
# F: 0x + 0y + 1z = 1
# B: 0x + 0y + 1z = -1

# Number of perspectives = faces/2 for Megaminx and Cube. 
# However tetrahedral pyraminx has 4.

# When solving;
# Find well known groups and permutations of transformations.
# Possibly, machine learning, unsupervised. 

