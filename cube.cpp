#include "cube.h"
#include<iostream>
#include<vector>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<string>
#include<fstream>

using namespace std;



bool Position::operator==(const Position &p){
    for(int e = 0;e<v.size();e++){
        if(v[e] != p.v[e]){
            return false;
        }
    }
    return true;
}

Position::~Position(){}

Position::Position(double x, double y, double z)
          :x{x},y{y},z{z}{
            v.push_back(x);
            v.push_back(y);
            v.push_back(z);
          }

ostream &operator<<(ostream &os, const Position &obj){
    return os << "(" << obj.v[0] << "," << obj.v[1] << "," << obj.v[2] << ")" << endl;
}

double Position::euclidean_dist(Position p){
    return abs(v[0] - p.v[0]) 
        + abs(v[1] - p.v[1]) 
        + abs(v[1] - p.v[1]);
}

Colour::~Colour(){}

Colour::Colour(Position position, string colour)
        :position{position},colour{colour}{}

ostream &operator<<(ostream &os, const Colour &obj){
    return os << "Colour: " << obj.colour << " at " << obj.position;
}


Face::~Face(){}

Face::Face(vector<Colour>colours, string side, int dim)
      :colours{colours}, side{side}, dim{dim}{
        if(side == "Up"){
            normal_v = {0,1,0};
        } else if(side == "Down"){
            normal_v = {0,-1,0};
        } else if(side == "Left"){
            normal_v = {-1,0,0};
        } else if(side == "Right"){
            normal_v = {1,0,0};
        } else if(side == "Front"){
            normal_v = {0,0,1};
        } else if(side == "Back"){
            normal_v = {0,0,-1};
        } 
      }

ostream &operator<<(ostream &os, const Face &obj){
    // os << endl << obj.side << ": ";
    string space;
    for(int s = 0;s<2*obj.dim;s++){
        space.push_back(' ');
    }
    for(int i = 0;i<obj.colours.size();i++){
        if(i%obj.dim == 0){
            os << endl << space;
        }
        os << obj.colours[i].colour << " ";
    }
    return os;
}


Piece::~Piece(){}

Piece::Piece(Position point, vector<Colour>colours, string type)
       :point{point},colours{colours}, type{type}{}

ostream &operator<<(ostream &os, const Piece &obj){
    os << "Piece: " << "Type; " << obj.type << endl << "Colours; {";
    
    for(auto c: obj.colours){
        os << c;
    } 
    
    os << "} Position; " << obj.point;
    return os;
}


Matrix::~Matrix(){}

Matrix::Matrix(vector<vector<double>>matrix)
        :matrix{matrix}{}

void Matrix::multiply(vector<double>&v){
    vector<double>res;
    for(int i = 0;i<matrix.size();i++){
        double val = 0;
        for(int j = 0;j<v.size();j++){
            val+=matrix[i][j]*v[j];
        }
        res.push_back(val);
    }
    v = res;
}

void Matrix::rotate(Piece &p){
    multiply(p.point.v);
    for(int c = 0;c<p.colours.size();c++){
        multiply(p.colours[c].position.v);
    }
}

void Matrix::transpose(){
    vector<vector<double>>transpose = matrix;
    for(int i = 0;i<matrix.size();i++){
        for(int j = 0;j<matrix[i].size();j++){
            transpose[i][j] = matrix[j][i];
        }
    }
    matrix = transpose;
}

ostream &operator<<(ostream &os, const Matrix &obj){
    for(auto v:obj.matrix){
        for(auto e:v){
            os << e << " ";
        }
        os << endl;
    }
    return os;
}


Transformation::~Transformation(){}

Transformation::Transformation(string layer_move)
                :layer_move{layer_move}{
                    bool is_p = false;
                    string layer;
                    for(char c:layer_move){
                        if(c == '*'){
                            is_p = true;
                        }else{
                            layer.push_back(c);
                        }
                    }
                    if(layer_move[0] == 'R' || layer_move[0] == 'L'){
                        if(is_p == true){
                            transformation_matrix = Matrix({{1,0,0},
                                                            {0,0,-1},
                                                            {0,1,0}});
                        } else{
                            transformation_matrix = Matrix({{1,0,0},
                                                            {0,0,1},
                                                            {0,-1,0}});
                        }
                    }
                    else if(layer_move[0] == 'U' || layer_move[0] == 'D'){
                        if(is_p == true){
                            transformation_matrix = Matrix({{0,0,-1},
                                                            {0,1,0},
                                                            {1,0,0}});
                        }
                        else{
                            transformation_matrix = Matrix({{0,0,1},
                                                            {0,1,0},
                                                            {-1,0,0}});
                        }
                    }
                    else{
                        if(is_p == true){
                            transformation_matrix = Matrix({{0,-1,0},
                                                            {1,0,0},
                                                            {0,0,1}});
                        } 
                        else{
                            transformation_matrix = Matrix({{0,1,0},
                                                            {-1,0,0},
                                                            {0,0,1}});
                        }
                    }
                    string layer_i;
                    for(int n = 1;n<layer_move.size();n++){
                        if(layer_move[n] == '*'){
                            break;
                        }
                        layer_i.push_back(layer_move[n]);
                    }
                    layer_index = stoi(layer_i);
            }

Transformation invert_transform(Transformation T){
    if(T.layer_move[T.layer_move.size()-1] == '*'){
        T.layer_move.pop_back();
        T.transformation_matrix.transpose();
    }
    else{
        T.layer_move.push_back('*');
        T.transformation_matrix.transpose();
    }
    return T;
}

ostream &operator<<(ostream &os, const Transformation &obj){
    return os << obj.layer_move << " ";
}



bool Cube::operator==(const Cube &c){
    if(order != c.order){
        return false;
    }
    for(int f = 0;f<faces.size();f++){
        for(int co = 0;co<faces[f].colours.size();co++){
            if(faces[f].colours[co].colour != c.faces[f].colours[co].colour){
                return false;
            }
        }
    }
    return true;
}


void Cube::generate_pieces_and_faces(){
    double mod_extremity = (double)(order-1)/2;
    double i = -mod_extremity;  
    int interval_dist = 1;

    vector<Colour>Up;
    vector<Colour>Down;
    vector<Colour>Right;
    vector<Colour>Left;
    vector<Colour>Front;
    vector<Colour>Back;

    while(i <= mod_extremity){
        double j = -mod_extremity;
        while(j <= mod_extremity){
            double k = -mod_extremity;
            while(k <= mod_extremity){
                if(abs(i) != mod_extremity && abs(j) != mod_extremity && abs(k) != mod_extremity){
                    k+=interval_dist;
                    continue;
                }
                else{
                    Position pp(i,j,k);
                    double div_factor = 4;
                    //Colours;
                    vector<Colour>p_colours;
                    string type;
                    if(abs(i) == mod_extremity && abs(j) == abs(i) && abs(k) == abs(j)){
                        type = "corner";
                        Position c_pos1(i, j - (j/(abs(j)))*interval_dist/div_factor, k - (k/abs(k))*interval_dist/div_factor);
                        if(i < 0){
                            string c = "W";
                            Colour c_p(c_pos1, c);
                            p_colours.push_back(c_p);
                            Left.push_back(c_p);
                        }else{
                            string c = "Y";
                            Colour c_p(c_pos1, c);
                            p_colours.push_back(c_p);
                            Right.push_back(c_p);
                        }
                        Position c_pos2(i - (i/(abs(i)))*interval_dist/div_factor, j, k - (k/abs(k))*interval_dist/div_factor);
                        if(j < 0){
                            string c = "G";
                            Colour c_p(c_pos2, c);
                            p_colours.push_back(c_p);
                            Down.push_back(c_p);
                        }else{
                            string c = "B";
                            Colour c_p(c_pos2, c);
                            p_colours.push_back(c_p);
                            Up.push_back(c_p);
                        }
                        Position c_pos3(i - (i/(abs(i)))*interval_dist/div_factor,j - (j/(abs(j)))*interval_dist/div_factor,k);
                        if(k < 0){
                            string c = "O";
                            Colour c_p(c_pos3, c);
                            p_colours.push_back(c_p);
                            Back.push_back(c_p);
                        }else{
                            string c = "R";
                            Colour c_p(c_pos3, c);
                            p_colours.push_back(c_p);
                            Front.push_back(c_p);
                        }
                    }
                    else if((abs(i) == mod_extremity && abs(j) == mod_extremity && abs(k) != mod_extremity) 
                            || (abs(i) == mod_extremity && abs(j) != mod_extremity && abs(k) == mod_extremity) 
                            || (abs(i) != mod_extremity && abs(j) == mod_extremity && abs(k) == mod_extremity)){
                        type = "edge";
                        if(abs(i) == mod_extremity && abs(j) == mod_extremity){
                            Position c_pos1(i - (i/abs(i))*interval_dist/div_factor,j,k);
                            Position c_pos2(i,j - (j/abs(j))*interval_dist/div_factor,k);
                            if(i < 0 && j < 0){
                                string c1 = "G";
                                Colour c1_p(c_pos1, c1);
                                p_colours.push_back(c1_p);
                                Down.push_back(c1_p);
                                string c2 = "W";
                                Colour c2_p(c_pos2, c2);
                                p_colours.push_back(c2_p);
                                Left.push_back(c2_p);                                
                            }
                            else if(i > 0 && j < 0){
                                string c1 = "G";
                                Colour c1_p(c_pos1, c1);
                                p_colours.push_back(c1_p);
                                Down.push_back(c1_p);
                                string c2 = "Y";
                                Colour c2_p(c_pos2, c2);
                                p_colours.push_back(c2_p);
                                Right.push_back(c2_p);
                            }
                            else if(i > 0 && j > 0){
                                string c1 = "B";
                                Colour c1_p(c_pos1, c1);
                                p_colours.push_back(c1_p);
                                Up.push_back(c1_p);
                                string c2 = "Y";
                                Colour c2_p(c_pos2, c2);
                                p_colours.push_back(c2_p);
                                Right.push_back(c2_p);
                            }
                            else {
                                string c1 = "B";
                                Colour c1_p(c_pos1, c1);
                                p_colours.push_back(c1_p);
                                Up.push_back(c1_p);
                                string c2 = "W";
                                Colour c2_p(c_pos2, c2);
                                p_colours.push_back(c2_p);
                                Left.push_back(c2_p);
                            }
                        }
                        else if(abs(j) == mod_extremity && abs(k) == mod_extremity){
                            Position c_pos1(i,j - (j/abs(j))*interval_dist/div_factor,k);
                            Position c_pos2(i,j,k - (k/abs(k))*interval_dist/div_factor);
                            if(k < 0 && j < 0){
                                string c1 = "O";
                                Colour c1_p(c_pos1, c1);
                                p_colours.push_back(c1_p);
                                Back.push_back(c1_p);
                                string c2 = "G";
                                Colour c2_p(c_pos2, c2);
                                p_colours.push_back(c2_p);
                                Down.push_back(c2_p);
                            }
                            else if(k > 0 && j < 0){
                                string c1 = "R";
                                Colour c1_p(c_pos1, c1);
                                p_colours.push_back(c1_p);
                                Front.push_back(c1_p);
                                string c2 = "G";
                                Colour c2_p(c_pos2, c2);
                                p_colours.push_back(c2_p);
                                Down.push_back(c2_p);
                            }
                            else if(k > 0 && j > 0){
                                string c1 = "R";
                                Colour c1_p(c_pos1, c1);
                                p_colours.push_back(c1_p);
                                Front.push_back(c1_p);
                                string c2 = "B";
                                Colour c2_p(c_pos2, c2);
                                p_colours.push_back(c2_p);
                                Up.push_back(c2_p);
                            }
                            else{
                                string c1 = "O";
                                Colour c1_p(c_pos1, c1);
                                p_colours.push_back(c1_p);
                                Back.push_back(c1_p);
                                string c2 = "B";
                                Colour c2_p(c_pos2, c2);
                                p_colours.push_back(c2_p);
                                Up.push_back(c2_p);
                            }
                        }
                        else{
                            Position c_pos1(i - (i/abs(i))*interval_dist/div_factor,j,k);
                            Position c_pos2(i,j,k - (k/abs(k))*interval_dist/div_factor);
                            if(i < 0 && k < 0){
                                string c1 = "O";
                                Colour c1_p(c_pos1, c1);
                                p_colours.push_back(c1_p);
                                Back.push_back(c1_p);
                                string c2 = "W";
                                Colour c2_p(c_pos2, c2);
                                p_colours.push_back(c2_p);
                                Left.push_back(c2_p);
                            }
                            else if(i > 0 && k < 0){
                                string c1 = "O";
                                Colour c1_p(c_pos1, c1);
                                p_colours.push_back(c1_p);
                                Back.push_back(c1_p);
                                string c2 = "Y";
                                Colour c2_p(c_pos2, c2);
                                p_colours.push_back(c2_p);
                                Right.push_back(c2_p);
                            }
                            else if(i > 0 && k > 0){
                                string c1 = "R";
                                Colour c1_p(c_pos1, c1);
                                p_colours.push_back(c1_p);
                                Front.push_back(c1_p);
                                string c2 = "Y";
                                Colour c2_p(c_pos2, c2);
                                p_colours.push_back(c2_p);
                                Right.push_back(c2_p);
                            }
                            else{
                                string c1 = "R";
                                Colour c1_p(c_pos1, c1);
                                p_colours.push_back(c1_p);
                                Front.push_back(c1_p);
                                string c2 = "W";
                                Colour c2_p(c_pos2, c2);
                                p_colours.push_back(c2_p);
                                Left.push_back(c2_p);
                            }
                        }
                    }
                    else{
                        type = "center";
                        Position c_pos(i, j,k);
                        if(abs(i) == mod_extremity){
                            if(i == mod_extremity){
                                string c = "Y";
                                Colour c_p(c_pos, c);
                                p_colours.push_back(c_p);
                                Right.push_back(c_p);
                            } 
                            else{
                                string c = "W";
                                Colour c1_p(c_pos, c);
                                p_colours.push_back(c1_p);
                                Left.push_back(c1_p);
                            }
                        }
                        else if(abs(j) == mod_extremity){
                            if(j == mod_extremity){
                                string c = "B";
                                Colour c1_p(c_pos, c);
                                p_colours.push_back(c1_p);
                                Up.push_back(c1_p);
                            } 
                            else{
                                string c = "G";
                                Colour c1_p(c_pos, c);
                                p_colours.push_back(c1_p);
                                Down.push_back(c1_p);
                            }
                        }
                        else if(abs(k) == mod_extremity){
                            if(k == mod_extremity){
                                string c = "R";
                                Colour c1_p(c_pos, c);
                                p_colours.push_back(c1_p);
                                Front.push_back(c1_p);
                            } 
                            else{
                                string c = "O";
                                Colour c1_p(c_pos, c);
                                p_colours.push_back(c1_p);
                                Back.push_back(c1_p);
                            }
                        }
                    }
                    Piece pe(pp, p_colours, type);
                    pieces.push_back(pe);
                }
                k+=interval_dist;
            }   
            j+=interval_dist;
        }
        i+=interval_dist;
    }

    faces.push_back(Face(Up, "Up", order));
    faces.push_back(Face(Left, "Left", order));
    faces.push_back(Face(Front, "Front", order));
    faces.push_back(Face(Right, "Right", order));
    faces.push_back(Face(Back, "Back", order));
    faces.push_back(Face(Down, "Down", order));
}


void Cube::group_faces(){
    double mod_extremity = (double)((double)(order-1)/2);
    int index = order*order - 1;
    int cr = index,cl = index,cu = index,cd = index,cf = index,cb = index;
    for(int p = 0;p<pieces.size();p++){
        for(int c = 0;c<pieces[p].colours.size();c++){
            if(pieces[p].colours[c].position.v[0] ==  mod_extremity){
                faces[3].colours[cr] = pieces[p].colours[c];
                cr--;
            }
            else if(pieces[p].colours[c].position.v[1] ==  mod_extremity){
                faces[0].colours[cu] = pieces[p].colours[c];
                cu--;
            }
            else if(pieces[p].colours[c].position.v[2] ==  mod_extremity){
                faces[2].colours[cf] = pieces[p].colours[c];
                cf--;
            }
            else if(pieces[p].colours[c].position.v[0] ==  -mod_extremity){
                faces[1].colours[cl] = pieces[p].colours[c];
                cl--;
            }
            else if(pieces[p].colours[c].position.v[1] ==  -mod_extremity){
                faces[5].colours[cd] = pieces[p].colours[c];
                cd--;
            }
            else{
                faces[4].colours[cb] = pieces[p].colours[c];
                cb--;
            }
        }
    }
}


void Cube::generate_possible_transforms(){
    vector<char>r_layer = {'R','U','F','L','D','B'};
    int max;
    if(order%2!=0){
        max = (int)(ceil((double)(order)/2));
    } else{
        max = order/2;
    }
    for(int l = 0;l<r_layer.size();l++){
        for(int j = 1;j<=max;j++){
            if((j == max && l>(r_layer.size()-1)/2)){
                if(order%2 == 0){
                    string t_s;
                    t_s.push_back(r_layer[l]);
                    string str = to_string(j);
                    t_s.push_back(str[0]);
                    string t_s1 = t_s;
                    t_s1.push_back('*');
                    Transformation T1(t_s);
                    possible_transforms.push_back(T1);
                    Transformation T2(t_s1);
                    possible_transforms.push_back(T2);
                }
                continue;
            }
            string t_s;
            t_s.push_back(r_layer[l]);
            string str = to_string(j);
            t_s.push_back(str[0]);
            string t_s1 = t_s;
            t_s1.push_back('*');
            Transformation T1(t_s);
            possible_transforms.push_back(T1);
            Transformation T2(t_s1);
            possible_transforms.push_back(T2);
        }
    }
}

Cube::~Cube(){}


Cube::Cube(int order)
      :order{order}{
        generate_pieces_and_faces();
        generate_possible_transforms();
        solved_state = pieces;
    }


bool Cube::is_solved(){
    for(auto f:faces){
        for(int c = 1;c<f.colours.size();c++){
            if(f.colours[c].colour != f.colours[c-1].colour){
                return false;
            }
        }
    }
    return true;
}


void Cube::transform(Transformation T){
    double mod_extremity = (double)(order-1)/2; 
    double interval_dist = 1;
    if(T.layer_move[0] == 'R'){
        double val = mod_extremity - (T.layer_index - 1)*interval_dist;
        for(int p = 0;p<pieces.size();p++){
            if(pieces[p].point.v[0] == val){
                // cout << p;
                T.transformation_matrix.rotate(pieces[p]);
                // cout << p;
            }
        }
    }
    else if(T.layer_move[0] == 'L'){
        double val = -mod_extremity + (T.layer_index - 1)*interval_dist;
        for(int p = 0;p<pieces.size();p++){
            if(pieces[p].point.v[0] == val){
                T.transformation_matrix.rotate(pieces[p]);
            }
        }
    }
    else if(T.layer_move[0] == 'U'){
        double val = mod_extremity - (T.layer_index - 1)*interval_dist;
        for(int p = 0;p<pieces.size();p++){
            if(pieces[p].point.v[1] == val){
                T.transformation_matrix.rotate(pieces[p]);
            }
        }
    }
    else if(T.layer_move[0] == 'D'){
        double val = -mod_extremity + (T.layer_index - 1)*interval_dist;
        for(int p = 0;p<pieces.size();p++){
            if(pieces[p].point.v[1] == val){
                T.transformation_matrix.rotate(pieces[p]);
            }
        }
    }
    else if(T.layer_move[0] == 'F'){
        double val = mod_extremity - (T.layer_index - 1)*interval_dist;
        for(int p = 0;p<pieces.size();p++){
            if(pieces[p].point.v[2] == val){
                T.transformation_matrix.rotate(pieces[p]);
            }
        }
    }
    else{
        double val = -mod_extremity + (T.layer_index - 1)*interval_dist;
        for(int p = 0;p<pieces.size();p++){
            if(pieces[p].point.v[2] == val){
                T.transformation_matrix.rotate(pieces[p]);
            }
        }
    }
    Transformation T_o = T;
    invert_transform(T_o);
    if(transforms_to_solve.size()>0 && (T.layer_move[0] == T_o.layer_move[0])){
        if(T.transformation_matrix.matrix == T_o.transformation_matrix.matrix){
            transforms_to_solve.pop_back();
        }
        else{
            transforms_to_solve.push_back(T);
        }
    }
    group_faces();
}


vector<Transformation>Cube::random_transforms(int n){
    vector<Transformation>transforms;
    for(int i = 0;i<n;i++){
        int index = rand()%possible_transforms.size() - 1;
        transforms.push_back(possible_transforms[index]);
    }
    return transforms;
}


void Cube::scramble(int n){
    cout << "Scramble: " << endl;
    vector<Transformation>transforms = random_transforms(n);
    for(auto T:transforms){
        try{
            cout << T;
            transform(T);
        } catch(...){
            continue;
        }
    }
    cout << endl;
}


void Cube::scramble(vector<Transformation>transforms){
    for(auto T:transforms){
        transform(T);
        // cout << T;
    }
}


double Cube::how_close_to_solved(Piece &p){
    double val = 0;
    for(int c = 0;c<p.colours.size();c++){
        Piece solved_piece(Position(p.point.x, p.point.y, p.point.z), p.colours, p.type);
        // Sum of euclidean distances between solved and current positions of piece. 
        val += solved_piece.point.euclidean_dist(p.colours[c].position); 
        solved_piece.~Piece();
    }
    return val;
}


int Cube::best_transform_index(Piece &p, Colour &c, bool &need_commutator, int &commutator_order){
    
}


void Cube::solve_colour_safely(Piece &p, Colour &c, vector<Transformation>&moves_to_solve){
    
}


void Cube::solve_first_layer(vector<Transformation>&moves_to_solve){
    // For cubes 2x2x2 and 3x3x3
    // Default starts with Green bottom layer. 
    // First we find a green colour on a face. If it is an even layered cube, we the bottom face. 
    // Randomly select a green coloured piece;
    string base_colour = "G";
    for(int p = 0;p<pieces.size();p++){
        for(int c = 0;c<pieces[p].colours.size();c++){
            if(pieces[p].colours[c].colour == base_colour){
                solve_colour_safely(pieces[p], pieces[p].colours[c], moves_to_solve);
            }
        }
    }
    //If odd layered cube, centers are well known. start with green center.  
    /* {

    } */
   // now the rest of the bottom layer needs to be solved. 



}


void Cube::solve_top_layer(vector<Transformation>&moves_to_solve){

}


vector<Transformation> Cube::human_solve(){  
    vector<Transformation>moves_to_solve;

    if(order == 2){
        solve_first_layer(moves_to_solve);
        solve_top_layer(moves_to_solve);
    }

    return moves_to_solve;
}


void Cube::solve(){
    // cout << transforms_to_solve.size();
    // for (int i = transforms_to_solve.size() - 1;i >= 0;i--){
    //     Transformation T_o = transforms_to_solve[i];
    //     invert_transform(T_o);
    //     transform(T_o);
    //     cout << T_o;
    // }
    cout << endl << "Solution" << endl;
    vector<Transformation>moves_to_solve = human_solve();
    for(int i = 0;i<moves_to_solve.size();i++){
        cout << moves_to_solve[i] << " ";
    }
}


ostream &operator<<(ostream &os, const Cube &obj){

    string data;
    for(auto p:obj.pieces){
        data.push_back('{');
        data.push_back('\n');
        for(auto c:p.colours){
            data.push_back('[');
            data.push_back('(');

            for(auto e:c.position.v){
                string es = to_string(e);
                for(char n = 0;n<es.size();n++){
                    data.push_back(es[n]);
                }
                data.push_back(',');
            }

            data.push_back(')');
            data.push_back('|');
            for(char a:c.colour){
                data.push_back(a);
            }

            data.push_back(']');
            data.push_back('\n');
        }

        data.push_back('}');
        data.push_back('\n');
    }

    fstream fw;
    fw.open("cubedata.txt",std::ofstream::out | std::ofstream::trunc);

    if (fw.is_open()){
        //store array contents to text file
        for (int i = 0; i < data.size(); i++) {
            fw << data[i];
        }
        fw.close();
    } else{
        cout << "File not opened";
    }
    fw.close();
    // os << data;


    os << endl << "Cube: " << endl;
    os << obj.faces[0];
    for(int i = 0;i<obj.order;i++){
        os << endl;
        for(int f = 1;f<obj.faces.size()-1;f++){
            for(int c = 0;c<obj.order;c++){
                os << obj.faces[f].colours[c + i*obj.order].colour << " ";
            }
        }
    }
    os << obj.faces[obj.faces.size()-1] << endl;

    return os;
}
