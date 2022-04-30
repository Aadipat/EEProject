import cube

Cube = cube.Rubiks_Cube(2)

Cube.show()

Cube.scramble(cube.select_random_transformations(Cube.Transformations_possible,2))

Cube.show()