import cube

Cube = cube.Rubiks_Cube(3)

# print(cube.position)

print(len(Cube.pieces))

Cube.show()

# for T in cube.Transformations_possible:
#     print(T.rotation_plane_coefficients[0], T.rotation_plane_coefficients[1], T.rotation_plane_coefficients[2])
#     cube.transform(T)

# T = Transform([1.5,0,0],1)
# cube.transform(T)
# cube.show()

Cube.scramble(cube.select_random_transformations(Cube.Transformations_possible,5))
# print(len(cube.pieces))

# cube.show()