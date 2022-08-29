#ifndef _CUBE_H
#define _CUBE_H
#include<iostream>
#include<vector>
#include<string>
using namespace std;


class Position{
    public:
    friend ostream &operator<<(ostream &os, const Position &obj);
    double x;
    double y;
    double z;
    vector<double>v;

    ~Position();
    Position(double x, double y, double z);
    bool operator==(const Position &p);
    double euclidean_dist(Position p);
};
ostream &operator<<(ostream &os, const Position &obj);

class Colour{
    public:
    friend ostream &operator<<(ostream &os, const Colour &obj);
    Position position;
    string colour;

    ~Colour();
    Colour(Position position, string colour);
};
ostream &operator<<(ostream &os, const Colour &obj);

class Face{
    public:
    friend ostream &operator<<(ostream &os, const Face &obj);
    int dim;
    string side;
    vector<Colour>colours;
    vector<int>normal_v;

    ~Face();
    Face(vector<Colour>colours, string side, int dim);
};
ostream &operator<<(ostream &os, const Face &obj);

class Piece{
    public:
    friend ostream &operator<<(ostream &os, const Piece &obj);
    Position point;
    vector<Colour>colours;
    string type;

    ~Piece();
    Piece(Position point, vector<Colour>colours, string type);
};
ostream &operator<<(ostream &os, const Piece &obj);

class Matrix{
    public:
    friend ostream &operator<<(ostream &os, const Matrix &obj);

    vector<vector<double>>matrix;

    ~Matrix();
    Matrix(vector<vector<double>>matrix = {{1,0,0},{0,1,0},{0,0,1}});

    void multiply(vector<double> &v);
    void rotate(Piece &p);
    void transpose();
};
ostream &operator<<(ostream &os, const Matrix &obj);

class Transformation{
    public:
    friend ostream &operator<<(ostream &os, const Transformation &obj);
    string layer_move;
    Matrix transformation_matrix;
    int layer_index;
    int dir_coeff;

    ~Transformation();
    Transformation(string layer_move);
};
Transformation invert_transform(Transformation T);
ostream &operator<<(ostream &os, const Transformation &obj);

class Cube{
    protected:
    vector<Transformation>transforms_to_solve;
    
    public:
    friend ostream &operator<<(ostream &os, const Cube &obj);
    int order;
    vector<Piece>pieces;
    vector<Face>faces;
    vector<Piece>solved_state;
    vector<Piece>solved_pieces;

    vector<Transformation>possible_transforms;

    void generate_pieces_and_faces();
    void group_faces();
    void generate_possible_transforms();

    ~Cube();
    Cube(int order);

    bool is_solved();

    void transform(Transformation T);
    vector<Transformation>random_transforms(int n);

    void reorient();

    void scramble(int n);
    void scramble(vector<Transformation>transforms);

    double how_close_to_solved(Piece &p);
    int best_transform_index(Piece &p, Colour &c, bool &need_commutator, int &commutator_order);

    void solve_colour_safely(Piece &p, Colour &c, vector<Transformation>&moves_to_solve);
    void solve_first_layer(vector<Transformation>&moves_to_solve);
    void solve_top_layer(vector<Transformation>&moves_to_solve);
    vector<Transformation>human_solve();

    void solve();

    bool operator==(const Cube &c);
};
ostream &operator<<(ostream &os, const Cube &obj);

#endif