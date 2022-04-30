import cube

Cube = cube.Rubiks_Cube(2)

Cube.show()

print(Cube.piece_vectors)
Cube.scramble(cube.select_random_transformations(Cube.Transformations_possible,2))
# print(len(cube.pieces))

print(Cube.piece_vectors)
Cube.show()