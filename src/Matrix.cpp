#ifndef MATRIXHEADER
#define MATRIXHEADER
#include <array>
#include <iostream>
#include <iomanip> 
#include "Point.cpp"
#include "Vector.cpp"

template<int H=4, int W=4> class matrix {
  private:
  std::array<std::array<double, W>, H> m;

  public:
  matrix(){}
  matrix(std::array<std::array<double, W>, H> x) : m(x) {}  
  matrix(std::initializer_list<std::initializer_list<double>> x) {
        int i = 0;
        for (const auto& row : x) {
            std::copy(row.begin(), row.end(), m[i].begin());
            i++;
        }
    }

  int width() const { return W; }
  int height() const { return H; }
  
  std::array<double, W>& operator[] (int i) { return m[i]; }
  const std::array<double, W>& operator[] (int i) const { return m[i]; }

  template<int HH, int WW>
  matrix<H, WW> operator * (const matrix<HH, WW> & t) const {
    static_assert(W == HH, "Matrix dimensions do not match");
    matrix<H, WW> a {};

    for(int i=0; i<H; i++)
      for(int j=0; j<WW; j++)
        for(int k=0; k<W; k++)
          a[i][j] += m[i][k] * t[k][j];

    this->print();
    std::cout << "****\n";
    t.print();
    std::cout << "===\n";
    a.print();
    std::cout << "---\n";

    return a;
  }

  point operator* (const point pt) const {
    matrix<W, 1> p;
    
    p[0][0] = pt.getX();
    p[1][0] = pt.getY();
    p[2][0] = pt.getZ();

    for(int i=3; i < W; i++) p[i][0] = 1;

    p = (*this) * p;
    
    auto w = p[3][0];

    // return point(p[0][0] / w, p[1][0] / w, p[2][0] / w);
    return point(p[0][0], p[1][0], p[2][0]);
  }

  vetor operator* (const vetor v) const { 
    matrix<W, 1> p;
    
    p[0][0] = v.getX();
    p[1][0] = v.getY();
    p[2][0] = v.getZ();

    p = (*this) * p;

    return vetor(p[0][0], p[1][0], p[2][0]);
  }

  const void print() const {
    for(int i=0; i<H; i++){
      for(int j=0; j<W; j++)
        std::cout << (*this)[i][j] << " ";
      if(W > 1 || i == H-1) std::cout << "\n";
    }
  }

};

matrix<4, 4> MatrixZRotation(double theta){
  matrix<4, 4> t ({
    {cos(theta), -sin(theta), 0, 0},
    {sin(theta), cos(theta), 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  });

  return t;
}

const double PI = acos(-1);

int main(){
  std::cout << std::fixed << std::setprecision(8);

  matrix<4, 4> m ({
    {10, 0, 0, 0},
    {0, -2, 0, 0},
    {0, 0, 6, 0},
    {0, 0, 0,  1}
  });
  auto r = MatrixZRotation(PI/4);
    
  auto a = m * r ;

  point p (1, 2, 3);

  (a * p).print();

  std::cout << "-------\n";
  std::cout << "-------\n";

  (m * (r * p)).print();

  return 0;
}


/*

M * P
P * M

*/

#endif